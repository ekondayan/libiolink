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

#ifndef AL1352_PROCESSDATAMASTER_H
#define AL1352_PROCESSDATAMASTER_H

#include "libiolink/src/iot/dataaccess.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class ProcessDataMaster: private BaseElement
    {
        public:
            explicit ProcessDataMaster(BaseElement *parent):
                BaseElement ("processdatamaster", parent){};

            AccessRead<DataTypeInt, DataUnit> temperature{"temperature", this, -30,80};
            AccessRead<DataTypeInt, DataUnit> voltage{"voltage", this, 0,40000};
            AccessRead<DataTypeInt, DataUnit> current{"current", this, 0,40000};
            AccessRead<DataTypeEnum>          supervisionstatus{"supervisionstatus", this, DataTypeEnum::Map{
                    {0,"OK"},
                    {1,"short circuit"},
                    {2,"overload"},
                    {8,"undervoltage"}}};
    };

}

#endif // AL1352_PROCESSDATAMASTER_H
