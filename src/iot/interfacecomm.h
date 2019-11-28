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

#ifndef INTERFACECOMM_H
#define INTERFACECOMM_H

#include "libiolink/src/utils/inc.h"
#include "libiolink/src/utils/exception.h"

namespace iolink::iot
{
    class InterfaceComm
    {
        public:
            InterfaceComm() =delete;
            InterfaceComm(const InterfaceComm&) =delete;
            InterfaceComm(InterfaceComm&&) =delete;
            InterfaceComm& operator= (const InterfaceComm&) =delete;
            InterfaceComm& operator= (InterfaceComm&&) =delete;

            virtual ~InterfaceComm() =default;

            virtual string_t httpGet(const string_t &url) const =0;
            virtual string_t httpPost(const string_t &json_t) const =0;

        protected:
            InterfaceComm(const string_t &ip, const uint16_t port = 80):
                m_ip{ip},
                m_port{port}
            {
                if(port == 0)
                    throw iolink::utils::exception_argument(__func__, "Port must be in the range 1:65535");

                std::regex regex_ip{R"((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))"};
                if(!regex_match(ip, regex_ip))
                    throw iolink::utils::exception_argument(__func__, "Incorrect IP address (example: nnn.nnn.nnn.nnn)");
            }

            string_t urlFromAddress(const string_t &adr = string_t{}) const
            {
                return "http://"+m_ip+":"+std::to_string(m_port)+adr;
            }

        protected:
            const string_t m_ip;
            const uint16_t m_port;
    };
}

#endif // INTERFACECOMM_H
