/*
 * arpt (ARPT)
 * Copyright (C) Yeong-won Seo  2024
 *
 * This file is part of ARPT
 *
 * ARPT is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3, or (at your option) any later version.
 *
 * ARPT is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ARPT; see the file LICENSE.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#include "arp.h"
#include "log.h"

namespace arpt
{
    ARP::ARP(const ARPOperation op, ARPPeer sender, ARPPeer target): m_Operation(op), m_Sender(sender), m_Target(target)
    {
    }

    ARPOperation ARP::__GetOperation() const
    {
        return static_cast<ARPOperation>(static_cast<uint16_t>(m_Operation));
    }

    ARPPeer ARP::__GetSender() const
    {
        return m_Sender;
    }

    ARPPeer ARP::__GetTarget() const
    {
        return m_Target;
    }

    Pack<Ethernet, ARP> CreateARPPack(const NetworkInterface& dev, ARPOperation op, ARPPeer sender, ARPPeer target)
    {
        MAC link = dev->Link;
        if (memcmp(sender.HardwareAddress, MAC{}, sizeof link) != 0)
        {
            link = sender.HardwareAddress;
        }

        const int ifindex = static_cast<int>(if_nametoindex(dev->Name.data()));

        sockaddr_ll sll{
            AF_PACKET,
            htons(ETH_P_ARP),
            ifindex,
            htons(ARPHRD_ETHER),
            PACKET_BROADCAST,
            sizeof(MAC)
        };
        sll.sll_addr[6] = 0;
        sll.sll_addr[7] = 0;

        return {
            { target.HardwareAddress, link, EthernetProtocol::ARP },
            {
                op,
                {
                    link,
                    sender.ProtocolAddress
                },
                {
                    MAC{},
                    target.ProtocolAddress
                }
            }
        };
    }

    ARPPacket::ARPPacket(
        const NetworkInterface& dev,
        const ARPOperation op,
        const ARPPeer sender,
        const ARPPeer target)
        : m_Interface(dev),
          m_Packet(CreateARPPack(dev, op, sender, target))
    {
    }

    int ARPPacket::Send()
    {
        const auto fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
        if (fd < 0)
        {
            errs() << "socket(): " << strerror(errno) << "\n";
            return -1;
        }

        sockaddr_ll sll{
            .sll_family = AF_PACKET,
            .sll_ifindex = static_cast<int>(if_nametoindex(m_Interface->Name.data()))
        };
        if (bind(fd, reinterpret_cast<sockaddr*>(&sll), sizeof sll) < 0)
        {
            errs() << "bind(): " << strerror(errno) << "\n";
            return -1;
        }

        sockaddr_ll dst{
            AF_PACKET,
            htons(ETH_P_ARP),
            static_cast<int>(if_nametoindex(m_Interface->Name.data())),
            htons(ARPHRD_ETHER),
            PACKET_BROADCAST,
            sizeof(MAC)
        };
        memcpy(dst.sll_addr, m_Interface->Link, sizeof m_Interface->Link);

        if (sendto(
            fd,
            &m_Packet,
            std::size(m_Packet),
            0,
            reinterpret_cast<sockaddr*>(&dst),
            sizeof dst) < 0)
        {
            errs() << "sendto(): " << strerror(errno) << "\n";
            return -1;
        }

        close(fd);
        return 0;
    }
}
