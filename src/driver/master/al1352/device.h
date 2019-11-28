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

#ifndef AL1352_MASTER_H
#define AL1352_MASTER_H

#include "libiolink/src/utils/exception.h"
#include "libiolink/src/iot/structdevice.h"
#include "deviceinfo.h"
#include "devicetag.h"
#include "firmware.h"
#include "iotsetup.h"
#include "processdatamaster.h"
#include "timer.h"
#include "iolinkmaster.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class Device: public StructDevice
    {
        public:
            explicit Device(std::unique_ptr<InterfaceComm> comm = nullptr):
                StructDevice{"device", std::move(comm)}
            {
                if(deviceinfo.productcode.getData().compare("AL1352") != 0)
                    throw iolink::utils::exception_logic(__func__, "Driver not suitable for the attached device.");
            }

            DeviceInfo  deviceinfo{this};
            DeviceTag   devicetag{this};
            IOTSetup    iotsetup{this};
            Firmware    firmware{this};
            ProcessDataMaster   processdatamaster{this};
            Timer               timer1{"timer[1]", this};
            Timer               timer2{"timer[2]", this};
            IOLinkMaster        iolinkmaster{this};
    };
}

#endif // AL1352_MASTER_H
