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

#ifndef NETWORKINTERFACEW_H
#define NETWORKINTERFACEW_H

#if __unix__

#include "module.h"

namespace arpt
{
    template<>
    class NetworkInterfaceListImpl<POSIX> final : public NetworkInterfaceListBase
    {
    public:
        explicit NetworkInterfaceListImpl(NetworkInterfaceQueryOptions options);
    };

    template<>
    class NetworkInterfaceImpl<POSIX> final : public NetworkInterfaceBase
    {
        std::string       m_Name;
        MAC               m_Link;
        uint8_t           m_Mask;
        IP                m_Address;
        std::optional<IP> m_Broadcast;
        std::optional<IP> m_Gateway;

    public:
        NetworkInterfaceImpl(
            const ifaddrs*    netIFA,
            const ifaddrs*    linkIFA,
            std::optional<IP> gateway);

        ~NetworkInterfaceImpl() override = default;

        [[nodiscard]]
        MAC __GetLink() const override;

        [[nodiscard]]
        uint8_t __GetMask() const override;

        [[nodiscard]]
        IP __GetAddress() const override;

        [[nodiscard]]
        std::optional<IP> __GetBroadcast() const override;

        [[nodiscard]]
        std::optional<IP> __GetGateway() const override;

        [[nodiscard]]
        std::string __GetName() const override;
    };
}

#endif

#endif //NETWORKINTERFACEW_H
