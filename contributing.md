\page contrib_page Contributing

# About the Mewa framework

The Mewa framework is the framework on which the application [Mewa](https://www.mewatools.com/) is being developed.
Initially, Mewa was developed using the Qt framework, but due to specific performance and usability requirements, and as we adapted Qt to match our needs, we realized that we would greatly benefit from a specialized framework solution.

# Development

Mewa's source code contains various example apps, some of them only purpose is to test single functionalities of the Mewa application.
This approach allows us to test and develop single functionalities independently from the whole application.

Each test/example app is a self-contained Qt application located in the examples folder.

| App | Location | Notes |
|--------|----------| ----- |
| Canvas Widget | [examples/canvaswidget](examples/canvaswidget) | Example of a MxWidget drawing a rectangle. See MxWidget::paint() for more information about widget painting |
| Color Wheel | [examples/test_colorwheel/](examples/test_colorwheel/) | Test app running the ColorWheel shader |
| Store Buttons | [examples/storewindow/test_storebuttons/](examples/storewindow/test_storebuttons/) | **1)** This app renders only the Store buttons (currently without text). **2)** Button rendering code is within `drawButtons()` function in file [src/storewindow/mxstoreview.cpp](src/storewindow/mxstoreview.cpp). **3)** The Store buttons Look&Feel still needs work: [store buttons discussion](https://github.com/Mewatools/mewa-artwork/discussions/5) |
| Roto Tree View | [examples/rotonode/test_rototreeview](examples/rotonode/test_rototreeview) | Test-app running roto's tree view, part of the Roto node parameters. The code that performs all the drawing is located in rotoshape.cpp, RotoShape::paint() function |

