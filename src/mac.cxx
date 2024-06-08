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

#include "module.h"
#include "mac.h"

namespace arpt
{
    MAC::MAC() : m_Data()
    {
    }

    MAC::MAC(const std::string& str) : m_Data()
    {
        char dummy = ':';
        std::stringstream ss(str);
        for (auto i = 0; i < 6; ++i)
        {
            if (i != 0)
                ss >> dummy;
            if (dummy != ':')
                throw std::format_error("no character other than ':' cannot be delimiter of MAC address");

            int32_t v;
            ss >> std::hex >> v;
            if (v > std::numeric_limits<uint8_t>::max())
                throw std::format_error("any representation of a segment of MAC address cannot be greater than 255");

            m_Data[i] = v;
        }
    }

    MAC::MAC(const uint8_t* data)
        : m_Data{
            data[0], data[1], data[2],
            data[3], data[4], data[5]
        }
    {
    }

    MAC::MAC(const std::array<uint8_t, 6> data) : m_Data(data)
    {
    }

    MAC::operator std::array<unsigned char, 6>() const
    {
        return m_Data;
    }

    MAC::operator const unsigned char*() const
    {
        return m_Data.data();
    }

    std::string MAC::ToString() const
    {
        return std::format(
            "{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}",
            m_Data[0], m_Data[1], m_Data[2],
            m_Data[3], m_Data[4], m_Data[5]);
    }

    const MAC MAC::Broadcast{ std::array<uint8_t, 6>{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };
}
