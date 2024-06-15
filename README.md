# COMP5530M - Simulation of the Dynamic Evolution of Limit Order Books using Generative Models
[![Video](/cxx/resources/images/thumbnail.jpeg)](https://www.dropbox.com/scl/fo/1dc8t7hktxg9h6v6jl3lu/ANMTkkjoqj_jAZXRig1AHFI?dl=0&e=2&preview=VIDEO_GROUP_4.mp4&rlkey=go1hqbycycbn66nmrk3owvm3y&st=8k0egpba)

This project was a collaborative effort in which
we created models to create synthetic Limit Order Books (LOB),
using Generative Adversarial Networks and Agent-Based Models.

A monolithic software application was created using C++ and Qt allowing users to backtest
their strategies on synthetic market data which attempts to simulate a Level 2 LOB. The application supports 
[Immediate or Cancel (IOC)](https://www.investor.gov/introduction-investing/investing-basics/glossary/immediate-or-cancel-order)
and [Fill or Kill (FOK) orders](https://www.investor.gov/introduction-investing/investing-basics/glossary/fill-or-kill-order)

## Prerequisites
Ensure that you have the necessary software/libraries installed to run the project. Most of the application
development was on macOS and UNIX:
+ [CMake 3.18](https://cmake.org/download/)
+ [sqlite3](https://cmake.org/cmake/help/latest/module/FindSQLite3.html)
+ [Qt](https://www.qt.io/download)
+ [pytorch/libtorch](https://pytorch.org/)
+ [httplib](https://github.com/yhirose/cpp-httplib/)
+ [nlohmann](https://github.com/nlohmann/json)

## Running the Application
From the [project root](/):
```bash
~ cd "cpp codebase"
~ mkdir build && cd build
~ cmake ../.
~ cmake --build .
```

### Data Collection
* The python files in the [CloudDataRetriever](/CloudDataRetriever) directory contains the files used to
  retrieve orderbook data from Coinbase for training the GANs.

### GAN training
* The python notebooks for GAN training can be found in the [GAN ](/GANs) directory. Once we finished training models
after a suitable number of epochs elapsed, we created .pt files and exported this to C++ to be used to load the model weights.

### Models
* The C++ implementation of the models to generate synthetic LOB data can be found in the [GAN](/cpp%20codebase/Logic/GAN) 
and [ABM](/cpp%20codebase/Logic/ABM) directories. 

### Application Workflow
![Application](/cxx/resources/images/application.png)
There are various workflows in the application:
* [A Server thread](/cpp%20codebase/Logic/API/API.cpp) which listens for API requests, processing them and updating the 
stored orderbook
* An Orderbook mutex ensuring that only one thread may modify the Orderbook at any one time, preventing race-conditions 
and ensure consistency
* [GAN thread](/cpp%20codebase/Logic/GAN/GAN.cpp) which infinitely generates LOB data when a signal is triggered from the
Qt application
* [Agent-Based thread](/cpp%20codebase/Logic/GAN/GAN.cpp) which infinitely generates LOB data when a signal is triggered
from the Qt application
* [Signals and Slots](cpp%20codebase/UI/OrderBookWidget/OrderBookWidget.cpp) which updates the user's total position &
quantity when they submit orders to the [API server](/cpp%20codebase/Logic/API/API.cpp).

### Other resources
* [Video, Report & Poster](https://www.dropbox.com/scl/fo/1dc8t7hktxg9h6v6jl3lu/ANMTkkjoqj_jAZXRig1AHFI?rlkey=go1hqbycycbn66nmrk3owvm3y&e=1&st=8k0egpba&dl=0)
* [Dataset](https://drive.google.com/file/d/1qQs-ymTzi8ufn4_YhRefNv0qW2z1Hjz6/view?usp=drive_link)
* [Linktree](https://linktr.ee/comp5530m_uolstockexchange)