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

#ifndef PROFILENETWORK_H
#define PROFILENETWORK_H

#include "dataaccess.h"

namespace iolink::iot
{
    class ProfileNetwork: public BaseElement
    {
        public:
            explicit ProfileNetwork(const string_t &id, BaseElement *parent):
                BaseElement{id, parent}
            {
                if(!m_parent)
                    throw iolink::utils::exception_argument(__func__, "Parent for this element is required");
            };

            json_t enableNetwork() const{return requestGet("/enablenetwork");};
            json_t disableNetwork() const{return requestGet("/disablenetwork");};

            AccessRead<DataTypeString>      macaddress      {"macaddress", this};
            AccessReadWrite<DataTypeString> ipaddress       {"ipaddress", this, 7u, 15u};
            AccessReadWrite<DataTypeString> ipv6address     {"ipv6address", this};
            AccessReadWrite<DataTypeString> subnetmask      {"subnetmask", this, 7u, 15u};
            AccessReadWrite<DataTypeString> ipdefaultgateway{"ipdefaultgateway", this, 7u, 15u};
            AccessReadWrite<DataTypeEnum>   dhcp            {"dhcp", this, DataTypeEnum::Map{
                    {0, "Static IP"},
                    {1, "DHCP"}}};
            AccessReadWrite<DataTypeString> ipversion       {"ipversion", this};
            AccessReadWrite<DataTypeString> hostname        {"hostname", this};
            AccessReadWrite<DataTypeString> autonegotiation {"autonegotiation", this};
            AccessReadWrite<DataTypeString> portspeed       {"portspeed", this};
    };
}

#endif // PROFILENETWORK_H
