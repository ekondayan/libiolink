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

#ifndef AL1352_IOTSETUP_H
#define AL1352_IOTSETUP_H

#include "network.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class IOTSetup: private BaseElement
    {
        public:
            explicit IOTSetup(BaseElement *parent):
                BaseElement{"iotsetup", parent}
            {}

            AccessReadWrite<DataTypeString> smobip{"smobip", this, 7u, 15u};
            AccessReadWrite<DataTypeInt>    smobport{"smobport", this, 0, 65535};
            AccessReadWrite<DataTypeInt, DataUnit> smobinterval{"smobinterval", this, 500, 2147483647, DataTypeInt::List{0}};

            Network network{this};
    };
}

#endif // AL1352_IOTSETUP_H
