#include <utility>

#include "networkinterface.h"

#include "log.h"

#if _WIN32
#include "networkinterface.windows.h"
#else
#include "networkinterface.posix.h"
#endif

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
