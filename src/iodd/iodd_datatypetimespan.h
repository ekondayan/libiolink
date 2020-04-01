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

#ifndef IODD_TIMESPAN_H
#define IODD_TIMESPAN_H

#include "../inc.h"
#include "../exception.h"

// TODO: implement unpackFromVector()
// TODO: implement packToVector()

namespace iolink::iodd
{
    class TimeSpan
    {
        public:
            using type_t   = int64_t;
            using hour_t   = uint8_t;
            using minute_t = uint8_t;
            using second_t = uint8_t;
            using ms_t     = uint16_t;
            using sign_t   = bool;

            TimeSpan() =default;
            TimeSpan(const TimeSpan&) =default;
            TimeSpan(TimeSpan&&) =default;
            TimeSpan& operator=(const TimeSpan&) =default;
            TimeSpan& operator=(TimeSpan&&) =default;
            ~TimeSpan() =default;

            explicit TimeSpan(type_t time_span)
            {

                setTimeSpan(time_span);
            }

            explicit TimeSpan(const string_t& str)
            {
                setTimeSpan(str);
            }

            TimeSpan(const hour_t hour = 0, const minute_t minute = 0, const second_t second = 0, const ms_t ms = 0, const sign_t sign = true)
            {
                setTimeSpan(hour, minute, second, ms, sign);
            }

            void setTimeSpan(type_t time_span)
            {
                this->m_time_span = time_span;
            }

            void setTimeSpan(const string_t time)
            {
                std::smatch matches;

                if(!std::regex_search(time, matches, m_reg_timespan))
                    throw iolink::utils::exception_logic(__func__, "TimeSpan format not correct");

                auto _sign = true;
                if(matches.str(1).size() && matches.str(1).compare("-") == 0)
                    _sign = false;

                if(matches.str(2).size())
                {
                    uint64_t _second = std::stoul(matches.str(2));

                    auto _hour = _second / 3600;
                    _second -= _hour * 3600;

                    auto _minute = _second / 60;
                    _second -= _minute * 60;

                    auto _ms     = 0;

                    if(matches.str(3).size())
                        _ms = static_cast<ms_t>(std::stoul(matches.str(4)));

                    setTimeSpan(_hour, _minute, _second, _ms, _sign);
                }
            }

            void setTimeSpan(const hour_t hour = 0, const minute_t minute = 0, const second_t second = 0, const ms_t ms = 0, const sign_t sign = true)
            {
                if(!isTimeSpanValid(hour, minute, second, ms))
                    throw iolink::utils::exception_logic(__func__, "Time is not valid");
            }

            string_t toString() const
            {
//                return  ((m_fractions >= 0)?"+":"-") +
//                        string_t("PT")   +
//                        std::to_string(m_hour * 3600 + m_minute * 60 + m_second) + "." +
//                        std::to_string(m_ms)                                     + "S";
            }

            static bool isTimeSpanValid(const hour_t hour, const minute_t minute, const second_t second, const ms_t ms)
            {
                if(hour > 23 || minute > 59 || second > 59 || ms > 999 || (hour == 0 && minute == 0 && second == 0 && ms == 0) )
                    return false;

                return true;
            }

            operator type_t() const
            {
                return m_time_span;
            }

        private:
            type_t   m_time_span = 0;

            static inline const std::regex m_reg_timespan{R"(^([+-]?)PT(\d+)(\.(\d{1,3}))?S$)"};
    };

    class TimeSpanT
    {
        public:
            using type_t = TimeSpan;
            using iodd_type_t = int64_t;

            TimeSpanT(const TimeSpanT&) =delete;
            TimeSpanT(TimeSpanT&&) =delete;
            TimeSpanT& operator =(const TimeSpanT&) =delete;
            TimeSpanT& operator =(TimeSpanT&&) =delete;
            ~TimeSpanT() = default;

            explicit TimeSpanT()
            {
            }

            bool isValid(const type_t& value) const
            {
                try
                {
                    type_t{value};
                }
                catch (...)
                {
                    return false;
                }
                return true;
            }

            type_t toType(const iodd_type_t& iodd_value) const
            {
                return type_t{iodd_value};
            }

            iodd_type_t toIoddType(const type_t& value) const
            {
                return value;
            }
    };
}

#endif // IODD_TIMESPAN_H
