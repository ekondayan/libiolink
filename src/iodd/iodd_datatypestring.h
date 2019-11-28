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

#ifndef IODD_DATATYPESTRING_H
#define IODD_DATATYPESTRING_H

#include "libiolink/src/utils/inc.h"
#include "libiolink/src/utils/exception.h"

// TODO: implement unpackFromVector() only when the length is fixed
// TODO: implement packToVector() only when the length is fixed

namespace iolink::iodd
{
    class StringT
    {
        public:
            using type_t = string_t;
            using iodd_type_t = string_t;

            enum class EncodingType{UTF8, ASCII};

            StringT(const StringT&) =delete;
            StringT(StringT&&) =delete;
            StringT& operator =(const StringT&) =delete;
            StringT& operator =(StringT&&) =delete;
            ~StringT() = default;

            explicit StringT(uint32_t min_len, uint32_t max_len, EncodingType encoding):
                m_min_len{min_len},
                m_max_len{max_len},
                m_encoding{encoding}
            {
                if(min_len > max_len)
                    throw iolink::utils::exception_argument(__func__, "Minimum value can't be higher than the max value");
            }

            explicit StringT(uint32_t max_len, EncodingType encoding = EncodingType::UTF8):
                StringT{std::numeric_limits<uint32_t>::min(), max_len, encoding}
            {
            }

            explicit StringT(EncodingType encoding = EncodingType::UTF8):
                StringT{std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max(), encoding}
            {

            }


            bool isValid(const type_t& value) const
            {
                auto len = value.length();
                return (m_min_len <= len && len <= m_max_len);
            }

            bool isValid(type_t&& value) const
            {
                return isValid(value);
            }

            type_t toType(const iodd_type_t& iodd_value) const
            {
                type_t new_string;

                for(size_t i = 1; i <= iodd_value.size(); ++i)
                {
                    const char& ch = iodd_value.at(i - 1);

                    if(ch == '&')
                    {
                        size_t len = 0;
                        try
                        {
                            if(len = 3; iodd_value.compare(i, len, "amp") == 0)
                                new_string.append("&");
                            else if(len = 2; iodd_value.compare(i, len, "gt") == 0)
                                new_string.append(">");
                            else if(len = 2; iodd_value.compare(i, len, "lt") == 0)
                                new_string.append("<");
                            else if(len = 4; iodd_value.compare(i, len, "apos") == 0)
                                new_string.append("\\'");
                            else if(len = 4; iodd_value.compare(i, len, "quot") == 0)
                                new_string.append("\\\"");
                            else
                                len = 0;
                        }
                        catch (...) {
                        }

                        if(len)
                        {
                            i += len;
                            continue;
                        }
                        else
                            throw iolink::utils::exception_logic(__func__, "String contains illegal characted");

                    }

                    new_string.append(std::string{ch});
                }

                return new_string;
            }

            const iodd_type_t toIoddType(const type_t& value) const
            {
                type_t new_string;

                for(size_t i = 1; i <= value.size(); ++i)
                {
                    const char& ch = value.at(i - 1);

                    switch (ch)
                    {
                        case '\\':
                        {
                            size_t len = 0;
                            try
                            {
                                if(len = 1; value.compare(i, len, "'") == 0)
                                    new_string.append("&apos");
                                else if(len = 1; value.compare(i, len, "\"") == 0)
                                    new_string.append("&quot");
                            }
                            catch (...) {
                            }

                            if(len)
                            {
                                i += len;
                                break;
                            }
                            else
                                new_string.append(std::string{ch});
                        }
                            break;

                        case '&':
                            new_string.append("&amp");
                            break;

                        case '<':
                            new_string.append("&lt");
                            break;

                        case '>':
                            new_string.append("&gt");
                            break;

                        default:
                            new_string.append(std::string{ch});
                            break;
                    }
                }

                if(new_string.size() < m_min_len)
                    throw iolink::utils::exception_logic(__func__, "String size less than minimum value");

                if(new_string.size() > m_max_len)
                    throw iolink::utils::exception_logic(__func__, "String size greater than maximum value");

                return new_string;
            }

        private:
            const uint32_t m_min_len      = std::numeric_limits<uint32_t>::min();
            const uint32_t m_max_len      = std::numeric_limits<uint32_t>::max();
            const EncodingType m_encoding = EncodingType::UTF8;
    };
}

#endif // IODD_DATATYPESTRING_H
