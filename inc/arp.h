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
}

#define ARPTYPE_DEFINED 1

#include "ip.h"
#include "mac.h"

#undef ARPTYPE_DEFINED

namespace arpt
{
    struct ARPPeer
    {
        MAC       HardwareAddress;
        IPImpl<4> ProtocolAddress;
    };

    enum ARPOperation : uint16_t
    {
        ARP_Request = 1,
        ARP_Reply   = 2
    };

    class __attribute__((packed, aligned(1))) ARP final
    {
        be16_t  m_HardwareType   = ARPType<MAC>::Value;
        be16_t  m_ProtocolType   = ARPType<IPImpl<4>>::Value;
        be8_t   m_HardwareLength = sizeof(MAC);
        be8_t   m_ProtocolLength = sizeof(IPImpl<4>);
        be16_t  m_Operation;
        ARPPeer m_Sender;
        ARPPeer m_Target;

    public:
        ARP(ARPOperation op, ARPPeer sender, ARPPeer target);

        [[nodiscard]]
        ARPOperation __GetOperation() const;

        [[nodiscard]]
        ARPPeer __GetSender() const;

        [[nodiscard]]
        ARPPeer __GetTarget() const;

        __declspec(property(get=__GetOperation))
        ARPOperation Operation;

        __declspec(property(get=__GetSender))
        ARPPeer Sender;

        __declspec(property(get=__GetTarget))
        ARPPeer Target;
    };
}
