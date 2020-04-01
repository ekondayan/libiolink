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

#ifndef DATATYPE_H
#define DATATYPE_H

#include "../inc.h"

#include "base.h"

namespace iolink::iot
{
    class DataEvent: public BaseElement
    {
        public:
            json_t subscribe(const string_t &callback_url, std::vector<string_t> element_urls) const
            {
                // Erase all empty urls
                element_urls.erase(remove_if(element_urls.begin(), element_urls.end(),
                                             [](const string_t &el){return el.empty();}),
                                   element_urls.end());

                if(callback_url.empty())
                    throw iolink::utils::exception_argument(__func__, "Callback urls can not be empty");

                if(element_urls.empty())
                    throw iolink::utils::exception_argument(__func__, "Element urls can not be empty");

                json_t data;
                data["callback"] = callback_url;
                data["data"] = element_urls;

                return requestPost("/subscribe", data);
            }

            json_t unsubscribe(const string_t &callback_url) const
            {
                if(callback_url.empty())
                    throw iolink::utils::exception_argument(__func__, "Callback urls can not be empty");

                return requestPost("/unsubscribe", R"("callback":")" + callback_url + R"(")");
            }

            json_t getSubscriptionInfo(const string_t &callback_url) const
            {
                if(callback_url.empty())
                    throw iolink::utils::exception_argument(__func__, "Callback urls can not be empty");

                return requestPost("/getsubscriptioninfo", R"("callback":")" + callback_url + R"(")");
            }

        protected:
            DataEvent(const string_t &id, BaseElement* const parent):
                BaseElement{id+"/datachanged", parent}
            {}
    };

    class DataUnit: public BaseElement
    {
        public:
            json_t unitJson() const
            {
                return requestGet("/getdata");
            }

            string_t unit() const
            {
                return unitJson()["data"]["value"];
            }

        protected:
            DataUnit(const string_t &id, BaseElement* const parent):
                BaseElement{id+"/unit", parent}
            {}
    };

    class DataTypeInt: public BaseElement
    {
        public:
            using type_t = int64_t;
            using List = std::initializer_list<type_t>;

            bool isValid(type_t value) const
            {
                return (((m_min <= value) && (value <= m_max)) || (find(begin(m_single_values), end(m_single_values), value) != end(m_single_values)));
            }

        protected:
            DataTypeInt(const string_t &id, BaseElement* const parent, type_t min, type_t max, std::initializer_list<type_t> single_values = {}):
                BaseElement{id, parent},
                m_min{min},
                m_max{max}
            {
                if(min > max)
                    throw iolink::utils::exception_argument(__func__, "Invalid values for min/max parameters");

                for(const auto single_value: single_values)
                    m_single_values.emplace_back(single_value);
            }

            DataTypeInt(const string_t &id, BaseElement* const parent, std::initializer_list<type_t> single_values):
                BaseElement{id, parent}
            {
                for(const auto single_value: single_values)
                    m_single_values.emplace_back(single_value);
            }

            DataTypeInt(const string_t &id, BaseElement* const parent):
                BaseElement{id, parent}
            {
            }

        private:
            const type_t m_min{std::numeric_limits<type_t>::min()};
            const type_t m_max{std::numeric_limits<type_t>::max()};
            std::vector<type_t> m_single_values;
    };

    class DataTypeString: public BaseElement
    {
        public:
            using type_t = string_t;

            bool isValid(const type_t &value) const
            {
                return ((m_min <= value.length()) && (value.length() <= m_max));
            }

        protected:
            DataTypeString(const string_t &id, BaseElement* const parent, const size_t min, const size_t max):
                BaseElement{id, parent},
                m_min{min},
                m_max{max}
            {
                if(min > max)
                    throw iolink::utils::exception_argument(__func__, "Invalid values for min/max parameters");
            }

            DataTypeString(const string_t &id, BaseElement* const parent, const size_t max):
                BaseElement{id, parent},
                m_max{max}
            {
            }

            DataTypeString(const string_t &id, BaseElement* const parent):
                BaseElement{id, parent}
            {
            }

        private:
            const size_t m_min{std::numeric_limits<size_t>::min()};
            const size_t m_max{std::numeric_limits<size_t>::max()};
    };

    class DataTypeEnum: public BaseElement
    {
        public:
            using type_t = int16_t;
            using Map = map_t<type_t>;

            string_t valueToString(type_t value) const
            {
                if(!isValid(value))
                    throw iolink::utils::exception_argument(__func__, "Trying to set a non existent key for enum");

                return m_enum.at(value);
            }

            bool isValid(type_t value) const
            {
                return m_enum.find(value) != m_enum.end();
            }

        protected:
            DataTypeEnum(const string_t &id, BaseElement *parent, const Map &enumeration):
                BaseElement{id, parent},
                m_enum{enumeration}
            {
                if(enumeration.empty())
                    throw iolink::utils::exception_argument(__func__, "Empty enumerations");
            }

        private:
            const Map m_enum;
    };
}

#endif // DATATYPE_H
