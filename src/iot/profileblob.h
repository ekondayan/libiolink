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

#ifndef PROFILEBLOB_H
#define PROFILEBLOB_H

#include "dataaccess.h"

namespace iolink::iot
{
    class ProfileBlob: public BaseElement
    {
        protected:
        public:
            explicit ProfileBlob(const string_t& id, BaseElement* parent):
                BaseElement{id, parent}
            {
                if(!m_parent)
                    throw iolink::utils::exception_argument(__func__, "Parent for this element can not be empty");
            }

            json_t setBlobData() const{ return json_t{};} // FIXME: to complete function body
            json_t getBlobData(uint32_t pos, uint32_t len) const{return requestPost("/getblobdata",R"("pos":)"+std::to_string(pos)+R"(,"length":)"+std::to_string(len));}
            json_t startStreamSet(uint64_t size) const{return requestPost("/start_stream_set",R"("size":)"+std::to_string(size));}
            json_t streamSet(const string_t &data) const{{return requestPost("/stream_set",R"("value":)"+data);}}
            json_t clear() const{return requestGet("/clear");}
            json_t getCRC() const{return requestGet("/getcrc");}
            json_t getMD5() const{return requestGet("/getmd5");}
            json_t getData() const{ return json_t{};} // FIXME: to complete function body
            json_t setData() const{ return json_t{};} // FIXME: to complete function body

            AccessRead<DataTypeInt> size{"size", this};
            AccessRead<DataTypeInt> chunksize{"chunksize", this};
    };
}

#endif // PROFILEBLOB_H
