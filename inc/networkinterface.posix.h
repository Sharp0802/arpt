#ifndef NETWORKINTERFACEW_H
#define NETWORKINTERFACEW_H

#if __unix__

#include "module.h"

namespace arpt
{
    template<>
    class NetworkInterfaceListImpl<POSIX> final : public NetworkInterfaceListBase
    {
    public:
        explicit NetworkInterfaceListImpl(NetworkInterfaceQueryOptions options);
    };

    template<>
    class NetworkInterfaceImpl<POSIX> final : public NetworkInterfaceBase
    {
        std::string m_Name;
        MAC m_Link;
        uint8_t m_Mask;
        IP m_Address;
        std::optional<IP> m_Broadcast;
        std::optional<IP> m_Gateway;

    public:
        NetworkInterfaceImpl(
            const ifaddrs* netIFA,
            const ifaddrs* linkIFA,
            std::optional<IP> gateway);

        ~NetworkInterfaceImpl() override = default;

        [[nodiscard]]
        MAC __GetLink() const override;

        [[nodiscard]]
        uint8_t __GetMask() const override;

        [[nodiscard]]
        IP __GetAddress() const override;

        [[nodiscard]]
        std::optional<IP> __GetBroadcast() const override;

        [[nodiscard]]
        std::optional<IP> __GetGateway() const override;

        [[nodiscard]]
        std::string __GetName() const override;
    };
}

#endif

#endif //NETWORKINTERFACEW_H
