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

#ifndef PROFILEIOLINKMASTER_H
#define PROFILEIOLINKMASTER_H

#include "profileiolinkdevice.h"

namespace iolink::iot
{
    class ProfileIOLinkMaster: public BaseElement
    {
        public:
            explicit ProfileIOLinkMaster(const string_t &id, BaseElement *parent):
                BaseElement{id, parent}
            {
                if(!m_parent)
                    throw iolink::utils::exception_argument(__func__, "Parent for this element can not be empty");
            };

            AccessReadWrite<DataTypeEnum> mode{"mode", this, DataTypeEnum::Map{
                    {0, "Disabled"},
                    {1, "DI"},
                    {2, "DO"},
                    {3, "IO-Link"}}};
            AccessRead<DataTypeEnum> comspeed{"comspeed", this, DataTypeEnum::Map{
                    {0, "COM1 (4.8 kBaud)"},
                    {1, "COM2 (38.4 kBaud)"},
                    {2, "COM3 (230.4 kBaud)"}}};
            AccessRead<DataTypeInt, DataUnit> mastercycletime_actual{"mastercycletime_actual", this, 0, 132800};
            AccessReadWrite<DataTypeInt, DataUnit> mastercycletime_preset{"mastercycletime_preset", this, 0, 132800};
            AccessReadWrite<DataTypeEnum> validation_datastorage_mode{"validation_datastorage_mode", this, DataTypeEnum::Map{
                    {0,"No check and clear"},
                    {1,"Type compatible V1.0 device"},
                    {2,"Type compatible V1.1 device"},
                    {3,"Type compatible V1.1 device with Backup + Restore"},
                    {4,"Type compatible V1.1 device with Restore"}}};
            AccessReadWrite<DataTypeInt>    validation_vendorid{"validation_vendorid", this, 0, 65535};
            AccessReadWrite<DataTypeInt>    validation_deviceid{"validation_deviceid", this, 0, 16777215};
            AccessReadWrite<DataTypeString> additionalpins_in{"additionalpins_in", this};
            AccessReadWrite<DataTypeString> additionalpins_out{"additionalpins_out", this};
            AccessReadWrite<DataTypeString, DataEvent> portevent{"portevent", this};

            ProfileIOLinkDevice iolinkdevice{this};
    };
}

#endif // PROFILEIOLINKMASTER_H
