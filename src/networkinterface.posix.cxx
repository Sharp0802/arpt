#include "networkinterface.posix.h"

#include <cstring>
#include <map>
#include <ranges>
#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/types.h>
#include <ifaddrs.h>

#if __linux__
#include <linux/if_packet.h> // sockaddr_ll
#include <linux/rtnetlink.h>
#else
#include <net/if_dl.h> // sockaddr_dl
#include <netlink/netlink.h>
#include <netlink/netlink_route.h>
#endif

#define THROW_ERRNO(msg) throw std::system_error(errno, std::generic_category(), msg)

namespace
{
    size_t ReadNetlinkSocket(const int fd, std::vector<uint8_t>& buffer, const uint32_t seq)
    {
        auto* cursor = buffer.data();

        nlmsghdr* hdr;
        size_t msgLength = 0;

        const auto pid = getpid();
        do
        {
            if (msgLength > buffer.size())
                THROW_ERRNO("Insufficient buffer size");

            ssize_t readLength = recv(fd, cursor, buffer.size() - msgLength, 0);
            if (readLength < 0)
                THROW_ERRNO("recv()");

            hdr = reinterpret_cast<nlmsghdr*>(cursor);
            if (!NLMSG_OK(hdr, readLength) || hdr->nlmsg_type == NLMSG_ERROR)
                THROW_ERRNO("NLMSG_ERROR");

            if (hdr->nlmsg_type == NLMSG_DONE)
                break;

            cursor += readLength;
            msgLength += readLength;

            if (~hdr->nlmsg_flags & NLM_F_MULTI)
                break;
        } while (hdr->nlmsg_seq != seq || hdr->nlmsg_pid != pid);

        return msgLength;
    }
}

namespace arpt
{
#define ADDRESS_FAMILY_NOT_SUPPORTED()                                 \
    throw std::system_error(                                           \
        std::make_error_code(std::errc::address_family_not_supported), \
        "Couldn't handle address family")

    std::map<std::string, IP> QueryDefaultGateway()
    {
        std::vector<uint8_t> buffer(BUFSIZ, 0);
        int seq = 0;


        auto* msg        = reinterpret_cast<nlmsghdr*>(buffer.data());
        msg->nlmsg_len   = NLMSG_LENGTH(sizeof(rtmsg));
        msg->nlmsg_type  = RTM_GETROUTE;
        msg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;
        msg->nlmsg_seq   = seq++;
        msg->nlmsg_pid   = getpid();


#if __linux__
        const int sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE);
#else
        const int sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
#endif
        if (sock < 0)
            THROW_ERRNO("socket()");
        if (send(sock, msg, msg->nlmsg_len, 0) < 0)
        {
            close(sock);
            THROW_ERRNO("send()");
        }
        auto len = ReadNetlinkSocket(sock, buffer, seq);
        close(sock);


        std::map<std::string, IP> map;
        for (; NLMSG_OK(msg, len); msg = NLMSG_NEXT(msg, len))
        {
            const auto* rtmsg = static_cast<::rtmsg*>(NLMSG_DATA(msg));
            if ((rtmsg->rtm_family != AF_INET &&
                 rtmsg->rtm_family != AF_INET6) ||
                rtmsg->rtm_table != RT_TABLE_MAIN)
                continue;

            // TODO : Support INET6
            if (rtmsg->rtm_family == AF_INET6)
                continue;

            char name[16];
            const uint8_t* address = nullptr;

            auto rtlen = RTM_PAYLOAD(msg);
            for (auto attr = RTM_RTA(rtmsg); RTA_OK(attr, rtlen); attr = RTA_NEXT(attr, rtlen))
            {
                switch (attr->rta_type)
                {
                case RTA_OIF:
                    if_indextoname(*static_cast<uint32_t*>(RTA_DATA(attr)), name);
                    break;
                case RTA_GATEWAY:
                    address = static_cast<uint8_t*>(RTA_DATA(attr));
                    break;

                default:
                    break;
                }
            }

            if (*name && (address && *address))
            {
                map.emplace(name, IP{ address, rtmsg->rtm_family == AF_INET ? 4UI8 : 6UI8 });
            }
        }

        return map;
    }

#if __linux__
    constexpr sa_family_t AF_DATALINK = AF_PACKET;
#else
    constexpr sa_family_t AF_DATALINK = AF_LINK;
#endif

    MAC RetrieveLinkFromIFA(const sockaddr* sa)
    {
        if (sa->sa_family != AF_DATALINK)
            ADDRESS_FAMILY_NOT_SUPPORTED();

        const uint8_t* p;
#if __linux__
        p = reinterpret_cast<const sockaddr_ll*>(sa)->sll_addr;
#else
        p = reinterpret_cast<const uint8_t*>(LLADDR(reinterpret_cast<const sockaddr_dl*>(sa)));
#endif
        return MAC{ p };
    }

    uint8_t RetrieveMaskFromIFA(const ifaddrs* ifa)
    {
        switch (ifa->ifa_netmask->sa_family)
        {
        case AF_INET:
            return std::popcount(reinterpret_cast<sockaddr_in*>(ifa->ifa_netmask)->sin_addr.s_addr);
        case AF_INET6:
        {
#if __ANDROID__
            uint8_t mask = 0;
            for (auto i = 0; i < 4; ++i)
                mask += std::popcount(reinterpret_cast<sockaddr_in6*>(ifa->ifa_netmask)->sin6_addr.s6_addr32[i]);
            return mask;
#else
            return std::popcount(
                *reinterpret_cast<__uint128_t*>(
                    &reinterpret_cast<sockaddr_in6*>(ifa->ifa_netmask)->sin6_addr
                )
            );
#endif
        }
        default:
            ADDRESS_FAMILY_NOT_SUPPORTED();
        }
    }

    IP RetrieveAddressFromIFA(const sockaddr* sa)
    {
        switch (sa->sa_family)
        {
        case AF_INET:
            return {
                reinterpret_cast<const uint8_t*>(
                    &reinterpret_cast<const sockaddr_in*>(sa)->sin_addr.s_addr
                ),
                4
            };
        case AF_INET6:
            return { reinterpret_cast<const sockaddr_in6*>(sa)->sin6_addr.s6_addr, 6 };
        default:
            ADDRESS_FAMILY_NOT_SUPPORTED();
        }
    }

    std::optional<IP> RetrieveBroadcastFromIFA(const ifaddrs* ifa)
    {
        if (~ifa->ifa_flags & IFF_BROADCAST)
            return std::nullopt;

        switch (ifa->ifa_broadaddr->sa_family)
        {
        case AF_INET:
            return IP{
                reinterpret_cast<const uint8_t*>(
                    &reinterpret_cast<const sockaddr_in*>(ifa->ifa_broadaddr)->sin_addr.s_addr
                ),
                4
            };
        case AF_INET6:
            return IP{
                reinterpret_cast<const sockaddr_in6*>(ifa->ifa_broadaddr)->sin6_addr.s6_addr,
                6
            };
        default:
            ADDRESS_FAMILY_NOT_SUPPORTED();
        }
    }

    std::vector<NetworkInterface> RetrieveInterfaceList()
    {
        ifaddrs* ifa;
        if (getifaddrs(&ifa) < 0)
            THROW_ERRNO("getifaddrs()");

        std::map<std::string, std::pair<ifaddrs*, ifaddrs*>> ifas;
        for (auto* ifap = ifa; ifap; ifap = ifap->ifa_next)
        {
            switch (ifap->ifa_addr->sa_family)
            {
            case AF_PACKET:
                ifas[ifap->ifa_name].first = ifap;
                break;

            case AF_INET:
                ifas[ifap->ifa_name].second = ifap;
                break;

            case AF_INET6:
                if (!ifas[ifap->ifa_name].second)
                    ifas[ifap->ifa_name].second = ifap;
                break;

            default:
                break;
            }
        }

        auto gateways = QueryDefaultGateway();

        std::vector<NetworkInterface> nis;
        nis.reserve(ifas.size());
        for (auto [link, net]: ifas | std::views::values)
        {
            if (link == nullptr || net == nullptr)
                continue;

            std::optional<IP> gateway = gateways.contains(net->ifa_name)
                ? std::optional(gateways.extract(net->ifa_name).mapped())
                : std::nullopt;
            nis.push_back(std::make_shared<NetworkInterfaceImpl<POSIX>>(net, link, gateway));
        }

        freeifaddrs(ifa);

        return nis;
    }

    NetworkInterfaceListImpl<POSIX>::NetworkInterfaceListImpl()
        : NetworkInterfaceListBase(RetrieveInterfaceList())
    {
    }

    NetworkInterfaceImpl<POSIX>::NetworkInterfaceImpl(
        const ifaddrs* netIFA,
        const ifaddrs* linkIFA,
        const std::optional<IP> gateway)
        : m_Name(netIFA->ifa_name),
          m_Link(RetrieveLinkFromIFA(linkIFA->ifa_addr)),
          m_Mask(RetrieveMaskFromIFA(netIFA)),
          m_Address(RetrieveAddressFromIFA(netIFA->ifa_addr)),
          m_Broadcast(RetrieveBroadcastFromIFA(netIFA)),
          m_Gateway(gateway)
    {
    }

    MAC NetworkInterfaceImpl<POSIX>::__GetLink() const
    {
        return m_Link;
    }

    uint8_t NetworkInterfaceImpl<POSIX>::__GetMask() const
    {
        return m_Mask;
    }

    IP NetworkInterfaceImpl<POSIX>::__GetAddress() const
    {
        return m_Address;
    }

    std::optional<IP> NetworkInterfaceImpl<POSIX>::__GetBroadcast() const
    {
        return m_Broadcast;
    }

    std::optional<IP> NetworkInterfaceImpl<POSIX>::__GetGateway() const
    {
        return m_Gateway;
    }

    std::string NetworkInterfaceImpl<POSIX>::__GetName() const
    {
        return m_Name;
    }
}