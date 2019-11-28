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

#ifndef AL1352_NETWORK_H
#define AL1352_NETWORK_H

#include "libiolink/src/iot/profilenetwork.h"

namespace iolink::master::al1352
{
    using namespace iolink::iot;

    class Network: public ProfileNetwork
    {
        public:
            explicit Network(BaseElement *parent = nullptr):
                ProfileNetwork("network", parent)
            {}

            json_t setBlock(std::vector<std::pair<const BaseElement *, std::variant<int64_t,string_t>>> elements, bool consistent = false)
            {
                if(elements.empty()) return json_t{};

                string_t data{R"("datatosend":[)"};

                auto allempty = true;
                for(auto[first, it] = std::tuple(true, begin(elements)); it!=end(elements); ++it)
                {
                    auto element = std::get<0>(*it);
                    auto value = std::get<1>(*it);

                    if(element == nullptr || (std::holds_alternative<string_t>(value) && std::get<string_t>(value).empty())) continue;

                    if(!first) data.append(",");

                    data.append(R"(")" + element->id() + R"(":)");
                    if(std::holds_alternative<int64_t>(value))
                        data.append(std::to_string(std::get<int64_t>(value)));
                    else
                        data.append(R"(")"+std::get<string_t>(value)+R"(")");

                    first = false;
                    allempty = false;
                }
                data.append("]");

                if(allempty) return json_t{};

                if(consistent) data.append(R"(,"consistent":true)");

                return requestPost("/setblock", data);
            }

            json_t enableNetwork() =delete;
            json_t disableNetwork() =delete;

        private:
            using ProfileNetwork::ipv6address;
            using ProfileNetwork::ipversion;
            using ProfileNetwork::hostname;
            using ProfileNetwork::autonegotiation;
            using ProfileNetwork::portspeed;
    };

}
#endif // AL1352_NETWORK_H
