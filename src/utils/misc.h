/*
 *   ___ ___        _     _       _
 *  |_ _/ _ \      | |   (_)_ __ | | __
 *   | | | | |_____| |   | | '_ \| |/ /
 *   | | |_| |_____| |___| | | | |   <
 *  |___\___/      |_____|_|_| |_|_|\_\
 *
 * Header only driver library for interfacing IO-Link devices and masters
 * written in modern C++
 *
 * Version: 0.1.0
 * URL: https://github.com/ekondayan/libiolink.git
 *
 * Copyright (c) 2019 Emil Kondayan
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef MISC_H
#define MISC_H

#include <stdint.h>
#include <type_traits>

namespace iolink::utils
{
    template <typename Type, std::size_t bits, std::size_t lshift = 0>
    constexpr Type createBitMask()
    {
        static_assert (std::is_integral_v<Type> && std::is_unsigned_v<Type> && sizeof(Type)*8 >= bits, "Bitmask can not be larger than the type size");
        static_assert ((sizeof(Type)*8 - bits) >= lshift, "Too much left shifting. The bitmask will be shifted out");

        Type mask = 0;

        for(std::size_t i = 0; i < bits; ++i)
            mask = (mask << 1) | 0x01;

        return mask << lshift;
    }

    template <typename Type>
    constexpr Type pow(Type x, int y)
    {
        return y == 0 ? Type(1) : x * pow(x, y-1);
    }

    bool isLittleEndian()
    {
        int num_endianness = 1;
        return *((char*)&num_endianness) == 0x01;
    }
}

#endif // MISC_H
