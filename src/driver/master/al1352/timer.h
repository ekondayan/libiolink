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

#ifndef AL1352_TIMER_H
#define AL1352_TIMER_H

#include "../../../iot/dataaccess.h"
#include "../../../iot/profiletimer.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class Timer: public ProfileTimer
    {
        public:
            explicit Timer(const string_t &id, BaseElement *parent):
                ProfileTimer{id, parent}
            {}

            json_t start() =delete;
            json_t stop() =delete;
    };
}

#endif // AL1352_TIMER_H
