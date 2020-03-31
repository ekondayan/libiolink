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

#ifndef AL1352_FIRMWARE_H
#define AL1352_FIRMWARE_H

#include "../../../iot/profileblob.h"
#include "../../../iot/profilesoftware.h"
#include "../../../iot/profileuploadablesoftware.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class AL1352_FirmwareBlob: public ProfileBlob
    {
        public:
            AL1352_FirmwareBlob(const string_t &id, BaseElement *parent):
                ProfileBlob (id, parent){};

            json_t setBlobData() =delete;
            json_t getBlobData() =delete;
            json_t clear() =delete;
            json_t getCRC() =delete;
            json_t getMD5() =delete;
            json_t getData() =delete;
            json_t setData() =delete;

            AccessRead<DataTypeInt> maxsize{"maxsize", this};

    };

    class AL1352_UploadableSoftware: public ProfileUploadableSoftware
    {
        public:
            AL1352_UploadableSoftware(const string_t &id, BaseElement *parent = nullptr):
                ProfileUploadableSoftware(id, parent)
            {}

            json_t preInstall() =delete;
            json_t postInstall() =delete;
            json_t abortInstall() =delete;

            AccessRead<DataTypeString> lastinstall{"lastinstall", this};
            AccessRead<DataTypeString> installhistory{"installhistory", this};
            AccessRead<DataTypeString> installstatus{"installstatus", this};

            AL1352_FirmwareBlob container{"container", this};
    };

    class Firmware: public ProfileSoftware, public AL1352_UploadableSoftware
    {
        public:
            explicit Firmware(BaseElement *parent):
                ProfileSoftware{"firmware", parent},
                AL1352_UploadableSoftware{"firmware", parent}
            {}

            // Services from ProfileSoftware
            json_t status() =delete;
            json_t diag() =delete;

            json_t factoryReset(){return ProfileSoftware::requestGet("/factoryreset");}
            json_t signal(){return ProfileSoftware::requestGet("/signal");}
            json_t reboot(){return ProfileSoftware::requestGet("/reboot");}

        private:
            using ProfileUploadableSoftware::lastinstall;
            using ProfileUploadableSoftware::installhistory;
            using ProfileUploadableSoftware::installstatus;

    };
}

#endif // AL1352_FIRMWARE_H
