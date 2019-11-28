# <u>libiolink</u>

---

# <u>Description</u>

C++17 header only, driver library for manipulating **IO-Link** devices and masters. It is targeting two categories of devices: 

- All the [IFM's IO-Link Masters](https://www.ifm.com/us/en/category/055/055_010)

- All the **IO-Link** compatible devices regardless of the manufacturer.

The library not only allows you to use the available drivers, but also gives you the tools to implement them by yourself. Writing a driver for an IFM's master or an **IO-Link** compatible devices from scratch can be done in just a few minutes.

 It is making heavy usage of C++ template system which  gives big performance benefit.  The header only approach greatly simplifies the process of including it into other projects. The minimum C++ standard is [ISO/IEC 14882](https://en.wikipedia.org/wiki/ISO/IEC_14882) (C++17).

It is intended to be used in a several different ways:

- Use available drivers for IFM masters

- Use available drivers for **IO-Link** devices

- Write you own driver for manipulating an IFM master

- Write you own driver for manipulating IO-Link device

- A mixture of all the above

The project is hosted on GitHub [https://github.com/ekondayan/libiolink](https://github.com/ekondayan/libiolink).

# <u>Dependencies</u>

libiolink uses Niels Lohmann's JSON library which is hosetd on GitHub

https://github.com/nlohmann/json to parse requests and responses from/to the

master. It is already included in the project so no action is required on your

side. This is the only dependency and the rest is plain C++17.

# <u>Usage</u>

## Override the GET and POST methods of `InterfaceComm`

The library does not provide a network layer to execute **POST** and **GET** requests to the master, so it needs to be implement separately. This is easily done by inheriting the `iolink::iot::InterfaceComm` and overriding the two virtual methods:

```
std::string_t httpGet(const iolink::string_t &adr) const override;
std::string_t httpPost(const iolink::string_t &json) const override;
```

Those two methods do one simple task - to execute a **POST** and a **GET** requests on

the master and return its raw response. This design decision was made for two reasons:

- There is a plethora of network libraries out there and  there is no reason to duplicate their functionality

- You may want  to use one of your choice

- You may wish to have more control over the execution of **GET** and **POST**.
  For example if you want to track the execution time or to count the requests

## Instantiate a driver for the master

A code snippet worth a thousand words.

```cpp
al1352::Device al1352(std::make_unique<Comm>("192.168.1.30"));
```

Here `Comm` is your class that inherits form `iolink::iot::InterfaceComm`. This is

all you need to have full controll over the master. 

## Instantiate a driver for the device

To read and write the **IO-Link** devices connected to the master, a device driver must be attached to a port on the master itself. This may sound complicate but it boils down to a single line of code ;)

```cpp
auto o1d105_w = al1352.iolinkmaster.port3.iolinkdevice.driverAttach<O1D105>();
```

This will instantiate a driver and will also attach it to a port on the master in this example `port3`. The variable `o1d105_w` is a weak pointer to `O1D105` driver. To access the devices you have to cast the pointer to a shared pointer by calling `o1d105_w.lock()` on the weak pointer:

```cpp
auto o1d105_drv = o1d105_w.lock();
```

Example:

```cpp
if(auto o1d105_drv = o1d105_w.lock())
  auto power_cycles = o1d105_drv->power_cycles.read();
```

That's it. Here comes the ...

## Full blown example

For simplicity I will use the QT's library network module, but you can use whatever you like.  Some good libraries are Qt, [Boost](https://www.boost.org/), [POCO](https://pocoproject.org/), [libcurl](https://curl.haxx.se/libcurl/), and many others.

```cpp
#include <QCoreApplication>
#include <QtNetwork>
#include "iolink/src/driver/master/al1352/device.h"
#include "iolink/src/driver/device/ifm/o1d105/o1d105.h"

class Comm final: public iolink::iot::InterfaceComm
{
    public:
        explicit Comm(const iolink::string_t &ip, uint16_t port = 80):
        InterfaceComm(ip, port)
        {
        }

        iolink::string_t httpGet(const iolink::string_t &adr) const override
        {
            if(adr.empty()) return string_t{};

            QNetworkAccessManager qnam;

            QNetworkRequest request(QUrl(QString::fromStdString(urlFromAddress(adr))));

            auto *reply = qnam.get(request);

            timer.start();

            while(!reply->isFinished()) QCoreApplication::processEvents();

            if(!reply->isFinished()) return string_t{};

            auto reply_data = reply->readAll();

            return reply_data.toStdString();
        }

        iolink::string_t httpPost(const iolink::string_t &json) const override
        {
            if(json.empty()) return string_t{};

            QNetworkAccessManager qnam;

            QNetworkRequest request(QUrl(QString::fromStdString(urlFromAddress())));
            request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

            auto *reply = qnam.post(request, QByteArray::fromStdString(json));

            timer.start();

            while(!reply->isFinished()) QCoreApplication::processEvents();

            if(!reply->isFinished()) return string_t{};

            auto reply_data = reply->readAll();

            return reply_data.toStdString();
        }
};

int main(int argc, char *argv[])
{
    using namespace iolink;
    using namespace iolink::driver;
    using namespace iolink::master;

    QCoreApplication a(argc, argv);

    al1352::Device al1352(std::make_unique<Comm>("192.168.1.30"));

    auto sn = al1352.deviceinfo.serialnumber.getData();

    auto o1d105_w = al1352.iolinkmaster.port3.iolinkdevice.driverAttach<O1D105>();

    if(auto o1d105_drv = o1d105_w.lock())
    {
        auto pc = driver->power_cycles.read();
    }

    return 0;
}
```

# <u>Tutorials</u>

In the tutorial section you can find a step by step guides how to:

- create a driver for an IFM master

- create a driver for an **IO-Link** device

- simple usage of the available drivers

# <u>Download</u>

You can download the project from GitHub using this command:

git clone https://github.com/ekondayan/libiolink.git libiolink

# <u>Install</u>

In order to use libiolink, you just need to download and extract the header files into you project. In fact, the header files in the libiolink subdirectory are the

only files required to compile programs with the library. The header files

are cross platform. It is not necessary to use CMake or install

anything, just include the header files into your project.

# <u>License</u>

Copyright (c) 2019 Emil Kondayan

This Source Code Form is subject to the terms of the Mozilla Public

License, v. 2.0. If a copy of the MPL was not distributed with this

file, You can obtain one at http://mozilla.org/MPL/2.0/.

# <u>Now what?</u>

* Head to the tutorials section

* Head to the examples section

* Head to the documentation

* if you are feeling nerdy, dig into the source code :)

# <u>Credits</u>

Author: Emil Kondayan

Owner: Emil Kondayan

Maintainer: Emil Kondayan

Contributors: Emil Kondayan
