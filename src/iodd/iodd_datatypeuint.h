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

#ifndef IODD_DATATYPEUINT_H
#define IODD_DATATYPEUINT_H

#include "../utils/misc.h"
#include "../utils/inc.h"
#include "../utils/exception.h"

// TODO: implement packToVector()

namespace iolink::iodd
{
    template <uint8_t bit_length,
              typename Type = typename std::conditional_t<(bit_length >= 2 && bit_length <= 8), uint8_t,
                                                        typename std::conditional_t<(bit_length >= 9 && bit_length <= 16), uint16_t,
                                                                                  typename std::conditional_t<(bit_length >= 17 && bit_length <= 32), uint32_t,
                                                                                                            typename std::enable_if<(bit_length >= 33 && bit_length <= 64), uint64_t>
                                                                                                            >
                                                                                  >
                                                        >>
    class UIntegerT
    {
        public:
            using type_t = Type;
            using iodd_type_t = Type;

            UIntegerT(const UIntegerT&) =delete;
            UIntegerT(UIntegerT&&) =delete;
            UIntegerT& operator =(const UIntegerT&) =delete;
            UIntegerT& operator =(UIntegerT&&) =delete;
            ~UIntegerT() = default;

            explicit UIntegerT(type_t min, type_t max, map_t<type_t> single_values = map_t<type_t>{}):
                m_min{min},
                m_single_values{single_values}
            {
                // Calculate the absolute minimum and maximum values
                if constexpr(bit_length == (sizeof (type_t) * 8))
                {
                    m_min = std::numeric_limits<type_t>::min();
                    m_max = std::numeric_limits<type_t>::max();
                }
                else
                {
                    for(int i = 0; i < bit_length; ++i)
                        m_max = (m_max << 1) | 0x01;

                    if(max < m_max )
                        m_max = max;
                }

                if(min > max)
                    throw iolink::utils::exception_argument(__func__, "Min value can't be higher than the Мax value");

                m_min = min;
                m_max = max;
            }

            explicit UIntegerT(type_t max, map_t<type_t> single_values):
                UIntegerT{std::numeric_limits<type_t>::min(), max, single_values}
            {
            }

            explicit UIntegerT():
                UIntegerT{std::numeric_limits<type_t>::min(), std::numeric_limits<type_t>::max()}
            {
            }


            // Only single values allowed. Min and Max are the same, which disables the range
            explicit UIntegerT(map_t<type_t> single_values):
                UIntegerT{0, 0, single_values}
            {
            }

            bool isValid(const type_t value) const
            {
                return (((m_min != m_max) && (m_min <= value) && (value <= m_max)) || (m_single_values.find(value) != end(m_single_values)));
            }

            const type_t& toType(const iodd_type_t& iodd_value) const
            {
                return iodd_value;
            }

            const iodd_type_t& toIoddType(const type_t& value) const
            {
                return value;
            }

            type_t unpackFromVector(vector_t& vector) const
            {
                constexpr auto bytes = bit_length / 8;
                constexpr auto bits  = bit_length % 8;

                type_t value = 0;
                for(std::size_t i = 0; i < bytes; ++i)
                {
                    value |= type_t(vector.back()) << (i * 8);
                    vector.pop_back();
                }

                if(bits)
                {
                    auto mask = utils::createBitMask<uint8_t, bits>();

                    value |= type_t(vector.back() & mask) << (bytes * 8);

                    uint8_t bits_transfer = 0;  // Bits from the previous byte
                    for(auto& el: vector)
                    {
                        uint8_t bits_poped = el & mask ;  // Bits to be shifted right
                        el = (el >> bits) | (bits_transfer << ( 8 - bits));  // Shift right the bits and apply the bits shifted from the previous byte
                        bits_transfer = bits_poped;  // Save the right shifted bits
                    }
                }
                return value;
            }

            void packToVector(vector_t& vector, const type_t& value) const
            {
            }

            string_t description(type_t value) const
            {
                if(m_single_values.find(value) == end(m_single_values))
                    throw iolink::utils::exception_argument(__func__, "Not a valid value");

                return m_single_values.at(value);
            }

        private:
            type_t m_min  = 0;
            type_t m_max  = 0;
            const map_t<type_t> m_single_values;
    };
}

#endif // IODD_DATATYPEUINT_H
