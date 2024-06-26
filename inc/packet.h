/*
 * arpt (ARPT)
 * Copyright (C) Yeong-won Seo  2024
 *
 * This file is part of ARPT
 *
 * ARPT is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3, or (at your option) any later version.
 *
 * ARPT is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ARPT; see the file LICENSE.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <utility>

template <typename, std::size_t>
concept IndexedAny = true;

namespace arpt
{
    // TODO : Remove these legacy pack-indexing stuffs implemented before pack-indexing (C++26) standardized
    // NOTE : Android NDK requries these code because its poor C++26 support
    template<size_t I, typename, typename... T>
    struct TypePackIndex
    {
        using Type = TypePackIndex<I - 1, T...>::Type;
    };

    template<typename T0, typename... T>
    struct TypePackIndex<0, T0, T...>
    {
        using Type = T0;
    };

    template <std::size_t I, typename T0, typename... T>
    struct ValuePackIndex
    {
        static typename TypePackIndex<I, T0, T...>::Type&& Get(T0&&, T&&... args)
        {
            return ValuePackIndex<I - 1, T...>::Get(std::forward<T>(args)...);
        }
    };

    template <typename T0, typename... T>
    struct ValuePackIndex<0, T0, T...>
    {
        static T0&& Get(T0&& arg0, T&&...)
        {
            return arg0;
        }
    };

    template<typename... T>
    class Packet;

    template<typename... T>
    struct __packed Pack final
    {
        friend class Packet<T...>;

    private:
        template<size_t N, size_t... I>
        [[nodiscard]]
        static consteval size_t Sum()
        {
            size_t v[sizeof...(I)] = { I... };
            size_t sum = 0;
            for (size_t i = 0; i < N; ++i)
                sum += v[i];
            return sum;
        }

        static const constinit size_t N = Sum<sizeof...(T), sizeof(T)...>();

        std::array<uint8_t, N> m_Buffer{};

    public:
        template<size_t I>
        [[nodiscard]]
        auto& Get()
        {
            auto* p = &m_Buffer[Sum<I, sizeof(T)...>()];
            return *reinterpret_cast<typename TypePackIndex<I, T...>::Type*>(p);
        }

        // NOLINTNEXTLINE(*-explicit-constructor)
        Pack(T&&... args);
    };

    template<typename... T>
    class Packet
    {
        Pack<T...> m_Pack;

    public:
        explicit Packet(Pack<T...>&& pack) : m_Pack(pack)
        {
        }

        template<size_t I>
        [[nodiscard]]
        auto& Get()
        {
            return m_Pack.template Get<I>();
        }

        void* operator&()
        {
            return m_Pack.m_Buffer.data();
        }

        // std::size
        [[nodiscard]]
        size_t size() const
        {
            return sizeof(m_Pack);
        }
    };

    template<size_t I, typename... T>
    struct __PackInit
    {
        static void Init(Pack<T...>& p, T&&... args)
        {
            p.template Get<I>() = ValuePackIndex<I, T...>::Get(std::forward<T>(args)...);
            __PackInit<I - 1, T...>::Init(p, std::forward<T>(args)...);
        }
    };

    template<typename... T>
    struct __PackInit<0, T...>
    {
        static void Init(Pack<T...>& p, T&&... args)
        {
            p.template Get<0>() = ValuePackIndex<0, T...>::Get(std::forward<T>(args)...);
        }
    };

    template<typename... T>
    Pack<T...>::Pack(T&&... args)
    {
        __PackInit<sizeof...(T) - 1, T...>::Init(*this, std::forward<T>(args)...);
    }
}

namespace std
{
    template<std::size_t I, typename... T>
    auto& get(arpt::Pack<T...>& pack) noexcept
    {
        return pack.template Get<I>();
    }

    template<std::size_t I, typename... T>
    auto& get(arpt::Packet<T...>& pack) noexcept
    {
        return pack.template Get<I>();
    }
}
