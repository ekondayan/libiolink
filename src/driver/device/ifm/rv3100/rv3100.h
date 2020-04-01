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

#ifndef RV3100_H
#define RV3100_H

#include "../../../../iodd/iodd_dataaccess.h"
#include "../../../../iodd/iodd_datatypestring.h"
#include "../../../../iodd/iodd_datatypeuint.h"
#include "../../../../iodd/iodd_datatypeint.h"

namespace iolink::driver
{
    using namespace utils;
    using namespace iodd;

    class RV3100: public BaseDriver
    {
        public:

            struct ProcessData
            {
                    const int16_t distance = 0;
                    const int16_t reflectivity = 0;
                    const uint8_t status = 0;
                    const bool    out1 = false;
                    const bool    out2 = false;
            };

            RV3100() =delete;
            RV3100(const RV3100&) =delete;
            RV3100(RV3100&&) =delete;
            RV3100& operator= (const RV3100&) =delete;
            RV3100& operator= (RV3100&&) =delete;

            ~RV3100() =default;

            explicit RV3100(const std::weak_ptr<iolink::iot::ProfileIOLinkDevice>& iolink_device):
                BaseDriver(iolink_device, 310, 499)
            {}

            ProcessData processData() const
            {
                auto data = iolink::utils::hexDecode<iolink::vector_t>(getIOLinkDevice()->pdin.getData());

                ProcessData reading = {
                    .distance     = int16_t((int16_t(data[0]) << 8) | int16_t(data[1])),
                    .reflectivity = int16_t((int16_t(data[4]) << 8) | int16_t(data[5])),
                    .status       = uint8_t(data[7] >> 4),
                    .out1         = (data[7] & 0x01)?true:false,
                    .out2         = (data[7] & 0x02)?true:false
                };

                return reading;
            }

            Read<16, 0, StringT> vendor_name{this, 19_ui32};
            Read<17, 0, StringT> vendor_text{this, 11_ui32};
            Read<18, 0, StringT> product_name{this, 6_ui32};
            Read<19, 0, StringT> product_id{this, 6_ui32};
            Read<20, 0, StringT> product_text{this, 12_ui32};
            Read<21, 0, StringT> serial_number{this, 12_ui32};
            Read<22, 0, StringT> firmware_version{this, 8_ui32};
            Read<23, 0, StringT> software_version{this, 8_ui32};
            ReadWrite<24, 0, StringT> application_tag{this, 32_ui32};
    };
}

#endif // RV3100_H
