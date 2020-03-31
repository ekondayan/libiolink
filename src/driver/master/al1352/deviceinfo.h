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

#ifndef AL1352_DEVICEINFO_H
#define AL1352_DEVICEINFO_H

#include "../../../iot/profiledeviceinfo.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class DeviceInfo: public ProfileDeviceInfo
    {
        public:
            explicit DeviceInfo(BaseElement *parent):
                ProfileDeviceInfo ("deviceinfo", parent){};

            AccessRead<DataTypeString> extensionrevisions{"extensionrevisions", this};
            AccessRead<DataTypeEnum>   fieldbustype{"fieldbustype", this, DataTypeEnum::Map{
                    {0,"Profinet"},
                    {1,"EtherNet/IP"},
                    {2,"EtherCAT"},
                    {3,"Modbus-TCP"},
                    {4,"Internet of Things"}}};

        private:
            using ProfileDeviceInfo::devicename;
            using ProfileDeviceInfo::devicevariant;
            using ProfileDeviceInfo::devicesymbol;
            using ProfileDeviceInfo::deviceicon;
            using ProfileDeviceInfo::productid;
            using ProfileDeviceInfo::productname;
            using ProfileDeviceInfo::producttext;
            using ProfileDeviceInfo::ordernumber;
            using ProfileDeviceInfo::productiondate;
            using ProfileDeviceInfo::productioncode;
            using ProfileDeviceInfo::vendortext;
            using ProfileDeviceInfo::vendorurl;
            using ProfileDeviceInfo::vendorlogo;
            using ProfileDeviceInfo::productwebsite;
            using ProfileDeviceInfo::supportcontact;
            using ProfileDeviceInfo::icon;
            using ProfileDeviceInfo::image;
            using ProfileDeviceInfo::standards;
    };

}

#endif // AL1352_DEVICEINFO_H
