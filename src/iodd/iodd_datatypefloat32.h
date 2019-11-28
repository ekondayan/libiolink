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

#ifndef IODD_DATATYPEFLOAT32_H
#define IODD_DATATYPEFLOAT32_H

#include "libiolink/src/utils/inc.h"
#include "libiolink/src/utils/exception.h"

// TODO: implement packToVector()

namespace iolink::iodd
{
    class Float32T
    {
        public:
            using type_t = float;
            using iodd_type_t = float;

            Float32T(const Float32T&) =delete;
            Float32T(Float32T&&) =delete;
            Float32T& operator =(const Float32T&) =delete;
            Float32T& operator =(Float32T&&) =delete;
            ~Float32T() = default;

            explicit Float32T(type_t min = std::numeric_limits<type_t>::min(), type_t max = std::numeric_limits<type_t>::max(), map_t<type_t> single_values = map_t<type_t>{}):
                m_min{min},
                m_max{max},
                m_single_values{single_values}
            {
                if(min > max)
                    throw iolink::utils::exception_argument(__func__, "Minimum value can't be higher than the max value");

            }

            explicit Float32T(map_t<type_t> single_values):
                m_single_values{single_values}
            {

            }

            bool isValid(const type_t value) const
            {
                return (((m_min <= value) && (value <= m_max)) || (m_single_values.find(value) != end(m_single_values)));
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
                uint32_t value = 0;
                for(std::size_t i = 0; i < 4; ++i)
                {
                    value |= uint32_t(vector.back()) << (i * 8);
                    vector.pop_back();
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
            const type_t m_min = std::numeric_limits<type_t>::min();
            const type_t m_max = std::numeric_limits<type_t>::max();
            const map_t<type_t> m_single_values;
    };
}

#endif // IODD_DATATYPEFLOAT32_H
