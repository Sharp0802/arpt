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
#include "os.h"
#include "networkinterface.h"

#include <mutex>

#include "networkinterface.posix.h"
#include "networkinterface.windows.h"

static arpt::NetworkInterfaceQueryOptions s_CachedOptions{};
static arpt::NetworkInterfaceList s_Cached = nullptr;

static std::mutex s_Mutex{};

namespace arpt
{
    NetworkInterfaceListBase::NetworkInterfaceListBase(
        std::vector<NetworkInterface> interfaces,
        NetworkInterfaceQueryOptions  options)
        : m_Interfaces(std::move(interfaces)),
          m_Options(options)
    {
    }

    std::vector<NetworkInterface> NetworkInterfaceListBase::Get()
    {
        return m_Interfaces;
    }

    NetworkInterfaceList QueryNetworkInterfaceList(NetworkInterfaceQueryOptions options)
    {
        std::lock_guard lock{ s_Mutex };

        if (!s_Cached || s_CachedOptions != options || options.NoCache)
            s_Cached = std::make_shared<NetworkInterfaceListImpl<Current>>(options);
        return s_Cached;
    }
}
