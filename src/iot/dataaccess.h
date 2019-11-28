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

#ifndef DATAACCESS_H
#define DATAACCESS_H

#include "datatype.h"

namespace iolink::iot
{
    template<typename DataType, typename ...Args>
    class AccessRead: public DataType, public Args...
    {
        public:
            template<typename ...CArgs>
            AccessRead(const string_t& id, BaseElement* parent, CArgs&& ... cargs):
                DataType{id, parent, std::forward<CArgs>(cargs) ...},
                Args{id, parent}...
            {}

            string_t address() const override
            {
                return DataType::address();
            }

            string_t id() const override
            {
                return DataType::id();
            }

            json_t getDataJson() const
            {
                return DataType::requestGet("/getdata");
            }

            typename DataType::type_t getData() const
            {
                return DataType::requestGet("/getdata")["data"]["value"];
            }

            json_t setData(typename DataType::type_t value) const
            {
                if(!this->isValid(value))
                    throw iolink::utils::exception_argument(__func__, "Trying to set a invalid value");

                return DataType::requestPost("/setdata", R"("newvalue":")" + std::to_string(value) + R"(")");
            }
    };

    template<typename DataType, typename ...Args>
    class AccessReadWrite: public DataType, public Args...
    {
        public:
            template<typename ...CArgs>
            AccessReadWrite(const string_t& id, BaseElement* parent, CArgs&& ... cargs):
                DataType{id, parent, std::forward<CArgs>(cargs) ...},
                Args{id, parent}...
            {}

            string_t address() const override
            {
                return DataType::address();
            }

            string_t id() const override
            {
                return DataType::id();
            }

            json_t getDataJson() const
            {
                return DataType::requestGet("/getdata");
            }

            typename DataType::type_t getData() const
            {
                return DataType::requestGet("/getdata")["data"]["value"];
            }

            json_t setData(typename DataType::type_t value) const
            {
                if(!this->isValid(value))
                    throw iolink::utils::exception_argument(__func__, "Trying to set an invalid value");

                return DataType::requestPost("/setdata", R"("newvalue":")" + std::to_string(value) + R"(")");
            }
    };










//    template<typename DataType>
//    class AccessRead: public DataType
//    {
//        public:
//            template<typename ...CArgs>
//            AccessRead(const string_t& id, typename DataType::base_t* parent, CArgs&& ... cargs):
//                DataType{id, parent, std::forward<CArgs>(cargs) ...}
//            {}

//            json_t getDataJson() const
//            {
//                return DataType::requestGet("/getdata");
//            }

//            typename DataType::type_t getData() const
//            {
//                return DataType::requestGet("/getdata")["data"]["value"];
//            }

//            json_t setData(typename DataType::type_t value) const
//            {
//                if(!this->isValid(value))
//                    throw iolink::utils::exception_argument(__func__, "Trying to set a invalid value");

//                return DataType::requestPost("/setdata", R"("newvalue":")" + std::to_string(value) + R"(")");
//            }
//    };

//    template<typename DataType>
//    class AccessReadWrite: public DataType
//    {
//        public:
//            template<typename ...CArgs>
//            AccessReadWrite(const string_t& id, typename DataType::base_t* parent, CArgs&& ... cargs):
//                DataType{id, parent, std::forward<CArgs>(cargs) ...}
//            {}

//            json_t getDataJson() const
//            {
//                return DataType::requestGet("/getdata");
//            }

//            typename DataType::type_t getData() const
//            {
//                return DataType::requestGet("/getdata")["data"]["value"];
//            }

//            json_t setData(typename DataType::type_t value) const
//            {
//                if(!this->isValid(value))
//                    throw iolink::utils::exception_argument(__func__, "Trying to set an invalid value");

//                return DataType::requestPost("/setdata", R"("newvalue":")" + std::to_string(value) + R"(")");
//            }
//    };

}

#endif // DATAACCESS_H
