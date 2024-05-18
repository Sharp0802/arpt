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

#ifndef IP_H
#define IP_H

#include "module.h"

namespace arpt
{
    template<uint8_t Version>
    class IPImpl;

    template<>
    class __attribute__((packed, aligned(1))) IPImpl<4> final
    {
        std::array<uint8_t, 4> m_Data;

    public:
        explicit IPImpl(const uint8_t* data);

        explicit IPImpl(std::array<uint8_t, 4> data);

        [[nodiscard]]
        operator std::array<uint8_t, 4>() const;

        [[nodiscard]]
        operator const uint8_t*() const;

        [[nodiscard]]
        std::string ToString() const;
    };

    template<>
    struct ARPType<IPImpl<4>>
    {
        uint16_t Value = 0x0800;
    };

    template<>
    class __attribute__((packed, aligned(1))) IPImpl<6> final
    {
        std::array<uint8_t, 16> m_Data;

    public:
        explicit IPImpl(const uint8_t* data);

        explicit IPImpl(std::array<uint8_t, 16> data);

        [[nodiscard]]
        operator std::array<uint8_t, 16>() const;

        [[nodiscard]]
        operator const uint8_t*() const;

        [[nodiscard]]
        std::string ToString() const;
    };

    template<>
    struct ARPType<IPImpl<6>>
    {
        static_assert("IPv6 doesn't use ARP; see NDP (https://en.wikipedia.org/wiki/Neighbor_Discovery_Protocol).");
    };

    class IP
    {
        std::uint8_t m_Version;
        IPImpl<4>    m_4;
        IPImpl<6>    m_6;

    public:
        IP(const uint8_t* data, uint8_t version);

        explicit IP(std::array<uint8_t, 16> data);

        explicit IP(std::array<uint8_t, 4> data);

        [[nodiscard]]
        IPImpl<4> __GetIPv4() const;

        [[nodiscard]]
        IPImpl<6> __GetIPv6() const;

        [[nodiscard]]
        uint8_t __GetVersion() const;

        __declspec(property(get=__GetIPv4))
        IPImpl<4> IPv4;

        __declspec(property(get=__GetIPv6))
        IPImpl<6> IPv6;

        __declspec(property(get=__GetVersion))
        uint8_t Version;

        [[nodiscard]]
        operator const uint8_t*() const;

        [[nodiscard]]
        std::string ToString() const;
    };
}

#endif //IP_H
