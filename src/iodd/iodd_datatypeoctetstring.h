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

#ifndef IODD_DATATYPEOCTETSTRING_H
#define IODD_DATATYPEOCTETSTRING_H

#include "libiolink/inc.h"
#include "libiolink/exception.h"

// TODO: implement packToVector() only when the length is fixed

namespace iolink::iodd
{
    class OctetStringT
    {
        public:
            using type_t = vector_t;
            using iodd_type_t = vector_t;

            OctetStringT(const OctetStringT&) =delete;
            OctetStringT(OctetStringT&&) =delete;
            OctetStringT& operator =(const OctetStringT&) =delete;
            OctetStringT& operator =(OctetStringT&&) =delete;
            ~OctetStringT() = default;

            explicit OctetStringT(uint32_t min_len, uint32_t max_len):
                m_min_len{min_len},
                m_max_len{max_len}
            {
                if(min_len > max_len)
                    throw iolink::utils::exception_argument(__func__, "Minimum value can't be higher than the max value");

                if(max_len == 0)
                    throw iolink::utils::exception_argument(__func__, "Zero length string not allowed");
            }

            explicit OctetStringT(uint32_t max_len, bool fixed_len = true):
                OctetStringT{fixed_len ? max_len : std::numeric_limits<uint32_t>::min(), max_len}
            {
            }

            explicit OctetStringT():
                OctetStringT{std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max()}
            {

            }

            bool isValid(const type_t& value) const
            {
                auto len = value.size();
                return (m_min_len <= len && len <= m_max_len);
            }

            bool isValid(type_t&& value) const
            {
                return isValid(value);
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
                if(m_min_len != m_max_len)
                    throw iolink::utils::exception_logic(__func__, "Only fixed value strings can be unpacket from ArrayT");

                type_t value(m_max_len);

                for(std::size_t i = 0; i < m_max_len; ++i)
                {
                    value[i] = vector.back();
                    vector.pop_back();
                }

                return value;
            }

        private:
            const uint32_t m_min_len = std::numeric_limits<uint32_t>::min();
            const uint32_t m_max_len = std::numeric_limits<uint32_t>::max();
    };
}

#endif // IODD_DATATYPEOCTETSTRING_H
