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

#include "eth.h"

namespace arpt
{
    Ethernet::Ethernet(const MAC destination, const MAC source, EthernetProtocol protocol): m_Destination(destination),
        m_Source(source),
        m_Protocol(static_cast<uint16_t>(protocol))
    {
    }

    MAC Ethernet::__GetDestination() const
    {
        return m_Destination;
    }

    MAC Ethernet::__GetSource() const
    {
        return m_Source;
    }

    EthernetProtocol Ethernet::__GetProtocol() const
    {
        return static_cast<EthernetProtocol>(static_cast<uint16_t>(m_Protocol));
    }
}
