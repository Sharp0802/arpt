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

#pragma once

#include "endians.h"

namespace arpt
{
    template<typename T>
    struct ARPType;

    template<typename THardware, typename TProtocol>
    struct ARPPeer
    {
        THardware HardwareAddress;
        TProtocol ProtocolAddress;
    };

    enum ARPOperation : uint16_t
    {
        ARP_Request = 1,
        ARP_Reply   = 2
    };

    class __attribute__((packed, aligned(1))) UnintendedARP final
    {
        struct
        {
            uint16_t     m_HardwareType;
            uint16_t     m_ProtocolType;
            uint8_t      m_HardwareLength;
            uint8_t      m_ProtocolLength;
            ARPOperation m_Operation;
        } Header;

        std::vector<uint8_t> m_Sender;
        std::vector<uint8_t> m_Target;

    public:
        // TODO : Implement ARP parser
        UnintendedARP(void* ptr, size_t size);
    };

    template<typename THardware, typename TProtocol>
    class __attribute__((packed, aligned(1))) IntendedARP final
    {
    public:
        using Peer = ARPPeer<THardware, TProtocol>;

    private:
        be16_t                        m_HardwareType   = ARPType<THardware>::Value;
        be16_t                        m_ProtocolType   = ARPType<TProtocol>::Value;
        be8_t                         m_HardwareLength = sizeof(THardware);
        be8_t                         m_ProtocolLength = sizeof(TProtocol);
        be16_t                        m_Operation;
        ARPPeer<THardware, TProtocol> m_Sender;
        ARPPeer<THardware, TProtocol> m_Target;

    public:
        IntendedARP(const ARPOperation op, Peer sender, Peer target)
            : m_Operation(op), m_Sender(sender), m_Target(target)
        {
        }

        [[nodiscard]]
        ARPOperation __GetOperation() const
        {
            return static_cast<ARPOperation>(static_cast<uint16_t>(m_Operation));
        }

        [[nodiscard]]
        Peer __GetSender() const
        {
            return m_Sender;
        }

        [[nodiscard]]
        Peer __GetTarget() const
        {
            return m_Target;
        }

        __declspec(property(get=__GetOperation))
        ARPOperation Operation;

        __declspec(property(get=__GetSender))
        Peer Sender;

        __declspec(property(get=__GetTarget))
        Peer Target;
    };
}
