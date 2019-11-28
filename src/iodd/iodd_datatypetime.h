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

#ifndef IODD_DATATYPETIME_H
#define IODD_DATATYPETIME_H

#include "libiolink/src/utils/inc.h"
#include "libiolink/src/utils/exception.h"

// TODO: implement unpackFromVector()
// TODO: implement packToVector()

namespace iolink::iodd
{
    class Time
    {
        public:
            using year_t   = int16_t;
            using month_t  = uint8_t;
            using day_t    = uint8_t;
            using hour_t   = uint8_t;
            using minute_t = uint8_t;
            using second_t = uint8_t;
            using ms_t     = uint16_t;

            Time() =default;
            Time(const Time&) =default;
            Time(Time&&) =default;
            Time& operator=(const Time&) =default;
            Time& operator=(Time&&) =default;
            ~Time() =default;

            explicit Time(const string_t& str)
            {
                setDateTime(str);
            }

            Time(const year_t year, const month_t month, const day_t day, const hour_t hour = 0, const minute_t minute = 0, const second_t second = 0, const ms_t ms = 0)
            {
                setDateTime(year, month, day, hour, minute, second, ms);
            }

            year_t   year()   const {return m_year;}
            month_t  month()  const {return m_month;}
            day_t    day()    const {return m_day;}

            hour_t   hour()   const {return m_hour;}
            minute_t minute() const {return m_minute;}
            second_t second() const {return m_second;}
            ms_t     ms()     const {return m_ms;}

            std::tuple<year_t, month_t, day_t> date() const
            {
                return {m_year, m_month, m_day};
            }

            std::tuple<hour_t, minute_t, second_t, ms_t> time() const
            {
                return {m_hour, m_minute, m_second, m_ms};
            }

            std::tuple<year_t, month_t, day_t, hour_t, minute_t, second_t, ms_t> dateTime() const
            {
                return {m_year, m_month, m_day, m_hour, m_minute, m_second, m_ms};
            }

            void setDateTime(const string_t date_time)
            {
                std::smatch matches;

                if(!std::regex_search(date_time, matches, m_reg_date_time))
                    throw iolink::utils::exception_logic(__func__, "Time format not correct");

                if(!matches.str(1).size() && !matches.str(5).size())
                    throw iolink::utils::exception_logic(__func__, "Parameter does not contain neither date nor time");

                if(matches.str(1).size())
                {
                    auto _year  = static_cast<year_t>(std::stol(matches.str(2)));
                    auto _month = static_cast<month_t>(std::stoul(matches.str(3)));
                    auto _day   = static_cast<day_t>(std::stoul(matches.str(4)));

                    setDate(_year, _month, _day);
                }

                if(matches.str(5).size())
                {
                    auto _hour   = static_cast<hour_t>  (std::stoul(matches.str(6)));
                    auto _minute = static_cast<minute_t>(std::stoul(matches.str(7)));
                    auto _second = static_cast<second_t>(std::stoul(matches.str(8)));
                    auto _ms     = 0;

                    if(matches.str(9).size())
                        _ms = static_cast<ms_t>(std::stoul(matches.str(10)));

                    setTime(_hour, _minute, _second, _ms);
                }
            }

            void setDateTime(const year_t year, const month_t month, const day_t day, const hour_t hour = 0, const minute_t minute = 0, const second_t second = 0, const ms_t ms = 0)
            {
                setDate(year, month, day);
                setTime(hour, minute, second, ms);
            }

            void setDate(std::tuple<year_t, month_t, day_t> date)
            {
                setDate(std::get<0>(date), std::get<1>(date), std::get<2>(date));
            }

            void setDate(const year_t year, const month_t month, const day_t day)
            {
                if(!isDateValid(year, month, day))
                    throw iolink::utils::exception_logic(__func__, "Date is not valid");

                m_year  = year;
                m_month = month;
                m_day   = day;
            }

            void setTime(const hour_t hour = 0, const minute_t minute = 0, const second_t second = 0, const ms_t ms = 0)
            {
                if(!isTimeValid(hour, minute, second, ms))
                    throw iolink::utils::exception_logic(__func__, "Time is not valid");

                m_hour   = hour;
                m_minute = minute;
                m_second = second;
                m_ms     = ms;
            }

            string_t dateToString() const
            {
                return                             std::to_string(m_year)   + "-" +
                        ((m_month  < 10)?"0":"") + std::to_string(m_month)  + "-" +
                        ((m_day    < 10)?"0":"") + std::to_string(m_day);
            }

            string_t timeToString() const
            {
                return  string_t("T") +
                        ((m_hour   < 10)?"0":"") + std::to_string(m_hour)   + ":" +
                        ((m_minute < 10)?"0":"") + std::to_string(m_minute) + ":" +
                        ((m_second < 10)?"0":"") + std::to_string(m_second) + "." +
                        std::to_string(m_ms);
            }

            string_t toString() const
            {
                return dateToString() + timeToString();
            }

            static bool isDateValid(const year_t year, const month_t month, const day_t day)
            {
                if(day < 1)
                    return false;

                switch (month)
                {
                    case 2:  // February
                        // Leap year is a multiple of 4 and not multiple of 100 or year is multiple of 400.
                        if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
                            return day <= 29;
                        else
                            return day <= 28;
                        break;

                    case 4:  // April
                    case 6:  // June
                    case 9:  // September
                    case 11: // November
                        return day <= 30;

                    case 1:  // January
                    case 3:  // March
                    case 5:  // May
                    case 7:  // July
                    case 8:  // August
                    case 10: // Octomber
                    case 12: // December
                        return day <= 31;
                }

                return false;
            }

            static bool isTimeValid(const hour_t hour, const minute_t minute, const second_t second, const ms_t ms)
            {
                if(hour > 23 || minute > 59 || second > 59 || ms > 999)
                    return false;

                return true;
            }

        private:
            year_t   m_year   = 0;
            month_t  m_month  = 1;
            day_t    m_day    = 1;
            hour_t   m_hour   = 0;
            minute_t m_minute = 0;
            second_t m_second = 0;
            ms_t     m_ms     = 0;

            static inline const std::regex m_reg_date_time{R"(^((\d{4})\-(\d{2})\-(\d{2}))?(T(\d{2}):(\d{2}):(\d{2})(\.(\d{1,3}))?)?$)"};
    };

    class TimeT
    {
        public:
            using type_t = Time;
            using iodd_type_t = string_t;

            TimeT(const TimeT&) =delete;
            TimeT(TimeT&&) =delete;
            TimeT& operator =(const TimeT&) =delete;
            TimeT& operator =(TimeT&&) =delete;
            ~TimeT() = default;

            explicit TimeT()
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
                return value.toString();
            }
    };
}

#endif // IODD_DATATYPETIME_H
