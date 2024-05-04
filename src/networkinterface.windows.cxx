#if _WIN32

#include "module.h"

namespace arpt
{
	std::string UTF16ToUTF8(const std::wstring& ws)
	{
		if (ws.empty())
			return "";

		const auto u8len = WideCharToMultiByte(
			CP_UTF8,
			0,
			ws.data(),
			(int)ws.size(),
			nullptr,
			0,
			nullptr,
			nullptr);
		if (u8len < 0)
			throw std::runtime_error(std::format("WideCharToMultiByte() failed: {}", u8len));

		std::string result(u8len, 0);
		WideCharToMultiByte(
			CP_UTF8,
			0,
			ws.data(),
			(int)ws.size(),
			result.data(),
			u8len,
			nullptr,
			nullptr);
		return result;
	}

	[[nodiscard]]
	std::vector<NetworkInterface> QueryInterfaces()
	{
		constexpr ULONG flags = GAA_FLAG_INCLUDE_GATEWAYS;

		ULONG length = BUFSIZ;
		auto* adapterAddress = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new uint8_t[length]);
		if (GetAdaptersAddresses(
			    AF_UNSPEC,
			    flags,
			    nullptr,
			    adapterAddress,
			    &length) == ERROR_BUFFER_OVERFLOW)
		{
			delete[] adapterAddress;
			adapterAddress = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(new uint8_t[length]);
		}
		if (auto ret = GetAdaptersAddresses(
			AF_UNSPEC,
			flags,
			nullptr,
			adapterAddress,
			&length); ret != NO_ERROR)
		{
			throw std::system_error(
				std::make_error_code(std::errc::io_error),
				std::format("GetAdaptersAddresses failed with error: {}", ret));
		}

		std::vector<NetworkInterface> nics;
		for (auto* adapter = adapterAddress; adapter; adapter = adapter->Next)
		{
			try
			{
				auto ptr = std::make_shared<NetworkInterfaceImpl<Windows>>(adapter);
				nics.emplace_back(ptr);
			}
			catch (std::exception& e)
			{
				errs() << e.what() << endl;
			}
		}

		delete[] adapterAddress;

		return nics;
	}

	[[nodiscard]]
	std::string GetNameFromAdapter(const IP_ADAPTER_ADDRESSES* adapter)
	{
		return UTF16ToUTF8(adapter->FriendlyName);
	}

	[[nodiscard]]
	MAC GetLinkAddressFromAdapter(const IP_ADAPTER_ADDRESSES* adapter)
	{
		if (adapter->PhysicalAddressLength == 0)
			return MAC({ 0, 0, 0, 0, 0, 0 });

		if (adapter->PhysicalAddressLength != 6)
			throw std::system_error(
				std::make_error_code(std::errc::address_family_not_supported),
				std::format("An physical address size is not supported; should be 6 ('{}')",
					GetNameFromAdapter(adapter)));

		return MAC(adapter->PhysicalAddress);
	}

	template<typename TAddress>
	[[nodiscard]]
	std::optional<IP> ExtractIPFromAddresses(TAddress address)
	{
		if (address == nullptr)
			return std::nullopt;

		for (; address; address = address->Next)
		{
			switch (address->Address.lpSockaddr->sa_family)
			{
			case AF_INET6:
				// TODO : Support IPv6
				break;
			case AF_INET:
				return IP(reinterpret_cast<uint8_t*>(address->Address.lpSockaddr->sa_data) + 2, 4);

				// NOT HANDLED : address-families supported on windows are only AF_INET and AF_INET6.
			default:
				break;
			}
		}

		return std::nullopt;
	}

	[[nodiscard]]
	std::optional<IP> GetGatewayFromAdapterInfo(const IP_ADAPTER_ADDRESSES* adapter)
	{
		if (adapter->FirstGatewayAddress == nullptr)
			return std::nullopt;

		if (const auto ip = ExtractIPFromAddresses(adapter->FirstGatewayAddress); ip.has_value())
			return ip;

		errs()
				<< "Supported gateway for adapter '" << adapter->AdapterName
				<< "' not found despite there is gateway address for adapter;"
		        << "Is there bound any supported address? (AF_INET)" << endl;

		return std::nullopt;
	}

	[[nodiscard]]
	uint8_t GetMaskFromAdapterInfo(const IP_ADAPTER_ADDRESSES* adapter)
	{
		for (auto* address = adapter->FirstUnicastAddress; address; address = address->Next)
		{
			switch (address->Address.lpSockaddr->sa_family)
			{
			case AF_INET6:
				// TODO : Support IPv6
					break;
			case AF_INET:
				return address->OnLinkPrefixLength;

				// NOT HANDLED : address-families supported on windows are only AF_INET and AF_INET6.
			default:
				break;
			}
		}

		throw std::system_error(
			std::make_error_code(std::errc::invalid_seek),
			std::format("Couldn't find valid mask from adapter '{}'.", adapter->AdapterName));
	}

	[[nodiscard]]
	IP GetAddressFromAdapterInfo(const IP_ADAPTER_ADDRESSES* adapter)
	{
		if (const auto ip = ExtractIPFromAddresses(adapter->FirstUnicastAddress); ip.has_value())
			return ip.value();

		throw std::system_error(
			std::make_error_code(std::errc::invalid_seek),
			std::format("Couldn't find valid address from adapter '{}'.", adapter->AdapterName));
	}

	[[nodiscard]]
	std::optional<IP> GetBroadcastFromAdapterInfo(const IP_ADAPTER_ADDRESSES* adapter)
	{
		if (!GetGatewayFromAdapterInfo(adapter).has_value())
			return std::nullopt;

		ULONG broadcast;

		const auto mask = GetMaskFromAdapterInfo(adapter);
		const auto ip = GetAddressFromAdapterInfo(adapter);

		ConvertLengthToIpv4Mask(mask, &broadcast);
		broadcast = ~broadcast;
		broadcast |= *reinterpret_cast<const uint32_t*>(static_cast<const uint8_t*>(ip.IPv4));

		return IP(reinterpret_cast<uint8_t*>(&broadcast), 4);
	}

	NetworkInterfaceListImpl<Windows>::NetworkInterfaceListImpl(): NetworkInterfaceListBase(QueryInterfaces())
	{
	}

	NetworkInterfaceImpl<Windows>::NetworkInterfaceImpl(const IP_ADAPTER_ADDRESSES* adapter)
		: m_Name(GetNameFromAdapter(adapter)),
		  m_Link(GetLinkAddressFromAdapter(adapter)),
		  m_Mask(GetMaskFromAdapterInfo(adapter)),
		  m_Address(GetAddressFromAdapterInfo(adapter)),
		  m_Broadcast(GetBroadcastFromAdapterInfo(adapter)),
		  m_Gateway(GetGatewayFromAdapterInfo(adapter))
	{
	}

	MAC NetworkInterfaceImpl<Windows>::__GetLink() const
	{
		return m_Link;
	}

	uint8_t NetworkInterfaceImpl<Windows>::__GetMask() const
	{
		return m_Mask;
	}

	IP NetworkInterfaceImpl<Windows>::__GetAddress() const
	{
		return m_Address;
	}

	std::optional<IP> NetworkInterfaceImpl<Windows>::__GetBroadcast() const
	{
		return m_Broadcast;
	}

	std::optional<IP> NetworkInterfaceImpl<Windows>::__GetGateway() const
	{
		return m_Gateway;
	}

	std::string NetworkInterfaceImpl<Windows>::__GetName() const
	{
		return m_Name;
	}
}

#endif
