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

#include <algorithm>

namespace arpt
{
    template<typename T>
    struct BigEndian
    {
    private:
        T m_Value;

        constexpr static T InverseBytes(T value)
        {
            auto *begin = reinterpret_cast<char*>(&value), *end = begin + sizeof(T);
            std::reverse(begin, end);
            return value;
        }

    public:
        // NOLINTNEXTLINE(*-explicit-constructor)
        BigEndian(T value) : m_Value(InverseBytes(value))
        {
        }

        // NOLINTNEXTLINE(*-explicit-constructor)
        operator T() const
        {
            return InverseBytes(m_Value);
        }
    };

    using be8_t = BigEndian<uint8_t>;
    using be16_t = BigEndian<uint16_t>;
    using be32_t = BigEndian<uint32_t>;
    using be64_t = BigEndian<uint64_t>;
}
