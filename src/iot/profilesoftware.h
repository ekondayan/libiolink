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

#ifndef PROFILESOFTWARE_H
#define PROFILESOFTWARE_H

#include "dataaccess.h"

namespace iolink::iot
{
    class ProfileSoftware: public BaseElement
    {
        protected:
        public:
            explicit ProfileSoftware(const string_t &id, BaseElement *parent):
                BaseElement{id, parent}
            {
                if(!m_parent)
                    throw iolink::utils::exception_argument(__func__, "Parent for this element can not be empty");
            }

            json_t status() const{return requestGet("/status");}
            json_t diag() const{return requestGet("/diag");}

            AccessRead<DataTypeString> version{"version", this};
            AccessRead<DataTypeString> type{"type", this};
    };
}

#endif // PROFILESOFTWARE_H
