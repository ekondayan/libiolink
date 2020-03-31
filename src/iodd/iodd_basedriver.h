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

#ifndef IODD_BASEDRIVER_H
#define IODD_BASEDRIVER_H

#include "../iot/profileiolinkdevice.h"

namespace iolink::iodd
{
    using IOLinkDevice = iolink::iot::ProfileIOLinkDevice;

    class BaseDriver
    {
        public:
            BaseDriver() =delete;
            BaseDriver(const BaseDriver&) =delete;
            BaseDriver(BaseDriver&&) =delete;
            BaseDriver& operator= (const BaseDriver&) =delete;
            BaseDriver& operator= (BaseDriver&&) =delete;
            virtual ~BaseDriver() =default;

            inline std::shared_ptr<iolink::iot::ProfileIOLinkDevice> getIOLinkDevice() const
            {
                if(auto iolink_device = m_iolink_device.lock())
                    return iolink_device;

                throw iolink::utils::exception_logic(__func__, "Can't create a shared pointer to IOLinkDevice");
            }

        protected:
            BaseDriver(const std::weak_ptr<iolink::iot::ProfileIOLinkDevice>& iolink_device, int id_vendor, int id_device, bool check = true):
                m_vendor_id{id_vendor},
                m_device_id{id_device},
                m_iolink_device{iolink_device}
            {
                if(check)
                {
                    auto device = getIOLinkDevice();

                    if(device->vendorid.getData() != m_vendor_id)
                        throw iolink::utils::exception_logic(__func__, "Driver not suitable for the attached device. Vendor id does not match");

                    if(device->deviceid.getData() != m_device_id)
                        throw iolink::utils::exception_logic(__func__, "Driver not suitable for the attached device. Device id does not match");
                }
            }

        public:
            const int m_vendor_id;
            const int m_device_id;

        private:
            std::weak_ptr<iolink::iot::ProfileIOLinkDevice> m_iolink_device;
    };
}

#endif // IODD_BASEDRIVER_H
