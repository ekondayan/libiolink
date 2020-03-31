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
//    enum ProfileNetworkMember: uint64_t{MACADDRESS       = 1 << 0,
//                                        IPADDRESS        = 1 << 1,
//                                        IPV6ADDRESS      = 1 << 2,
//                                        SUBNETMASK       = 1 << 3,
//                                        IPDEFAULTGATEWAY = 1 << 4,
//                                        IPVERSION        = 1 << 5,
//                                        DHCP             = 1 << 6,
//                                        HOSTNAME         = 1 << 7,
//                                        AUTONEGOTIATION  = 1 << 8,
//                                        PORTSPEED        = 1 << 9};

//    template<uint64_t mask>
//    class ProfileNetwork: public BaseElement
//    {
//        public:

//            explicit ProfileNetwork(const string_t &id, BaseElement *parent):
//                BaseElement{id, parent}
//            {
//                if(!m_parent)
//                    throw iolink::utils::exception_argument(__func__, "Parent for this element is required");
//            };

//            json_t enableNetwork() const{return requestGet("/enablenetwork");};
//            json_t disableNetwork() const{return requestGet("/disablenetwork");};

//            template<typename T = std::enable_if_t<mask & MACADDRESS, AccessRead<DataTypeString>>>
//            T macaddress2 {"macaddress"};



//            std::enable_if_t<mask & MACADDRESS, AccessRead<DataTypeString>>             macaddress      {"macaddress", this};
//            std::enable_if_t<mask & IPADDRESS, AccessReadWrite<DataTypeString>>         ipaddress       {"ipaddress", this, 7u, 15u};
//            std::enable_if_t<mask & IPV6ADDRESS, AccessReadWrite<DataTypeString>>       ipv6address     {"ipv6address", this};
//            std::enable_if_t<mask & SUBNETMASK, AccessReadWrite<DataTypeString>>        subnetmask      {"subnetmask", this, 7u, 15u};
//            std::enable_if_t<mask & IPDEFAULTGATEWAY , AccessReadWrite<DataTypeString>> ipdefaultgateway{"ipdefaultgateway", this, 7u, 15u};
//            std::enable_if_t<mask & DHCP, AccessReadWrite<DataTypeEnum>>                dhcp            {"dhcp", this, DataTypeEnum::Map{
//                    {0, "Static IP"},
//                    {1, "DHCP"}}};
//            std::enable_if_t<mask & IPVERSION, AccessReadWrite<DataTypeString>>         ipversion       {"ipversion", this};
//            std::enable_if_t<mask & HOSTNAME, AccessReadWrite<DataTypeString>>          hostname        {"hostname", this};
//            std::enable_if_t<mask & AUTONEGOTIATION, AccessReadWrite<DataTypeString>>   autonegotiation {"autonegotiation", this};
//            std::enable_if_t<mask & PORTSPEED, AccessReadWrite<DataTypeString>>         portspeed       {"portspeed", this};
//    };

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
