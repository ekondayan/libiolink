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

#ifndef PROFILEIOLINKDEVICE_H
#define PROFILEIOLINKDEVICE_H

#include "dataaccess.h"

#include "../utils.h"

namespace iolink::iodd
{
    class BaseDriver;
}

namespace iolink::iot
{
    class ProfileIOLinkDevice: public BaseElement
    {
        public:
            explicit ProfileIOLinkDevice(BaseElement *parent):
                BaseElement{"iolinkdevice", parent},
                m_self_ptr{this, [](void *){}} // Pass null deleter, because this object is managed by the caller(can be allocated on the heap or be static)
            {
                if(!m_parent)
                    throw iolink::utils::exception_argument(__func__, "Parent for this element can not be empty");
            };

            json_t iolReadAcyclic(uint32_t index, uint32_t sub_index = 0) const
            {
                return requestPost("/iolreadacyclic", R"("index":)"+std::to_string(index)+R"(,"subindex":)"+std::to_string(sub_index));
            }

            json_t iolWriteAcyclic(const string_t &value, uint32_t index, uint32_t sub_index) const
            {
                return requestPost("/iolwriteacyclic", R"("index":)"+std::to_string(index)+R"(,"subindex":)"+std::to_string(sub_index)+R"(,"value":")"+value+R"(")");
            }

            template<typename T>
            void write(T value, uint32_t index, uint32_t sub_index = 0) const
            {
                iolWriteAcyclic(utils::hexEncode(std::forward<T>(value)), index, sub_index);
            }

            template<typename T>
            T read(uint32_t index, uint32_t sub_index = 0) const
            {
                return utils::hexDecode<T>(string_t{iolReadAcyclic(index, sub_index)["data"]["value"]});
            }

            template<typename T>
            std::weak_ptr<T> driverAttach()
            {
                if constexpr(std::is_base_of_v<iodd::BaseDriver, T>)
                {
                    if(m_driver && !m_driver.unique())
                        throw iolink::utils::exception_logic(__func__, "Driver still in use. Please release all shared pointers to the old driver before reataching a new one");

                    m_driver.reset(new T(m_self_ptr));
                    return std::dynamic_pointer_cast<T>(m_driver);
                }
                else
                {
                    throw iolink::utils::exception_logic(__func__, "Class not derived from iodd::BaseDriver");
                }
            }

            void driverDetach()
            {
                if(m_driver && !m_driver.unique())
                    throw iolink::utils::exception_logic(__func__, "Driver still in use. Please release all shared pointers to the driver before detaching it");

                m_driver.reset();
            }

            template<typename T>
            std::weak_ptr<T> driver() const
            {
                if constexpr(std::is_base_of_v<iodd::BaseDriver, T>)
                {
                    return std::dynamic_pointer_cast<T>(m_driver);
                }
                else
                {
                    throw iolink::utils::exception_logic(__func__, "Class not derived from iodd::BaseDriver");
                }
            }

            AccessRead<DataTypeInt>         vendorid              {"vendorid", this, 0, 65535};
            AccessRead<DataTypeInt>         deviceid              {"deviceid", this, 0, 16777215};
            AccessRead<DataTypeString>      productname           {"productname", this};
            AccessRead<DataTypeString>      serial                {"serial", this};
            AccessReadWrite<DataTypeString> applicationspecifictag{"applicationspecifictag", this};
            AccessRead<DataTypeString>      pdin                  {"pdin", this};
            AccessReadWrite<DataTypeString> pdout                 {"pdout", this};
            AccessRead<DataTypeEnum>        status                {"status", this, DataTypeEnum::Map{
                    {0, "State not connected"},
                    {1, "State preoperate"},
                    {2, "State operate"},
                    {3, "State communication error"}}};
            AccessReadWrite<DataTypeString, DataEvent> iolinkevent{"iolinkevent", this};

        private:
            std::shared_ptr<iodd::BaseDriver> m_driver;
            /*
             * Pointing to self. Used only as reference counter passed to drivers. It is drivers responsibility
             * to check if this pointer is not expired. This object can expire if it gets out of scope or gets deleted
             * by the caller.
             */
            std::shared_ptr<ProfileIOLinkDevice> m_self_ptr;
    };
}

#endif // PROFILEIOLINKDEVICE_H
