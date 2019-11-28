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

#ifndef CONVERT_H
#define CONVERT_H

#include "libiolink/src/utils/misc.h"
#include "libiolink/src/utils/inc.h"

#include "exception.h"

namespace iolink::utils
{
    template <typename T>
    T decodeFromHexString(const string_t &str)
    {
        auto hexCharToInt = [](const char ch)
        {
            static const char* const lut_upper = "0123456789ABCDEF";
            static const char* const lut_lower = "0123456789abcdef";

            const char* p = std::lower_bound(lut_lower, lut_lower + 16, ch);
            if (*p == ch)
                return uint8_t(p - lut_lower);

            p = std::lower_bound(lut_upper, lut_upper + 16, ch);
            if(*p == ch)
                return uint8_t(p - lut_upper);

            throw iolink::utils::exception_argument(__func__, "Input string contains a char that is not a hex digit");
        };

        if constexpr(std::is_same<T, bool>::value)
        {
            std::regex reg_true("(true|1)", std::regex_constants::icase);
            if(std::regex_match(str, reg_true))
                return true;

            std::regex reg_false("(false|0)", std::regex_constants::icase);
            if(std::regex_match(str, reg_false))
                return false;

            throw iolink::utils::exception_argument(__func__, "Value does not represent boolean");
        }
        else if constexpr(std::is_floating_point<T>::value)
        {
            if (str.length() != 8)
                throw iolink::utils::exception_argument(__func__, "Input string must be 8 bytes long");

            float    decoded = 0;
            uint32_t *decoded_ptr = reinterpret_cast<uint32_t*>(&decoded);

            if(isLittleEndian())
            {
                for (int i = 0; i < 8; i += 2)
                {
                    *decoded_ptr |= (hexCharToInt(str[i]) << 4) | hexCharToInt(str[i + 1]);
                    if( i < 8 ) *decoded_ptr <<= 8;
                }
            }
            else
            {
                for (int i = 7; i >= 1; i -= 2)
                {
                    *decoded_ptr |= (hexCharToInt(str[i - 1]) << 4) | hexCharToInt(str[i]);
                    if(i > 1) *decoded_ptr <<= 8;
                }
            }

            return decoded;
        }
        else if constexpr(std::is_integral<T>::value)
        {
            size_t len = str.length();

            // Every byte is encoded with two byte characters
            if(sizeof(T) * 2 != len)
                throw iolink::utils::exception_argument(__func__, "Input string does not corespond with the integer length");

            T output = 0;

            if(isLittleEndian())
            {
                for (size_t i = 0; i < len; i += 2)
                {
                    output |= (hexCharToInt(str[i]) << 4) | hexCharToInt(str[i + 1]);
                    if( (i+2) < len ) output <<= 8;
                }
            }
            else
            {
                for (size_t i = len; i >= 2; i += 2)
                {
                    output |= (hexCharToInt(str[i - 1]) << 4) | hexCharToInt(str[i]);
                    if( (i+2) > 2 ) output <<= 8;
                }
            }

            return output;
        }
        else if constexpr(std::is_same<T, string_t>::value || std::is_same<T, vector_t>::value)
        {
            size_t len = str.length();
            if (len & 1)
                throw iolink::utils::exception_argument(__func__, "Input string has odd length");

            T output;
            output.reserve(len / 2);

            for (size_t i = 0; i < len; i += 2)
                output.push_back((hexCharToInt(str[i]) << 4) | hexCharToInt(str[i + 1]));

            return output;
        }
        else
        {
            throw iolink::utils::exception_argument(__func__, "Unsupported input type");
        }
    }

    template<typename T>
    string_t encodeToHexString(T value)
    {
        // Look up table
        static const char* const lut_upper = "0123456789ABCDEF";

        if constexpr(std::is_same<T, bool>::value)
        {
            return "0" + lut_upper[value & 0x0F];
        }
        else if constexpr(std::is_floating_point<T>::value)
        {
            float f = static_cast<float>(value);
            uint8_t *ch = reinterpret_cast<uint8_t*>(&f);
            string_t output;

            output.reserve(8);

            if(isLittleEndian())
                for(int i = 3; i >= 0; --i)
                {
                    output.push_back(lut_upper[ch[i] >> 4]);
                    output.push_back(lut_upper[ch[i] & 0x0F]);
                }
            else
                for(int i = 0; i < 4; ++i)
                {
                    output.push_back(lut_upper[ch[i] >> 4]);
                    output.push_back(lut_upper[ch[i] & 0x0F]);
                }

            return output;
        }
        else if constexpr(std::is_integral<T>::value)
        {
            string_t output;

            output.reserve(2 * sizeof(T));

            if(isLittleEndian())
                for(int i = sizeof(T) * 8 - 4; i >= 0; i -= 4)
                {
                    const uint8_t ch = (value >> i) & 0x0F;
                    output.push_back(lut_upper[ch]);
                }
            else
                for(int i = 0; i < int(sizeof(T) * 8 - 4); i += 4)
                {
                    const uint8_t ch = (value >> i) & 0x0F;
                    output.push_back(lut_upper[ch]);
                }

            return output;
        }
        else if constexpr(std::is_same<T, string_t>::value || std::is_same<T, vector_t>::value)
        {
            const size_t len = value.length();
            string_t output;

            output.reserve(2 * len);

            for(size_t i = 0; i < len; ++i)
            {
                const uint8_t c = value[i];
                output.push_back(lut_upper[c >> 4]);
                output.push_back(lut_upper[c & 0x0F]);
            }

            return output;
        }
        else
        {
            throw iolink::utils::exception_argument(__func__, "Unsupported input type");
        }
    }

    constexpr auto operator "" _ui64(unsigned long long int integer)
    {
        return static_cast<uint64_t>(integer);
    }

    constexpr auto operator "" _ui32(unsigned long long int integer)
    {
        if(integer > std::numeric_limits<uint32_t>::max())
            throw exception_logic(__func__, "Integer can not be safely converted to unsigned equivalent");

        return static_cast<uint32_t>(integer);
    }

    constexpr auto operator "" _ui16(unsigned long long int integer)
    {
        if(integer > std::numeric_limits<uint16_t>::max())
            throw exception_logic(__func__, "Integer can not be safely converted to unsigned equivalent");

        return static_cast<uint16_t>(integer);
    }

    constexpr auto operator "" _ui8(unsigned long long int integer)
    {
        if(integer > std::numeric_limits<uint8_t>::max())
            throw exception_logic(__func__, "Integer can not be safely converted to unsigned equivalent");

        return static_cast<uint8_t>(integer);
    }
}

#endif // CONVERT_H
