# O1D105

In this tutorial I will guide you how to implement a driver for IFM's **O1D105** laser sensor from scratch. 

First things first. Let's clone the repository from the GitHub:

```bash
git clone https://github.com/ekondayan/libiolink.git libiolink
```

Next we create the filesystem structure. Go to `src/driver/device/<MANUFACTURER>/<DEVICE>` where:

- <MANUFACTURER> is the name of the device's manufacturer. If the directory does not exists, create a new one named `ifm`

- <DEVICE> is the name of the device. If the directory does not exists, create a new one named `o1d105`

Usually the driver consists of a single header file. This is not a mandatory requirement, but is the recommended approach. Now create an empty header file, named it after the device name - `o1d105.h`.

```bash
cd libiolink/src/driver/device
mkdir -p ifm/o1d105/
touch o1d105.h
```

Every driver must be implemented within the `iolink::driver` namespace and inherit from `BaseDriver`. The `BaseDriver` constructor acceps a weak pointer to the master's driver, `vendorid` and `deviceid`. It reads the `vendorid` and `deviceid` from the device and check if they match.

This is how a device driver skeleton looks like:

```cpp
#ifndef O1D105_H
#define O1D105_H

namespace iolink::driver
{
    using namespace utils;
    using namespace iodd;

    class O1D105: public BaseDriver
    {
        public:
            O1D105() =delete;
            O1D105(const O1D105&) =delete;
            O1D105(O1D105&&) =delete;
            O1D105& operator= (const O1D105&) =delete;
            O1D105& operator= (O1D105&&) =delete;

            ~O1D105() =default;

            explicit O1D105(const std::weak_ptr<iolink::iot::ProfileIOLinkDevice>& iolink_device):
                BaseDriver(iolink_device, 310, 806)
            {}
    }
}

#endif // O1D105_H
```

Copy and paste the code above in the driver's file `o1d105.h`. The lifetime of the driver instance is controlle by the master's driver that is why the copy, move operations and default constructor are disabled. At this point you can attach the newly created driver to the master's object by calling `driverAttach<O1D105>()`. The template parameter is the name of the driver's class, in this case `O1D105`.

```cpp
auto driver_w = <MASTER_OBJECT>.iolinkmaster.port1.iolinkdevice.driverAttach<O1D105>();
```

This is the bare minimum to have a working driver, but without handling the Process Data and the Acyclic Data, this driver wouldn't be much usefull. At this point you must get familiar with the device IO-Link interface description. Most of device manufacturers provide a PDF version, which is much easier to read. If you can not find the PDF file for your device, you have to dig into the IO-Device Description XML files. Luckily ifm provide a PDF file, which can be found here: [O1D105](https://www.ifm.com/download/files/ifm-O1D105-20180125-IODD11-en/$file/ifm-O1D105-20180125-IODD11-en.pdf)

Reading a Process Data involves two steps:

- declare a simple structure, that will hold the values of the Process Data

```cpp
struct ProcessData
{
    const int16_t distance = 0;
    const int16_t reflectivity = 0;
    const uint8_t status = 0;
    const bool    out1 = false;
    const bool    out2 = false;
};
```

- read the Process Data from the master and decode it into a vector by calling `decodeFromHexStringiolink::vector_t`. The ifm masters return the data as a hex encoded string. To read the process data from the device, you have to acuire a shared pointer to the master first by calling `getIOLinkDevice()`. This is a helper function inherited from `BaseDriver`. It tries to lock the weak pointer to the master's object. If it fails an exception is trown.

```cpp
auto data = decodeFromHexString<iolink::vector_t>(getIOLinkDevice()->pdin.getData());
```

- parse the data into a ProcessData object

```cpp
ProcessData reading = {
    .distance     = int16_t((int16_t(data[0]) << 8) | int16_t(data[1])),
    .reflectivity = int16_t((int16_t(data[4]) << 8) | int16_t(data[5])),
    .status       = uint8_t(data[7] >> 4),
    .out1         = (data[7] & 0x01)?true:false,
    .out2         = (data[7] & 0x02)?true:false
};

return reading;
```

Create a function `ProcessData processData() const` and put the code above into it.

```cpp
struct ProcessData
{
    const int16_t distance     = 0;
    const int16_t reflectivity = 0;
    const uint8_t status       = 0;
    const bool    out1         = false;
    const bool    out2         = false;
};

ProcessData processData() const
{
    auto data = decodeFromHexString<iolink::vector_t>(getIOLinkDevice()->pdin.getData());

    ProcessData reading = {
        .distance     = int16_t((int16_t(data[0]) << 8) | int16_t(data[1])),
        .reflectivity = int16_t((int16_t(data[4]) << 8) | int16_t(data[5])),
        .status       = uint8_t(data[7] >> 4),
        .out1         = (data[7] & 0x01)?true:false,
        .out2         = (data[7] & 0x02)?true:false
    };

    return reading;
}
```

Now you have a real working driver, but this is only half of the job. The other half involves implementing the asynchronous calls for accessing the device's parameters. This step is a little bit more involving because it requires you to get familiar with the IODD and the Access classes. 

There are three template  Access classes: Read, Write, ReadWrite. The first and the second arguments are the index and subindex of the parameter. The third parameter is the IODD type. In the constructor the first parameter must be a pointer to the driver itself. The rest of the parameters are forwarded to the IODD constructor.

For example the `vendor_name` parameter is a read only of type StringT. It's index is `16` and the subindex is `0`. You can define it like this:

```cpp
Read<16, 0, StringT> vendor_name{this, 19_ui32};
```

The first parameter in the constructor is a pointer to the driver. The second one, specifies the maximum string length. The literal `_ui32` converts from signed 32 bit integer to unsigned 32 bit integer.

Another example is the `dS1` parameter. It is ReadWrite with index `370` and subindex `0`. It's type is 16 bit length UIntegerT with minimum value of `0` and maximum `5000`.

```cpp
ReadWrite<370, 0, UIntegerT<16>> dS1{this, 0_ui16, 5000_ui16};
```

Here is the full list of `O1D105` parameters:

```cpp
Write<2, 0, UIntegerT<8>> standard_command{this, iolink::map_t<uint8_t>{ {67, "Teach SP TP1"},
{68, "Teach SP TP2"},
{75, "Teach Custom - Background"},
{130, "Restore Factory Settings"},
{240, "IO-Link 1.1 system test command 240, Event 8DFE appears"},
{241, "IO-Link 1.1 system test command 241, Event 8DFE disappears"},
{242, "IO-Link 1.1 system test command 242, Event 8DFF appears"},
{243, "IO-Link 1.1 system test command 243, Event 8DFF disappears"},
}};
            // 12 device_access_lock
Read<16, 0, StringT> vendor_name{this, 19_ui32};
Read<17, 0, StringT> vendor_text{this, 11_ui32};
Read<18, 0, StringT> product_name{this, 6_ui32};
Read<19, 0, StringT> product_id{this, 6_ui32};
Read<20, 0, StringT> product_text{this, 12_ui32};
Read<21, 0, StringT> serial_number{this, 12_ui32};
Read<22, 0, StringT> firmware_version{this, 8_ui32};
Read<23, 0, StringT> software_version{this, 8_ui32};
ReadWrite<24, 0, StringT> application_tag{this, 32_ui32};
Read<36, 0, UIntegerT<8>> device_status{this, iolink::map_t<uint8_t>{
{0, "Device is OK"},
{1, "Maintenance required"},
{2, "Out of specification"},
{3, "Functional check"},
{4, "Failure"}
}};
Read<37, 0, ArrayT<UIntegerT<8>, 24>> detailed_device_status{this, 10_ui8, 20_ui8};
ReadWrite<58, 0, UIntegerT<8>> ti_selection{this, iolink::map_t<uint8_t>{{1, "OUT1"},
{2, "OUT2"}
}};
            // 59 ti_result
ReadWrite<370, 0, UIntegerT<16>> dS1{this, 0_ui16, 5000_ui16};
ReadWrite<371, 0, UIntegerT<16>> dr1{this, 0_ui16, 5000_ui16};
ReadWrite<372, 0, UIntegerT<16>> dS2{this, 0_ui16, 5000_ui16};
ReadWrite<373, 0, UIntegerT<16>> dr2{this, 0_ui16, 5000_ui16};
ReadWrite<530, 0, UIntegerT<16>> dFo{this, 0_ui16, 5000_ui16};
Read<541, 0, IntegerT<32>> power_cycles{this, 0, 2147483647};
Read<542, 0, IntegerT<32>> operating_hours{this, 0, 2147483647};
            // 545 active_events
Read<546, 0, ArrayT<UIntegerT<32>, 10>> param_config_fault{this};
ReadWrite<550, 0, UIntegerT<8>> loc{this, iolink::map_t<uint8_t>{ 
{0, "Loc"},
{1, "uLoc"}
}};
ReadWrite<551, 0, UIntegerT<8>> uni{this, iolink::map_t<uint8_t>{
{0, "mm"},
{1, "m"},
{2, "in"}
}};
ReadWrite<580, 0, UIntegerT<8>> ou1{this, iolink::map_t<uint8_t>{ 
{3,  "Hno / Hysteresis fct normally open"},
{4,  "Hnc / Hysteresis fct normally closed"},
{5,  "Fno / Window fct normally open"},
{6,  "Fnc / Window fct normally closed"},
{16, "Off / Output Off"}
}};
ReadWrite<583, 0, IntegerT<16>> sp1{this, int16_t(200), int16_t(9999)};
ReadWrite<590, 0, UIntegerT<8>> ou2{this, iolink::map_t<uint8_t>{ 
{1,  "I / Analog signal 4...20 mA"},
{2,  "U / Analog signal 0...10 V"},
{3,  "Hno / Hysteresis fct normally open"},
{4,  "Hnc / Hysteresis fct normally closed"},
{5,  "Fno / Window fct normally open"},
{6,  "Fnc / Window fct normally closed"},
{16, "Off / Output Off"}
}};
ReadWrite<593, 0, IntegerT<16>> sp2{this, int16_t(200), int16_t(9999)};
ReadWrite<630, 0, IntegerT<16>> asp{this, int16_t(0), int16_t(9999)};
ReadWrite<631, 0, IntegerT<16>> aep{this, int16_t(0), int16_t(9999)};
ReadWrite<800, 0, UIntegerT<8>> dis_u{this, iolink::map_t<uint8_t>{ 
{0, "d1 / fast"},
{1, "d2 / medium"},
{2, "d3 / slow"}
}};
ReadWrite<801, 0, UIntegerT<8>> dis_r{this, iolink::map_t<uint8_t>{ 
{0, "0 deg"},
{2, "180 deg"}
}};
ReadWrite<802, 0, UIntegerT<8>> dis_b{this, iolink::map_t<uint8_t>{ 
{0, "OFF"},                                                                             {1, "On"}                                                                            }};
ReadWrite<2000, 0, UIntegerT<8>> transmitter_configuration{this, iolink::map_t<uint8_t>{ 
{0, "Off / Off"},
{1, "On / On"},
{2, "OFF_ExtActive / Off by external signal active"},
{3, "On_ExtActive / On by external signal active"}
};
ReadWrite<2005, 0, UIntegerT<16>> rate{this, 1_ui16, 33_ui16};
Read<2008, 0, UIntegerT<16>> rep_r{this, 0_ui16, 9999_ui16};
ReadWrite<2010, 0, IntegerT<16>> fsp1{this, int16_t(200), int16_t(9999)};
ReadWrite<2011, 0, IntegerT<16>> nsp1{this, int16_t(200), int16_t(9999)};
ReadWrite<2020, 0, IntegerT<16>> fsp2{this, int16_t(200), int16_t(9999)};
ReadWrite<2021, 0, IntegerT<16>> nsp2{this, int16_t(200), int16_t(9999)};
```

Your new driver is fully implemented. You can see the full source code here `src/driver/device/ifm/o1d105/o1d105.h`.
