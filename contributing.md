\page contrib_page Contributing

# About the Mewa framework

The Mewa framework is the framework on which the application [Mewa](https://www.mewatools.com/) is being developed.
Initially, Mewa was developed using the Qt framework, but due to specific performance and usability requirements, and as we adapted Qt to match our needs, we realized that we would greatly benefit from a specialized framework solution.

# Development

Mewa's source code contains various example apps, some of them only purpose is to test single functionalities of the Mewa application.
This approach allows us to test and develop single functionalities independently from the whole application.

Each test/example app is a self-contained Qt application.

| App | Location | Notes |
|--------|----------| ----- |
| Simple Widget | [simplewidget.pro](https://github.com/Mewatools/mewa/blob/master/solutions/simplewidget/simplewidget.pro) | Example of a MxWidget drawing a rectangle. See MxWidget::paint() for more information about widget painting |
| Triangle Plugin | [test_triangle.pro](https://github.com/Mewatools/mewa/blob/master/solutions/triangleplugin/test_triangle/test_triangle.pro) | Example of a Mewa plugin |
| Color Wheel | [colorselector.pro](https://github.com/Mewatools/mewa/blob/master/src/renderer/opengl/colorwheelprogram/test_colorwheel/colorselector.pro) | OpenGL app to test the ColorWheel shader |
| Store Buttons | [test_storebuttons.pro](https://github.com/Mewatools/mewa/blob/master/solutions/storewindow/test_storebuttons/test_storebuttons.pro) | **1)** This app renders only the Store buttons (currently without text). **2)** Button rendering code is within `drawButtons()` function in file [src/storewindow/mxstoreview.cpp](src/storewindow/mxstoreview.cpp). **3)** The Store buttons Look&Feel still needs work: [store buttons discussion](https://github.com/Mewatools/mewa-artwork/discussions/5) |
| Roto Tree View | [test_rototreeview.pro](https://github.com/Mewatools/mewa/blob/master/solutions/rotonode/test_rototreeview/test_rototreeview.pro) | Test-app running roto's tree view, part of the Roto node parameters. The code that performs all the drawing is located in rotoshape.cpp, RotoShape::paint() function |

