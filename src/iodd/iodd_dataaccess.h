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

#ifndef IODD_DATAACCESS_H
#define IODD_DATAACCESS_H

#include "../inc.h"
#include "iodd_basedriver.h"

namespace iolink::iodd
{
    class BaseAccess
    {
        protected:
            explicit BaseAccess(BaseDriver* const driver):
                m_driver{driver}
            {}

            BaseDriver* const m_driver;
    };

    template<uint32_t index, uint32_t sub_index, typename IODDType>
    class Read: protected BaseAccess, public IODDType
    {
        public:

            template<typename ...CArgs>
            explicit Read(BaseDriver* const driver, CArgs&& ... cargs):
                BaseAccess{driver},
                IODDType{std::forward<CArgs>(cargs) ...}
            {}

            typename IODDType::type_t read() const
            {
                return this->toType(BaseAccess::m_driver->getIOLinkDevice()->template read<typename IODDType::iodd_type_t>(index, sub_index));
            }
    };

    template<uint32_t index, uint32_t sub_index, typename IODDType>
    class Write: protected BaseAccess, public IODDType
    {
        public:

            template<typename ...CArgs>
            explicit Write(BaseDriver* const driver, CArgs&& ... cargs):
                BaseAccess{driver},
                IODDType{std::forward<CArgs>(cargs) ...}
            {}

            void write(typename IODDType::type_t value) const
            {
                if(!this->isValid(value))
                    throw iolink::utils::exception_iodd(__func__, iolink::utils::exception_iodd::ErrorCodeType::ERROR_INVALID_VALUE);

                BaseAccess::m_driver->getIOLinkDevice()->template write<typename IODDType::iodd_type_t>(this->toIoddType(value), index, sub_index);
            }
    };

    // INFO: може ли този клас да унаследява Read и Write?
    template<uint32_t index, uint32_t sub_index, typename IODDType>
    class ReadWrite: protected BaseAccess, public IODDType
    {
        public:

            template<typename ...CArgs>
            explicit ReadWrite(BaseDriver* const driver, CArgs&& ... cargs):
                BaseAccess{driver},
                IODDType{std::forward<CArgs>(cargs) ...}
            {
            }

            typename IODDType::type_t read() const
            {
                return this->toType(BaseAccess::m_driver->getIOLinkDevice()->template read<typename IODDType::iodd_type_t>(index, sub_index));
            }

            void write(typename IODDType::type_t value) const
            {
                if(!this->isValid(value))
                    throw iolink::utils::exception_iodd(__func__, iolink::utils::exception_iodd::ErrorCodeType::ERROR_INVALID_VALUE);

                BaseAccess::m_driver->getIOLinkDevice()->template write<typename IODDType::iodd_type_t>(this->toIoddType(value), index, sub_index);
            }
    };
}

#endif // IODD_DATAACCESS_H
