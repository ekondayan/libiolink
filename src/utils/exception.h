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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "inc.h"

namespace iolink::utils
{
    class exception_argument: public std::invalid_argument
    {
        public:
            exception_argument(const string_t &func_name, const string_t &message = string_t{}):
                std::invalid_argument{"Function[" + ((func_name.length() != 0)? func_name + "()":string_t{}) + "] " +
                                 "Error["    + ((message.length()   != 0)? message  :string_t{}) + "]"}
            {
            }

        private:
            const string_t   m_func_name;

    };

    class exception_logic: public std::logic_error
    {
        public:
            exception_logic(const string_t &func_name, const string_t &message = string_t{}):
                std::logic_error{"Function[" + ((func_name.length() != 0)? func_name + "()":string_t{}) + "] " +
                                 "Error["    + ((message.length()   != 0)? message  :string_t{}) + "]"}
            {
            }

        private:
            const string_t   m_func_name;

    };

    class exception_master: public std::exception
    {
        public:
            enum class ErrorCodeType: int{ERROR_230 = 230, ERROR_231 = 231, ERROR_232 = 232, ERROR_233 = 233,
                                          ERROR_400 = 400, ERROR_403 = 403,
                                          ERROR_500 = 500, ERROR_503 = 503, ERROR_530 = 530, ERROR_531 = 531, ERROR_532 = 532,
                                          BAD_RESPONSE = -1};

            exception_master(const string_t &func_name, ErrorCodeType error, const string_t &message = string_t{}):
                m_func_name{func_name},
                m_error_code{error}
            {
                m_error = "Function[" + ((func_name.length() != 0)? func_name + "()":string_t{}) + "] " +
                          "Code["     + std::to_string(static_cast<int>(error)) + "] " +
                          "Error["    + m_error_list.at(error) +
                          ((message.length() != 0)? " #" + message:string_t{}) +
                          "]";
            }

            explicit exception_master(ErrorCodeType error, const string_t &message = string_t{}):
                exception_master{string_t{}, error, message}
            {
            }

            ErrorCodeType error_code() const noexcept
            {
                return m_error_code;
            }

            string_t error() const noexcept
            {
                return m_error;
            }

            const char* what() const noexcept override
            {
                return m_error.c_str();
            }

        private:
            const string_t   m_func_name;
            const ErrorCodeType m_error_code;
            string_t   m_error;
            inline static const std::unordered_map<ErrorCodeType, const string_t> m_error_list
            {
                {ErrorCodeType::ERROR_230, "OK; but reboot required"},
                {ErrorCodeType::ERROR_231, "OK, but block request not yet terminated"},
                {ErrorCodeType::ERROR_232, "Data accepted but changed internally"},
                {ErrorCodeType::ERROR_233, "IP settings of the IoT core changed; application has to reboot the device; Wait for min. 1 second before the device is rebooted"},
                {ErrorCodeType::ERROR_400, "Invalid request"},
                {ErrorCodeType::ERROR_403, "Unauthorised access"},
                {ErrorCodeType::ERROR_500, "Internal server fault"},
                {ErrorCodeType::ERROR_503, "Service not available"},
                {ErrorCodeType::ERROR_530, "Requested data is invalid"},
                {ErrorCodeType::ERROR_531, "IO-Link error"},
                {ErrorCodeType::ERROR_532, "Error in PLC connection"},
                {ErrorCodeType::BAD_RESPONSE, "Bad response"}
            };
    };

    class exception_iodd: public std::exception
    {
        public:
            enum class ErrorCodeType: int{ERROR_INVALID_VALUE = 0,
                                          BAD_RESPONSE = -1};

            exception_iodd(const string_t &func_name, ErrorCodeType error, const string_t &message = string_t{}):
                m_func_name{func_name},
                m_error_code{error}
            {
                m_error = "Function[" + ((func_name.length() != 0)? func_name + "()":string_t{}) + "] " +
                          "Error["    + m_error_list.at(error) +
                          ((message.length() != 0)? " #" + message:string_t{}) +
                          "]";
            }

            explicit exception_iodd(ErrorCodeType error, const string_t &message = string_t{}):
                exception_iodd{string_t{}, error, message}
            {
            }

            string_t error() const noexcept
            {
                return m_error;
            }

            const char* what() const noexcept override
            {
                return m_error.c_str();
            }

        private:
            const string_t   m_func_name;
            const ErrorCodeType m_error_code;
            string_t   m_error;
            inline static const std::unordered_map<ErrorCodeType, const string_t> m_error_list
            {
                {ErrorCodeType::ERROR_INVALID_VALUE, "Invalid value"}
            };
    };
}

#endif // EXCEPTION_H
