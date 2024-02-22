
[![Discord](https://img.shields.io/discord/827792740359340103)](https://disboard.org/server/827792740359340103)

[![mewa](https://snapcraft.io/mewa/badge.svg)](https://snapcraft.io/mewa)
[![mewa](https://snapcraft.io/mewa/trending.svg?name=0)](https://snapcraft.io/mewa)

# What is Mewa

It's a software framework built on C++ and OpenGL, providing high performance on desktop and mobile. 

The framework includes reusable classes that form the foundation of Mewa applications.

## Framework

Mewa source is organized into modules so that each module can be tested and developed independently from the whole application.
Each module can be worked on as a self contained Qt application. Current modules and respective Qt application are as follows:

| Module | Location | Notes |
|--------|----------| ----- |
| Simple Widget | [simplewidget.pro](solutions/simplewidget/simplewidget.pro) | Basic widget drawing |
| Triangle Plugin | [test_triangle.pro](solutions/triangleplugin/test_triangle/test_triangle.pro) | Example of a Mewa plugin |
| Color Wheel | [colorselector.pro](src/renderer/opengl/colorwheelprogram/test_colorwheel/colorselector.pro) | N/A |
| Store Window | [test_storebuttons.pro](solutions/storewindow/test_storebuttons/test_storebuttons.pro) | **1)** This app renders only the Store buttons (currently without text). **2)** Button rendering code is within `drawButtons()` function in file [mxstoreview.cpp](solutions/storewindow/mxstoreview.cpp). **3)** The Store buttons Look&Feel still needs work: [store buttons discussion](https://github.com/Mewatools/mewa-artwork/discussions/5) |
| Roto Node Tree View | [test_rototreeview.pro](solutions/rotonode/test_rototreeview/test_rototreeview.pro) | Tree view showing the hierarchy of shapes |

To know more about the Mewa framework check the [developers page](https://mewatools.github.io/mewa/).


## Applications

Mewa applications are designed to simplify motion graphics for artists.

### Vector based Animations

With CSS dn SVG animations becoming more common through websites and mobile apps, the need for smoother and richer animations keeps growing.

Mewa was designed from the ground up to raise the bar of web animations performance. Taking full advantage of GPU, Mewa can deliver smoother animations, with an even richer content.

It's main goal is to make cross-platform animations feel faster and responsive, running richer animations on any screen size and aspect ratio.

Mewa Animations offer:
- Constant frame rate: for guaranteed smooth animations
- Offload work from CPU to GPU : for responsive user interaction
- Scale-invariant drawing : for seamless look & feel across any screen size
- Small file size : for faster startup
- javascript API : for programmable user interaction (something XML and JSON do not)
- Portable across mobile, desktop and web

[![video showing how to create a shape animation](https://img.youtube.com/vi/PUa01c3YIYY/0.jpg)](https://www.youtube.com/watch?v=PUa01c3YIYY)

### Video compositing

With an underlying node-graph interface, Mewa aims to offer compositing capabilities with:

- Faster/Imediate previews, everywhere
- Previews in full resolution, always
- More intuitive user interface
- Dedicated script engine for:
  - Faster execution of expressions/scripts
  - Guaranteed UI responsiveness
  - Easier extensibility and customizability

[![video showing compositing app](https://img.youtube.com/vi/re_owHkXqW0/1.jpg)](https://www.youtube.com/watch?v=re_owHkXqW0)

### Other Screenshots

![mewa-draw-singleapp](https://user-images.githubusercontent.com/8775959/189542488-ed60a32b-fcdf-473b-b77b-3ac0d48d7a4d.png)

![mewa-draw-nodegraph](https://user-images.githubusercontent.com/8775959/189543145-f6ebf97f-510e-41a7-b9e7-daa0a0cb4c3a.png)


