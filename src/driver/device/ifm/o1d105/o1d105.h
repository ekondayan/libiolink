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

#ifndef O1D105_H
#define O1D105_H

#include "../../../../iodd/iodd_basedriver.h"
#include "../../../../iodd/iodd_dataaccess.h"
#include "../../../../iodd/iodd_datatypestring.h"
#include "../../../../iodd/iodd_datatypeuint.h"
#include "../../../../iodd/iodd_datatypeint.h"
#include "../../../../iodd/iodd_datatypetime.h"
#include "../../../../iodd/iodd_datatypearray.h"

namespace iolink::driver
{
    using namespace utils;
    using namespace iodd;

    class O1D105: public BaseDriver
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

            O1D105() =delete;
            O1D105(const O1D105&) =delete;
            O1D105(O1D105&&) =delete;
            O1D105& operator= (const O1D105&) =delete;
            O1D105& operator= (O1D105&&) =delete;

            ~O1D105() =default;

            explicit O1D105(const std::weak_ptr<iolink::iot::ProfileIOLinkDevice>& iolink_device):
                BaseDriver(iolink_device, 310, 806)
            {}

            ProcessData processData() const
            {
                auto data = hexDecode<iolink::vector_t>(getIOLinkDevice()->pdin.getData());

                ProcessData reading = {
                    .distance     = int16_t((int16_t(data[0]) << 8) | int16_t(data[1])),
                    .reflectivity = int16_t((int16_t(data[4]) << 8) | int16_t(data[5])),
                    .status       = uint8_t(data[7] >> 4),
                    .out1         = (data[7] & 0x01)?true:false,
                    .out2         = (data[7] & 0x02)?true:false
                };

                return reading;
            }

            //            std::tuple<int16_t, int16_t, uint8_t, bool, bool> processData() const
            //            {
            //                auto data = hexDecode<iolink::vector_t>(getIOLinkDevice()->pdin.getData());

            //                return {
            //                            int16_t((int16_t(data[0]) << 8) | int16_t(data[1])),
            //                        int16_t((int16_t(data[4]) << 8) | int16_t(data[5])),
            //                        uint8_t(data[7] >> 4),
            //                        (data[7] & 0x01)?true:false,
            //                        (data[7] & 0x02)?true:false};
            //            }

            Write<2, 0, UIntegerT<8>> standard_command{this, iolink::map_t<uint8_t>{ {67, "Teach SP TP1"},
                                                                                     {68, "Teach SP TP2"},
                                                                                     {75, "Teach Custom - Background"},
                                                                                     {130, "Restore Factory Settings"},
                                                                                     {240, "IO-Link 1.1 system test command 240, Event 8DFE appears"},
                                                                                     {241, "IO-Link 1.1 system test command 241, Event 8DFE disappears"},
                                                                                     {242, "IO-Link 1.1 system test command 242, Event 8DFF appears"},
                                                                                     {243, "IO-Link 1.1 system test command 243, Event 8DFF disappears"},
                                                                                   }};
            // 12 device_access_lock
            Read<16, 0, StringT> vendor_name{this, 19_ui32};
            Read<17, 0, StringT> vendor_text{this, 11_ui32};
            Read<18, 0, StringT> product_name{this, 6_ui32};
            Read<19, 0, StringT> product_id{this, 6_ui32};
            Read<20, 0, StringT> product_text{this, 12_ui32};
            Read<21, 0, StringT> serial_number{this, 12_ui32};
            Read<22, 0, StringT> firmware_version{this, 8_ui32};
            Read<23, 0, StringT> software_version{this, 8_ui32};
            ReadWrite<24, 0, StringT> application_tag{this, 32_ui32};
            Read<36, 0, UIntegerT<8>> device_status{this, iolink::map_t<uint8_t>{{0, "Device is OK"},
                                                                                 {1, "Maintenance required"},
                                                                                 {2, "Out of specification"},
                                                                                 {3, "Functional check"},
                                                                                 {4, "Failure"}
                                                                                }};
            Read<37, 0, ArrayT<UIntegerT<8>, 24>> detailed_device_status{this, 10_ui8, 20_ui8};
            ReadWrite<58, 0, UIntegerT<8>> ti_selection{this, iolink::map_t<uint8_t>{{1, "OUT1"},
                                                                                     {2, "OUT2"}
                                                                                    }};
            // 59 ti_result
            ReadWrite<370, 0, UIntegerT<16>> dS1{this, 0_ui16, 5000_ui16};
            ReadWrite<371, 0, UIntegerT<16>> dr1{this, 0_ui16, 5000_ui16};
            ReadWrite<372, 0, UIntegerT<16>> dS2{this, 0_ui16, 5000_ui16};
            ReadWrite<373, 0, UIntegerT<16>> dr2{this, 0_ui16, 5000_ui16};
            ReadWrite<530, 0, UIntegerT<16>> dFo{this, 0_ui16, 5000_ui16};
            Read<541, 0, IntegerT<32>> power_cycles{this, 0, 2147483647};
            Read<542, 0, IntegerT<32>> operating_hours{this, 0, 2147483647};
            // 545 active_events
            Read<546, 0, ArrayT<UIntegerT<32>, 10>> param_config_fault{this};
            ReadWrite<550, 0, UIntegerT<8>> loc{this, iolink::map_t<uint8_t>{ {0, "Loc"},
                                                                              {1, "uLoc"}
                                                                            }};
            ReadWrite<551, 0, UIntegerT<8>> uni{this, iolink::map_t<uint8_t>{ {0, "mm"},
                                                                              {1, "m"},
                                                                              {2, "in"}
                                                                            }};
            ReadWrite<580, 0, UIntegerT<8>> ou1{this, iolink::map_t<uint8_t>{ {3,  "Hno / Hysteresis fct normally open"},
                                                                              {4,  "Hnc / Hysteresis fct normally closed"},
                                                                              {5,  "Fno / Window fct normally open"},
                                                                              {6,  "Fnc / Window fct normally closed"},
                                                                              {16, "Off / Output Off"}
                                                                            }};
            ReadWrite<583, 0, IntegerT<16>> sp1{this, int16_t(200), int16_t(9999)};
            ReadWrite<590, 0, UIntegerT<8>> ou2{this, iolink::map_t<uint8_t>{ {1,  "I / Analog signal 4...20 mA"},
                                                                              {2,  "U / Analog signal 0...10 V"},
                                                                              {3,  "Hno / Hysteresis fct normally open"},
                                                                              {4,  "Hnc / Hysteresis fct normally closed"},
                                                                              {5,  "Fno / Window fct normally open"},
                                                                              {6,  "Fnc / Window fct normally closed"},
                                                                              {16, "Off / Output Off"}
                                                                            }};
            ReadWrite<593, 0, IntegerT<16>> sp2{this, int16_t(200), int16_t(9999)};
            ReadWrite<630, 0, IntegerT<16>> asp{this, int16_t(0), int16_t(9999)};
            ReadWrite<631, 0, IntegerT<16>> aep{this, int16_t(0), int16_t(9999)};
            ReadWrite<800, 0, UIntegerT<8>> dis_u{this, iolink::map_t<uint8_t>{ {0, "d1 / fast"},
                                                                                {1, "d2 / medium"},
                                                                                {2, "d3 / slow"}
                                                                              }};
            ReadWrite<801, 0, UIntegerT<8>> dis_r{this, iolink::map_t<uint8_t>{ {0, "0 deg"},
                                                                                {2, "180 deg"}
                                                                              }};
            ReadWrite<802, 0, UIntegerT<8>> dis_b{this, iolink::map_t<uint8_t>{ {0, "OFF"},
                                                                                {1, "On"}
                                                                              }};
            ReadWrite<2000, 0, UIntegerT<8>> transmitter_configuration{this, iolink::map_t<uint8_t>{ {0, "Off / Off"},
                                                                                                     {1, "On / On"},
                                                                                                     {2, "OFF_ExtActive / Off by external signal active"},
                                                                                                     {3, "On_ExtActive / On by external signal active"}
                                                                                                   }};
            ReadWrite<2005, 0, UIntegerT<16>> rate{this, 1_ui16, 33_ui16};
            Read<2008, 0, UIntegerT<16>> rep_r{this, 0_ui16, 9999_ui16};
            ReadWrite<2010, 0, IntegerT<16>> fsp1{this, int16_t(200), int16_t(9999)};
            ReadWrite<2011, 0, IntegerT<16>> nsp1{this, int16_t(200), int16_t(9999)};
            ReadWrite<2020, 0, IntegerT<16>> fsp2{this, int16_t(200), int16_t(9999)};
            ReadWrite<2021, 0, IntegerT<16>> nsp2{this, int16_t(200), int16_t(9999)};
    };
}

#endif // O1D105_H
