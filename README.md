# Mewa

Mewa is a new node-graph based compositor, dedicated to simplifying the chaotic world of motion graphics for artists. Download it at <https://www.mewatools.com/>

## Why another compositor?

Mewa is a new take on compositing software, focused on:

- Fast previews in full resolution
- Imediate preview, everywhere
- More intuitive UX/UI
- Extensibility and customizability

## Features

- Scripting and GLSL support

## Developing Mewa

Mewa's source code is organized into modules so that each module can be tested and developed independently from the whole application.

Each module can be worked on as a self contained Qt application. Current modules and respective Qt application are as follows:

| Module | Location | Notes |
|--------|----------| ----- |
| Color Wheel | [src/gui/colorselector/test_colorwheel/](src/gui/colorselector/test_colorwheel/) | N/A |
| Store Buttons | [src/storewindow/test_storebuttons/](src/storewindow/test_storebuttons/) | **1)** This app renders only the Store buttons (currently without text). **2)** Button rendering code is within `drawButtons()` function in file [src/storewindow/mxstoreview.cpp](src/storewindow/mxstoreview.cpp). **3)** The Store buttons Look&Feel still needs work: [store buttons discussion](https://github.com/Mewatools/mewa-artwork/discussions/5) |


[![mewa](https://snapcraft.io/mewa/badge.svg)](https://snapcraft.io/mewa)
[![mewa](https://snapcraft.io/mewa/trending.svg?name=0)](https://snapcraft.io/mewa)
