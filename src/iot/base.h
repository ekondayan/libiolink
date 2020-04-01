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

#ifndef BASE_H
#define BASE_H

#include "interfacecomm.h"
#include "../exception.h"

namespace iolink::iot
{
    // TODO: да го направя темплейт и да вкарам функциите на DataEvent и DataUnit.
    // Това не работи!!! Да измисля по-елегантен начин!!!
    // template<bool is_event, bool is_unit>
    class BaseElement
    {
        public:
            BaseElement() =delete;
            BaseElement(const BaseElement&) =delete;
            BaseElement(BaseElement&&) =delete;
            BaseElement& operator= (const BaseElement&) =delete;
            BaseElement& operator= (BaseElement&&) =delete;

            virtual ~BaseElement() =default;

            virtual string_t address() const
            {
                auto adr = (m_id.empty() || !m_parent)?string_t{}:string_t{"/" + m_id};

                if(m_parent)
                    adr.insert(0, m_parent->address());

                return adr;
            }

            virtual string_t id() const
            {
                return m_id;
            }

        protected:
            BaseElement(const string_t& id, BaseElement* const parent = nullptr, std::unique_ptr<InterfaceComm> comm = nullptr):
                m_comm{std::move(comm)},
                m_parent{parent},
                m_id{id}
            {
                if( (!m_parent && !m_comm) || (m_parent && m_comm))
                    throw iolink::utils::exception_argument(__func__, "Either Parent or Communication object must be set");
            }

            json_t requestGet(string_t adr) const
            {
                if(adr.empty())
                    throw iolink::utils::exception_argument(__func__, "Address argument must be non empty string");

                if(m_parent)
                {
                    if(!m_id.empty())
                        adr.insert(0, "/"+m_id);

                    return m_parent->requestGet(adr);
                }

                if(!m_comm)
                    throw iolink::utils::exception_logic(__func__, "Communication object not set");

                if(m_comm->isSecurityMode())
                    return requestPost(adr);

                return checkResponseCode(json_t::parse(m_comm->httpGet(adr)));
            }

            json_t requestPost(const string_t& adr, const string_t& data) const
            {
                return requestPost(adr, json_t::parse("{"+data+"}"));
            }

            json_t requestPost(string_t adr, const json_t& data = {}) const
            {
                if(adr.empty())
                    throw iolink::utils::exception_argument(__func__, "Address argument must be non empty string");

                if(m_parent)
                {
                    if(!m_id.empty())
                        adr.insert(0, "/"+m_id);

                    return m_parent->requestPost(adr, data);
                }

                if(!m_comm)
                    throw iolink::utils::exception_logic(__func__, "Communication object not set");

                json_t request;
                request["cid"]  = -1;
                request["code"] = "request";
                request["adr"]  = adr;
                if(!data.empty())
                    request["data"] = data;
                m_comm->applySecurityToRequestObject(request);

                return checkResponseCode(json_t::parse(m_comm->httpPost(request.dump())));
            }

        private:
            const json_t& checkResponseCode(const json_t& response) const
            {
                switch (string_t error = response.contains("error")?response["error"]:""; static_cast<int>(response["code"]))
                {
                    case 200: break;
                    case 230: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_230, error);
                    case 231: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_231, error);
                    case 232: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_232, error);
                    case 233: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_233, error);
                    case 400: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_400, error);
                    case 403: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_403, error);
                    case 500: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_500, error);
                    case 503: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_503, error);
                    case 530: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_530, error);
                    case 531: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_531, error);
                    case 532: throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::ERROR_532, error);
                    default : throw utils::exception_master(__func__, utils::exception_master::ErrorCodeType::BAD_RESPONSE);
                }

                return response;
            }

        protected:
            std::unique_ptr<InterfaceComm> m_comm;
            const BaseElement* const       m_parent;
            const string_t                 m_id;
    };
}

#endif // BASE_H
