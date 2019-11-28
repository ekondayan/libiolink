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

#ifndef PROFILEDEVICEINFO_H
#define PROFILEDEVICEINFO_H

#include "dataaccess.h"

namespace iolink::iot
{
    class ProfileDeviceInfo: public BaseElement
    {
        protected:
        public:
            explicit ProfileDeviceInfo(const string_t &id, BaseElement* parent):
                BaseElement{id, parent}
            {
                if(!m_parent)
                    throw iolink::utils::exception_argument(__func__, "Parent for this element can not be empty");
            };

            AccessRead<DataTypeString> devicename{"devicename", this};
            AccessRead<DataTypeString> devicefamiliy{"devicefamily", this};
            AccessRead<DataTypeString> devicevariant{"devicevariant", this};
            AccessRead<DataTypeString> devicesymbol{"devicesymbol", this};
            AccessRead<DataTypeString> deviceicon{"deviceicon", this};
            AccessRead<DataTypeString> serialnumber{"serialnumber", this};
            AccessRead<DataTypeString> productid{"productid", this};
            AccessRead<DataTypeString> productname{"productname", this};
            AccessRead<DataTypeString> productcode{"productcode", this};
            AccessRead<DataTypeString> producttext{"producttext", this};
            AccessRead<DataTypeString> ordernumber{"ordernumber", this};
            AccessRead<DataTypeString> productiondate{"productiondate", this};
            AccessRead<DataTypeString> productioncode{"productioncode", this};
            AccessRead<DataTypeString> hwrevision{"hwrevision", this};
            AccessRead<DataTypeString> swrevision{"swrevision", this};
            AccessRead<DataTypeString> bootloaderrevision{"bootloaderrevision", this};
            AccessRead<DataTypeString> vendor{"vendor", this};
            AccessRead<DataTypeString> vendortext{"vendortext", this};
            AccessRead<DataTypeString> vendorurl{"vendorurl", this};
            AccessRead<DataTypeString> vendorlogo{"vendorlogo", this};
            AccessRead<DataTypeString> productwebsite{"productwebsite", this};
            AccessRead<DataTypeString> supportcontact{"supportcontact", this};
            AccessRead<DataTypeString> icon{"icon", this};
            AccessRead<DataTypeString> image{"image", this};
            AccessRead<DataTypeString> standards{"standards", this};
    };
}

#endif // PROFILEDEVICEINFO_H
