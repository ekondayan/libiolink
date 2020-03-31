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

#ifndef IODD_DATATYPEARRAY_H
#define IODD_DATATYPEARRAY_H

#include "../utils/inc.h"
#include "../utils/exception.h"

// TODO: implement subindex_access
// TODO: template must not accept itself as template argument of IODDType

namespace iolink::iodd
{

    template<typename IODDType, std::size_t count, bool subindex_access = false>
    class ArrayT
    {
        public:
            using type_t = std::array<typename IODDType::type_t, count>;
            using iodd_type_t = vector_t;

            ArrayT(const ArrayT&) =delete;
            ArrayT(ArrayT&&) =delete;
            ArrayT& operator =(const ArrayT&) =delete;
            ArrayT& operator =(ArrayT&&) =delete;
            ~ArrayT() = default;

            template<typename ...CArgs>
            explicit ArrayT(CArgs&& ... cargs):
                m_element{std::forward<CArgs>(cargs) ...}
            {
            }

            bool isValid(const type_t& value) const
            {
                for(const auto& el: value)
                    if(!m_element.isValid(el))
                        return false;

                return true;
            }

            type_t toType(iodd_type_t iodd_vector) const
            {
                auto array = type_t{};
                for(int i = count - 1; i >= 0; --i)
                        array[i] = m_element.unpackFromVector(iodd_vector);

                return array;
            }

            iodd_type_t toIoddType(type_t array) const
            {
                auto iodd_vector = iodd_type_t{};
                for(int i = count - 1; i >= 0; --i)
                        m_element.packToVector(iodd_vector, array[i]);

                return iodd_vector;
            }

            type_t makeEmpty()
            {
                return type_t{};
            }

        private:
            IODDType m_element;
    };
}

#endif // IODD_DATATYPEARRAY_H
