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

#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include "ip.h"
#include "mac.h"
#include "module.h"
#include "os.h"

namespace arpt
{
    struct NetworkInterfaceQueryOptions
    {
        bool EnableIPv6;

        constexpr bool operator==(const NetworkInterfaceQueryOptions&) const;
    };

    constexpr bool NetworkInterfaceQueryOptions::operator==(const NetworkInterfaceQueryOptions& other) const
    {
        return EnableIPv6 == other.EnableIPv6;
    }

    class NetworkInterfaceBase;

    using NetworkInterface = std::shared_ptr<NetworkInterfaceBase>;

    class NetworkInterfaceBase
    {
    public:
        virtual ~NetworkInterfaceBase() = default;

        [[nodiscard]]
        virtual MAC __GetLink() const = 0;

        [[nodiscard]]
        virtual uint8_t __GetMask() const = 0;

        [[nodiscard]]
        virtual IP __GetAddress() const = 0;

        [[nodiscard]]
        virtual std::optional<IP> __GetBroadcast() const = 0;

        [[nodiscard]]
        virtual std::optional<IP> __GetGateway() const = 0;

        [[nodiscard]]
        virtual std::string __GetName() const = 0;

        __declspec(property(get=__GetLink))
        MAC Link;

        __declspec(property(get=__GetMask))
        uint8_t Mask;

        __declspec(property(get=__GetAddress))
        IP Address;

        __declspec(property(get=__GetBroadcast))
        std::optional<IP> Broadcast;

        __declspec(property(get=__GetGateway))
        std::optional<IP> Gateway;

        __declspec(property(get=__GetName))
        std::string Name;
    };

    template<OS>
    class NetworkInterfaceImpl;

    class NetworkInterfaceListBase
    {
        std::vector<NetworkInterface> m_Interfaces;

    protected:
        NetworkInterfaceQueryOptions m_Options;

        explicit NetworkInterfaceListBase(
            std::vector<NetworkInterface> interfaces,
            NetworkInterfaceQueryOptions  options);

    public:
        [[nodiscard]]
        std::vector<NetworkInterface> Get();
    };

    template<OS>
    class NetworkInterfaceListImpl;

    using NetworkInterfaceList = std::shared_ptr<NetworkInterfaceListBase>;

    [[nodiscard]]
    NetworkInterfaceList QueryNetworkInterfaceList(NetworkInterfaceQueryOptions options);
}

#endif //NETWORKINTERFACE_H
