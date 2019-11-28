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

#ifndef AL1352_IOLINKMASTER_H
#define AL1352_IOLINKMASTER_H

#include "libiolink/src/iot/profileblob.h"
#include "libiolink/src/iot/profileiolinkmaster.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class AL1352_IOLinkMasterBlob: public ProfileBlob
    {
        public:
            AL1352_IOLinkMasterBlob(const string_t &id, BaseElement *parent):
                ProfileBlob (id, parent){};

            json_t setBlobData() const =delete;
            json_t clear() const =delete;
            json_t getCRC() const =delete;
            json_t getMD5() const =delete;
            json_t getData() const =delete;
            json_t setData() const =delete;

            AccessRead<DataTypeInt> maxsize{"maxsize", this};

    };

    class Port: public ProfileIOLinkMaster
    {
        public:
            Port(const string_t &id, BaseElement *parent):
                ProfileIOLinkMaster(id, parent)
            {}

            json_t validationUseConnectedDevice(){return requestGet("/validation_useconnecteddevice");}

            AccessRead<DataTypeInt, DataUnit> pin2in{"pin2in", this};
            AL1352_IOLinkMasterBlob container{"datastorage", this};

        private:
            using ProfileIOLinkMaster::additionalpins_in;
            using ProfileIOLinkMaster::additionalpins_out;
    };

    class IOLinkMaster: private BaseElement
    {
        public:
            explicit IOLinkMaster(BaseElement *parent):
                BaseElement ("iolinkmaster", parent)
            {};

            Port port1{"port[1]", this};
            Port port2{"port[2]", this};
            Port port3{"port[3]", this};
            Port port4{"port[4]", this};
            Port port5{"port[5]", this};
            Port port6{"port[6]", this};
            Port port7{"port[7]", this};
            Port port8{"port[8]", this};

    };
}

#endif // AL1352_IOLINKMASTER_H
