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

#ifndef PROFILETIMER_H
#define PROFILETIMER_H

#include "dataaccess.h"

namespace iolink::iot
{

    class ProfileTimer: public BaseElement
    {
        protected:
        public:
            explicit ProfileTimer(const string_t &id, BaseElement *parent):
                BaseElement{id, parent}
            {
                if(!m_parent)
                    throw iolink::utils::exception_argument(__func__, "Parent for this element can not be empty");
            }

            json_t start() const{return requestGet("/start");}
            json_t stop() const{return requestGet("/stop");}

            AccessReadWrite<DataTypeInt, DataEvent> counter{"counter", this};
            AccessReadWrite<DataTypeInt, DataUnit>  interval{"interval", this, 500,2147483647, DataTypeInt::List{0}};
    };
}

#endif // PROFILETIMER_H
