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

#ifndef IODD_DATATYPEBOOLEAN_H
#define IODD_DATATYPEBOOLEAN_H

#include "../inc.h"
#include "../exception.h"

namespace iolink::iodd
{
    class BooleanT
    {
        public:
            using type_t = bool;
            using iodd_type_t = bool;

            BooleanT(const BooleanT&) =delete;
            BooleanT(BooleanT&&) =delete;
            BooleanT& operator =(const BooleanT&) =delete;
            BooleanT& operator =(BooleanT&&) =delete;
            ~BooleanT() = default;

            explicit BooleanT(map_t<type_t> single_values = map_t<type_t>{}):
                m_single_values{single_values}
            {
            }

            bool isValid(const type_t value) const
            {
                return true;
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
                type_t value = uint8_t(vector.back()) & 0x01;

                uint8_t bits_transfer = 0;  // Bits from the previous byte
                for(auto& el: vector)
                {
                    uint8_t bits_poped = el & 0x01;  // Save the bit to be shifted right
                    el = (el >> 1) | (bits_transfer << 7);  // Shift right the byte and apply the bit shifted from the previous byte
                    bits_transfer = bits_poped;  // Save the right shifted bit
                }
                return value;
            }

            string_t description(type_t value) const
            {
                if(m_single_values.find(value) == end(m_single_values))
                    throw iolink::utils::exception_argument(__func__, "Not a valid value");

                return m_single_values.at(value);
            }

        private:
            const map_t<type_t> m_single_values;
    };
}

#endif // IODD_DATATYPEBOOLEAN_H
