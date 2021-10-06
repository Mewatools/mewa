## About the Mewa framework

The Mewa framework is the framework on which the application Mewa is being developed.
Initially, Mewa was developed using the Qt framework, but due to specific performance and usability requirements, and as we adapted Qt to match our needs, we realized that we would greatly benefit from a specialized framework solution.

## Framework Development

Mewa's source code is organized into modules so that each module can be tested and developed independently from the whole application.

Each module can be worked on as a self contained Qt application. Current modules and respective Qt application are as follows:

| Module | Location | Notes |
|--------|----------| ----- |
| Color Wheel | [src/gui/colorselector/test_colorwheel/](src/gui/colorselector/test_colorwheel/) | N/A |
| Store Buttons | [src/storewindow/test_storebuttons/](src/storewindow/test_storebuttons/) | **1)** This app renders only the Store buttons (currently without text). **2)** Button rendering code is within `drawButtons()` function in file [src/storewindow/mxstoreview.cpp](src/storewindow/mxstoreview.cpp). **3)** The Store buttons Look&Feel still needs work: [store buttons discussion](https://github.com/Mewatools/mewa-artwork/discussions/5) |

