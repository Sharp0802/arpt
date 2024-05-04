#include "module.h"

namespace arpt
{
    NetworkInterfaceListBase::NetworkInterfaceListBase(std::vector<NetworkInterface> interfaces)
        : m_Interfaces(std::move(interfaces))
    {
    }

    std::vector<NetworkInterface> NetworkInterfaceListBase::Get()
    {
        return m_Interfaces;
    }

    NetworkInterfaceList QueryNetworkInterfaceList()
    {
        return std::make_shared<NetworkInterfaceListImpl<Current>>();
    }
}
