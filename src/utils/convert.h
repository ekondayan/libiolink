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

#include "inc.h"
#include "misc.h"
#include "exception.h"

namespace iolink::utils
{
    template <typename T>
    inline T decodeFromHexString(const string_t &str)
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
                for (int i = 7; i >= 1; i -= 2)
                {
                    *decoded_ptr |= (hexCharToInt(str[i - 1]) << 4) | hexCharToInt(str[i]);
                    if(i > 1)
                        *decoded_ptr <<= 8;
                }
            else
                for (int i = 0; i < 8; i += 2)
                {
                    *decoded_ptr |= (hexCharToInt(str[i]) << 4) | hexCharToInt(str[i + 1]);
                    if( i < 6 )
                        *decoded_ptr <<= 8;
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
                for (size_t i = 0; i < len; i += 2)
                {
                    output |= (hexCharToInt(str[i]) << 4) | hexCharToInt(str[i + 1]);
                    if( (i+2) < len )
                        output <<= 8;
                }
            else
                for (size_t i = len; i >= 2; i += 2)
                {
                    output |= (hexCharToInt(str[i - 1]) << 4) | hexCharToInt(str[i]);
                    if( (i+2) > 2 )
                        output <<= 8;
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
    inline string_t encodeToHexString(T value)
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
                for(int i = 0; i < 4; ++i)
                {
                    output.push_back(lut_upper[ch[i] >> 4]);
                    output.push_back(lut_upper[ch[i] & 0x0F]);
                }
            else
                for(int i = 3; i >= 0; --i)
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


    inline string_t encodeToBase64(const vector_t& input)
    {
        static const char* const lut_base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        constexpr char pad_char_base64 = '=';

        string_t output;
        output.reserve(((input.size() / 3) + (input.size() % 3 > 0)) * 4);

        uint32_t tmp;
        auto it = input.begin();

        for(size_t i = 0; i < input.size() / 3; ++i)
        {
            tmp  = (*it++) << 16 | (*it++) << 8 | (*it++);

            output.append(1, lut_base64[(tmp >> 18) & 0x0000003F]);
            output.append(1, lut_base64[(tmp >> 12) & 0x0000003F]);
            output.append(1, lut_base64[(tmp >> 6)  & 0x0000003F]);
            output.append(1, lut_base64[ tmp        & 0x0000003F]);
        }

        switch(input.size() % 3)
        {
            case 1:
                tmp = (*it++) << 16;
                output.append(1, lut_base64[(tmp >> 18) & 0x0000003F]);
                output.append(1, lut_base64[(tmp >> 12) & 0x0000003F]);
                output.append(2, pad_char_base64);
                break;
            case 2:
                tmp  = (*it++) << 16 | (*it++) << 8;
                output.append(1, lut_base64[(tmp >> 18) & 0x0000003F]);
                output.append(1, lut_base64[(tmp >> 12) & 0x0000003F]);
                output.append(1, lut_base64[(tmp >> 6)  & 0x0000003F]);
                output.append(1, pad_char_base64);
                break;
        }

        return output;
    }

    inline string_t encodeToBase64(const string_t& input)
    {
        return encodeToBase64(vector_t{input.begin(), input.end()});
    }

    //    std::vector<byte> decode(const std::string& input)
    //            {
    //                    if(input.length() % 4)
    //                            throw std::runtime_error("Invalid base64 length!");

    //                    std::size_t padding{};

    //                    if(input.length())
    //                    {
    //                            if(input[input.length() - 1] == kPadCharacter) padding++;
    //                            if(input[input.length() - 2] == kPadCharacter) padding++;
    //                    }

    //                    std::vector<byte> decoded;
    //                    decoded.reserve(((input.length() / 4) * 3) - padding);

    //                    std::uint32_t temp{};
    //                    auto it = input.begin();

    //                    while(it < input.end())
    //                    {
    //                            for(std::size_t i = 0; i < 4; ++i)
    //                            {
    //                                    temp <<= 6;
    //                                    if     (*it >= 0x41 && *it <= 0x5A) temp |= *it - 0x41;
    //                                    else if(*it >= 0x61 && *it <= 0x7A) temp |= *it - 0x47;
    //                                    else if(*it >= 0x30 && *it <= 0x39) temp |= *it + 0x04;
    //                                    else if(*it == 0x2B)                temp |= 0x3E;
    //                                    else if(*it == 0x2F)                temp |= 0x3F;
    //                                    else if(*it == kPadCharacter)
    //                                    {
    //                                            switch(input.end() - it)
    //                                            {
    //                                            case 1:
    //                                                    decoded.push_back((temp >> 16) & 0x000000FF);
    //                                                    decoded.push_back((temp >> 8 ) & 0x000000FF);
    //                                                    return decoded;
    //                                            case 2:
    //                                                    decoded.push_back((temp >> 10) & 0x000000FF);
    //                                                    return decoded;
    //                                            default:
    //                                                    throw std::runtime_error("Invalid padding in base64!");
    //                                            }
    //                                    }
    //                                    else throw std::runtime_error("Invalid character in base64!");

    //                                    ++it;
    //                            }

    //                            decoded.push_back((temp >> 16) & 0x000000FF);
    //                            decoded.push_back((temp >> 8 ) & 0x000000FF);
    //                            decoded.push_back((temp      ) & 0x000000FF);
    //                    }

    //                    return decoded;
    //            }

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
