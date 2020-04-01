#ifndef MULTIANALOGINPUT_H
#define MULTIANALOGINPUT_H

#include "../../../../iodd/iodd_basedriver.h"
#include "../../../../iodd/iodd_dataaccess.h"
#include "../../../../iodd/iodd_datatypestring.h"
#include "../../../../iodd/iodd_datatypeuint.h"
#include "../../../../iodd/iodd_datatypeint.h"
#include "../../../../iodd/iodd_datatypetime.h"

namespace iolink::driver
{

    using namespace iodd;

    class AnalogConverterOutMulti: public BaseDriver
    {
        public:

            //        struct ProcessData
            //        {
            //                const int16_t distance = 0;
            //                const int16_t reflectivity = 0;
            //                const uint8_t status = 0;
            //                const bool    out1 = false;
            //                const bool    out2 = false;
            //        };

            AnalogConverterOutMulti() =delete;
            AnalogConverterOutMulti(const AnalogConverterOutMulti&) =delete;
            AnalogConverterOutMulti(AnalogConverterOutMulti&&) =delete;
            AnalogConverterOutMulti& operator= (const AnalogConverterOutMulti&) =delete;
            AnalogConverterOutMulti& operator= (AnalogConverterOutMulti&&) =delete;

            ~AnalogConverterOutMulti() =default;

            explicit AnalogConverterOutMulti(const std::weak_ptr<iolink::iot::ProfileIOLinkDevice>& iolink_device):
                BaseDriver(iolink_device, 303, 264128)
            {}

            //        ProcessData processData() const
            //        {
            //            auto data = iolink::utils::hexDecode<iolink::vector_t>(getIOLinkDevice()->pdin.getData());

            //            ProcessData reading = {
            //                .distance     = int16_t((int16_t(data[0]) << 8) | int16_t(data[1])),
            //                .reflectivity = int16_t((int16_t(data[4]) << 8) | int16_t(data[5])),
            //                .status       = uint8_t(data[7] >> 4),
            //                .out1         = (data[7] & 0x01)?true:false,
            //                .out2         = (data[7] & 0x02)?true:false
            //            };

            //            return reading;
            //        }

    };
}

#endif // MULTIANALOGINPUT_H
