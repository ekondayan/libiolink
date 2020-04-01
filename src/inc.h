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

#ifndef INC_LIB_H
#define INC_LIB_H

#include <memory>
#include <stdexcept>
#include <exception>
#include <climits>
#include <algorithm>
#include <iostream>
#include <regex>
#include <map>
#include <variant>
#include <vector>
#include <string>

#include "lib/json.hpp"

namespace iolink
{
//    TODO: cleanup. List all namespaces
//    using namespace iodd;
//    using namespace iot;
//    using namespace master;
//    using namespace device;
//    using namespace utils;

    using string_t = std::string;
    using vector_t = std::vector<uint8_t>;
    using json_t   = nlohmann::json;

    template<typename T>
    using map_t = std::unordered_map<T, const string_t>;
}

#endif // INC_LIB_H
