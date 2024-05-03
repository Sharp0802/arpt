#ifndef NETWORKINTERFACEWINDOWS_H
#define NETWORKINTERFACEWINDOWS_H

#if _WIN32

#include <winsock2.h>
#include <windows.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <ipmib.h>

#include "networkinterface.h"

namespace arpt
{
	template<>
	class NetworkInterfaceListImpl<Windows> final : public NetworkInterfaceListBase
	{
	public:
		NetworkInterfaceListImpl();
	};

	template<>
	class NetworkInterfaceImpl<Windows> final : public NetworkInterfaceBase
	{
		std::string m_Name;
		MAC m_Link;
		uint8_t m_Mask;
		IP m_Address;
		std::optional<IP> m_Broadcast;
		std::optional<IP> m_Gateway;

	public:
		NetworkInterfaceImpl(const IP_ADAPTER_ADDRESSES* adapter);

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

#endif //NETWORKINTERFACEWINDOWS_H
