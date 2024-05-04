#ifndef MAC_H
#define MAC_H

#include "module.h"

namespace arpt
{
    class MAC
    {
        std::array<uint8_t, 6> m_Data;

    public:
        explicit MAC(const uint8_t* data);

        explicit MAC(std::array<uint8_t, 6> data);

        [[nodiscard]]
        operator std::array<uint8_t, 6>() const;

        [[nodiscard]]
        operator const uint8_t*() const;

        [[nodiscard]]
        std::string ToString() const;
    };
}

#endif //MAC_H
