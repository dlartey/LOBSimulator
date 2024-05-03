# COMP5530M - Simulation of the Dynamic Evolution of Limit Order Books using Generative Models
[![Video](https://uc2b8cfe85e7e67db77dad205030.previews.dropboxusercontent.com/p/thumb/ACTROnFLF0rEZA3ruuAjg1TvCFX-RqVN9-F1t3veFv0xDvaXwKz9BeitpNgszuxD8YkDGyAF4gtOsWg8oWkSc3Zh2-ASAfNtdjx9PlpRoyBpBJJv_Ec4DaKl018Ip2Rs9R_VTb8lhMtjzMxGJFIes0UgzFFSIgMfY2x9pNlYP7GGi2W1YzvX3visZHRJVcw0rYFFL9uIM1qiDq1Zcxn08OqnKpm0ODnh2F_JoatlCuDh_1_3zm8lUWESKmSFnidDfYtKJ8z6otS056sj_YWUJyXwbEd6fZH2HF3Wm9KgNCX-7wHbcInk77zIgn2-WchpUO_xic7BYOY8er4lRKH2IGLh5TUQCVhhtPSA0SitfWxF86Vf-hbHLlTPN9DtcXdyvto7qC4Pwg-3LPANcAnCuBNDvjLv40CXgRk989bJXOtku_sxX6QHItu37ynttngwN0MoUnmadoJMIFwWcBtQIoR-g4z-eYKziSGu8v_4v1WpYZ3cegqta4126igmvmS-bJVg86NHhLpGH9rW_zoFigM1y0SvtHEpsNkQUa2d4CNrHRVz320__5_gu9bmgMzneNSH0P3bVk7DTFH-PQiG7WFBP5rs-0iUUHmBBhj_D_G5Vnx6j4TvtcOe8eLj-DZmOKYpd1xkKiZWo6mkzg8HL6upua5locgYd1dLliZ2EpLFE0NdDPumF3HY0ZRheiZzJHSpqCnkQ964kPPVVQZ4zDUmV4BWCC9MRod7UiWiSNA-iAvTHls0GqNaQEw1Q5K0X1k1-IJulAkkr1T3jklw8y208Hd8uXywgHJwLI_MQfCUK-T0MYeb_JQkQu7LufZsgtTXquss1AUvBCDwVvbsiklV1Y3Z8Xfaljnc4G7lQ_ggDT8HNziuZHvwCZHv5CQknruKv0wAtT6qqY39cnFuxjQzizN7ygUW-LLe8zAZDFn7Old278dHDTsAOPeW8FJ4_2spAi6HTX9Rma2joxZIGyi4tMk4xCZ6EpgwM6TvypoWdRps7Gfyd4A6zPGvLrPp2hHoqokO_UUvEHXFda9Qk_dczRLxqtHkCD9psBWLGrt7h8gbFbmc13TH11PZaXVkEqtAuouhBK3mB2bdbaFSaMH9gOjyzA3R-me5fEAm4zEpuahqDc8XW32wiq-6S7okVq1OsrPXfNfOFbD_C9pUf2oUTVFj1a1cQR7a_vbLTYd4LCrXsZCXgsey2O8ZMDkpD_TOqdj9NJ9rH6yTk3Z1CQkS7eYlzc5j7442tsA4vSnF7vhjBoMdOAijnY38B7QB4bHXiV2dlplGBQDv5w3QbWqWqPTVVzGeKzJvZXjTGBbT7omWiwucdGYz3JffDf7xLlpoF0uLYohR_GG91R9ulD5iELrEvfTH0D8pOuj3sOPhy5jUdJbLVxhgmLLj1NxoCKnEXk95REgSNKMGqQnJKBiT1djVy74ERdOik24UrgRYiw1jhWScyiWYDlTgWqgz2JeBxcqR7ePIRDB9KUMYwA7rA-o4lm1sW_RGXhvmFjSIdS73KSW8EWzGyjVh_Ln87EJTHgZT6mTqaOnJNfaEPz7mqtF9JRu1jQbzGZWv2BDMTg/p.jpeg?size=1024x768&size_mode=2&psid=7832fa92-aa2b-4bbf-b9a1-d968ddd58605)](https://www.dropbox.com/scl/fo/1dc8t7hktxg9h6v6jl3lu/ANMTkkjoqj_jAZXRig1AHFI?dl=0&e=2&preview=VIDEO_GROUP_4.mp4&rlkey=go1hqbycycbn66nmrk3owvm3y&st=8k0egpba)

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
![img.png](/cpp%20codebase/resources/application.png)
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