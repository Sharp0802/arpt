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

#ifndef MAC_H
#define MAC_H

#include "module.h"

namespace arpt
{
    class MAC
    {
        std::array<uint8_t, 6> m_Data;

    public:
        explicit MAC(const uint8_t* data);

        explicit MAC(std::array<uint8_t, 6> data);

        [[nodiscard]]
        operator std::array<uint8_t, 6>() const;

        [[nodiscard]]
        operator const uint8_t*() const;

        [[nodiscard]]
        std::string ToString() const;
    };
}

#endif //MAC_H
