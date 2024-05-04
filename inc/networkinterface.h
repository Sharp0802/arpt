#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include "module.h"

namespace arpt
{
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
        explicit NetworkInterfaceListBase(std::vector<NetworkInterface> interfaces);

    public:
        [[nodiscard]]
        std::vector<NetworkInterface> Get();
    };

    template<OS>
    class NetworkInterfaceListImpl;

    using NetworkInterfaceList = std::shared_ptr<NetworkInterfaceListBase>;

    [[nodiscard]]
    NetworkInterfaceList QueryNetworkInterfaceList();
}

#endif //NETWORKINTERFACE_H
