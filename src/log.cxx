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
    auto getThreadId()
    {
        static std::atomic_uint8_t count{ 0 };
        thread_local uint8_t       id = count.fetch_add(1);
        return id;
    }

    auto getHeader()
    {
        auto now = std::chrono::system_clock::now();

        return std::format(
            "[{:03d} {:%FT%T}]",
            getThreadId(),
            now);
    }

    std::ostream& errs()
    {
        return std::cerr << "\x1b[31mERR " << getHeader() << ' ';
    }

    std::ostream& outs()
    {
        return std::cout << "--- " << getHeader() << ' ';
    }

    std::ostream& endl(std::ostream& os)
    {
        os << "\x1b[m";
        return std::flush(std::endl(os));
    }
}
