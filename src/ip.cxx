/*
 * lilac (LILAC)
 * Copyright (C) Yeong-won Seo  2024
 *
 * This file is part of LILAC
 *
 * LILAC is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3, or (at your option) any later
 * version.
 *
 * LILAC is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LILAC; see the file LICENSE.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#include "module.h"

namespace arpt
{
    IPImpl<4>::IPImpl(const uint8_t* data)
        : m_Data{
            data[0], data[1],
            data[2], data[3]
        }
    {
    }

    IPImpl<4>::IPImpl(const std::array<uint8_t, 4> data) : m_Data(data)
    {
    }

    IPImpl<4>::operator std::array<uint8_t, 4>() const
    {
        return m_Data;
    }

    IPImpl<4>::operator const uint8_t*() const
    {
        return m_Data.data();
    }

    std::string IPImpl<4>::ToString() const
    {
        return std::format("{}.{}.{}.{}", m_Data[0], m_Data[1], m_Data[2], m_Data[3]);
    }

    IPImpl<6>::IPImpl(const uint8_t* data)
        : m_Data{
            data[0], data[1], data[2], data[3],
            data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11],
            data[12], data[13], data[14], data[15],
        }
    {
    }

    IPImpl<6>::IPImpl(std::array<uint8_t, 16> data) : m_Data(data)
    {
    }

    IPImpl<6>::operator std::array<uint8_t, 16>() const
    {
        return m_Data;
    }

    IPImpl<6>::operator const uint8_t*() const
    {
        return m_Data.data();
    }

    std::string IPImpl<6>::ToString() const
    {
        return std::format(
            "{:02X}{:02X}:{:02X}{:02X}:{:02X}{:02X}:{:02X}{:02X}:"
            "{:02X}{:02X}:{:02X}{:02X}:{:02X}{:02X}:{:02X}{:02X}",
            m_Data[0], m_Data[1], m_Data[2], m_Data[3],
            m_Data[4], m_Data[5], m_Data[6], m_Data[7],
            m_Data[8], m_Data[9], m_Data[10], m_Data[11],
            m_Data[12], m_Data[13], m_Data[14], m_Data[15]);
    }

    IP::IP(const uint8_t* data, uint8_t version)
        : m_4({ 0, 0, 0, 0 }),
          m_6({ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 })
    {
        switch (version)
        {
            case 4:
                m_Version = 4;
                m_4 = IPImpl<4>(data);
                break;
            case 6:
                m_Version = 6;
                m_6 = IPImpl<6>(data);
                break;
            default:
                throw std::system_error(
                    std::make_error_code(std::errc::invalid_argument),
                    "Invalid internet protocol version");
        }
    }

    IP::IP(const std::array<uint8_t, 16> data) : IP(data.data(), 6)
    {
    }

    IP::IP(const std::array<uint8_t, 4> data) : IP(data.data(), 4)
    {
    }

    IPImpl<4> IP::__GetIPv4() const
    {
        if (m_Version == 6)
            throw std::system_error(
                std::make_error_code(std::errc::operation_not_supported),
                "Couldn't get IPv4 from IPv6");
        return m_4;
    }

    IPImpl<6> IP::__GetIPv6() const
    {
        if (m_Version == 4)
            throw std::system_error(
                std::make_error_code(std::errc::operation_not_supported),
                "Couldn't get IPv6 from IPv4");
        return m_6;
    }

    uint8_t IP::__GetVersion() const
    {
        return m_Version;
    }

    IP::operator const uint8_t*() const
    {
        switch (m_Version)
        {
            case 4:
                return m_4;
            case 6:
                return m_6;
            default:
                throw std::system_error(
                    std::make_error_code(std::errc::address_family_not_supported),
                    "Invalid internet protocol version");
        }
    }

    std::string IP::ToString() const
    {
        switch (m_Version)
        {
            case 4:
                return m_4.ToString();
            case 6:
                return m_6.ToString();
            default:
                throw std::system_error(
                    std::make_error_code(std::errc::address_family_not_supported),
                    "Invalid internet protocol version");
        }
    }
}
