#include "module.h"

namespace arpt
{
    IPImpl<4>::IPImpl(const uint8_t* data)
        : m_Data{
            data[0], data[1],
            data[2], data[3]
        }
    {
    }

    IPImpl<4>::IPImpl(const std::array<uint8_t, 4> data) : m_Data(data)
    {
    }

    IPImpl<4>::operator std::array<unsigned char, 4>() const
    {
        return m_Data;
    }

    IPImpl<4>::operator const unsigned char*() const
    {
        return m_Data.data();
    }

    std::string IPImpl<4>::ToString() const
    {
        return std::format("{}.{}.{}.{}", m_Data[0], m_Data[1], m_Data[2], m_Data[3]);
    }

    IP::IP(const uint8_t* data, uint8_t version)
        : m_4(data)
    {
        switch (version)
        {
        case 4:
            m_Version = 4;
            break;
        case 6:
            m_Version = 6;
            throw std::system_error(
                std::make_error_code(std::errc::address_family_not_supported),
                "IPv6 is not supported yet");
        default:
            throw std::system_error(
                std::make_error_code(std::errc::invalid_argument),
                "Invalid internet protocol version");
        }
    }

    IP::IP(const std::array<uint8_t, 16> data) : IP(data.data(), 6)
    {
    }

    IP::IP(const std::array<uint8_t, 4> data) : IP(data.data(), 4)
    {
    }

    IPImpl<4> IP::__GetIPv4() const
    {
        return m_4;
    }

    uint8_t IP::__GetVersion() const
    {
        return m_Version;
    }

    IP::operator const unsigned char*() const
    {
        switch (m_Version)
        {
        case 4:
            return m_4;
        default:
            throw std::system_error(
                std::make_error_code(std::errc::address_family_not_supported),
                "Invalid internet protocol version");
        }
    }

    std::string IP::ToString() const
    {
        switch (m_Version)
        {
        case 4:
            return m_4.ToString();
        default:
            throw std::system_error(
                std::make_error_code(std::errc::address_family_not_supported),
                "Invalid internet protocol version");
        }
    }
}
