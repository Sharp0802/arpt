#include "module.h"

namespace arpt
{
    MAC::MAC(const uint8_t* data)
        : m_Data{
            data[0], data[1], data[2],
            data[3], data[4], data[5]
        }
    {
    }

    MAC::MAC(const std::array<uint8_t, 6> data) : m_Data(data)
    {
    }

    MAC::operator std::array<unsigned char, 6>() const
    {
        return m_Data;
    }

    MAC::operator const unsigned char*() const
    {
        return m_Data.data();
    }

    std::string MAC::ToString() const
    {
        return std::format(
            "{:02X}:{:02X}:{:02X}:{:02X}:{:02X}:{:02X}",
            m_Data[0], m_Data[1], m_Data[2],
            m_Data[3], m_Data[4], m_Data[5]);
    }
}