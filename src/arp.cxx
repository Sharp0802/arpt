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
}
