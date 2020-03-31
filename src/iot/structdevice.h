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

#ifndef STRUCT_DEVICE_H
#define STRUCT_DEVICE_H

#include "../iot/base.h"

namespace iolink::iot
{
    class StructDevice: public BaseElement
    {
        public:
            explicit StructDevice(const string_t &id, std::unique_ptr<InterfaceComm> comm = nullptr):
                BaseElement{id, nullptr, std::move(comm)}
            {}

            json_t getTree() const{return requestGet("/gettree");}
            json_t getIdentity() const{return requestGet("/getidentity");}

            json_t getDataMulti(std::vector<string_t> element_urls, bool consistent = false) const
            {
                // Erase all empty urls
                element_urls.erase(remove_if(element_urls.begin(),
                                             element_urls.end(),
                                             [](const string_t &el){return el.empty();}),
                                   element_urls.end()
                                   );

                if(element_urls.empty()) return json_t{};

                json_t data;
                data["datatosend"] = element_urls;
                data["consistent"] = consistent;

                return requestPost("/getdatamulti", data);
            };

            json_t getElementInfo(const string_t &url) const{return requestPost("/getelementinfo", R"("url":")"+url+R"(")");}
            json_t setElementInfo(const string_t &url, const string_t &uid, std::vector<string_t> profiles) const; // FIXME: complete implementation of setElementInfo

            void setCommClass(std::unique_ptr<InterfaceComm> comm = nullptr)
            {
                m_comm = std::move(comm);
            }
    };
}


#endif // AL1352_DEVICE_H
