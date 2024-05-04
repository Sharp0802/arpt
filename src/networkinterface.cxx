#include "module.h"

namespace arpt
{
    NetworkInterfaceListBase::NetworkInterfaceListBase(
        std::vector<NetworkInterface> interfaces,
        NetworkInterfaceQueryOptions options)
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
        return std::make_shared<NetworkInterfaceListImpl<Current>>(options);
    }
}
