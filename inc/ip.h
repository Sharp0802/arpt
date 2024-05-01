#ifndef IP_H
#define IP_H

#include <array>
#include <cstdint>
#include <format>

namespace arpt
{
    template<uint8_t Version>
    class IPImpl;

    template<>
    class IPImpl<4>
    {
        std::array<uint8_t, 4> m_Data;

    public:
        explicit IPImpl(const uint8_t* data);

        explicit IPImpl(std::array<uint8_t, 4> data);

        [[nodiscard]]
        operator std::array<uint8_t, 4>() const;

        [[nodiscard]]
        operator const uint8_t*() const;

        [[nodiscard]]
        std::string ToString() const;
    };

    class IP
    {
        std::uint8_t m_Version;
        IPImpl<4> m_4;

    public:
        IP(const uint8_t* data, uint8_t version);

        explicit IP(std::array<uint8_t, 16> data); // not ready
        explicit IP(std::array<uint8_t, 4> data);

        [[nodiscard]]
        IPImpl<4> __GetIPv4() const;

        [[nodiscard]]
        uint8_t __GetVersion() const;

        __declspec(property(get=__GetIPv4))
        IPImpl<4> IPv4;

        __declspec(property(get=__GetVersion))
        uint8_t Version;

        [[nodiscard]]
        operator const uint8_t*() const;

        [[nodiscard]]
        std::string ToString() const;
    };
}

#endif //IP_H
