\page contrib_page Contributing

# About the Mewa framework


The Mewa framework is the framework on which the application [Mewa](https://www.mewa.app/) is being developed.

Mewa is built on C++ and OpenGL, providing high performance on desktop and mobile. 


## Directory Structure

<pre>
mewa/
├── framework/                        # Core utilities and base classes shared across modules
│   ├── containers/                   # Container classes
│   ├── scripting/                    # Scripting engine components
│   └── ...                           # Other framework-level components
├── modules/                          # All individual modules
│   ├── imagesequencenode/            # Nodegraph's Image node
│   ├── storewindow/                  # Store Window module
│   │   ├── src.pri                   # Qt project input file
│   │   ├── storewindow.cpp           # Module source file
│   │   ├── ...                       # Module source files
│   │   ├── icons/                    # Module icons
│   │   └── test/                     # Test app for the Store Window
│   │       ├── test-storewindow.pro  # Qt project file
│   │       ├── main.cpp              # Test app entry point
│   │       └── ...                   # Other test app resources
│   └── ...                           # Other modules
└── docs/                             # Documentation for the project
</pre>

The framework includes reusable classes that can be reused across modules.

Each module has a test-app with the role of verifying the module functionality.


# Modules

Mewa source is organized into modules so that each module can be tested and developed independently from the whole application.
Each module can be worked on as a self contained Qt application. Current modules and respective Qt application are as follows:


| App | Location | Notes |
|--------|----------| ----- |
| Simple Widget | [simplewidget.pro](https://github.com/Mewatools/mewa/blob/master/solutions/simplewidget/simplewidget.pro) | Example of a MxWidget drawing a rectangle. See MxWidget::paint() for more information about widget painting |
| Triangle Plugin | [test_triangle.pro](https://github.com/Mewatools/mewa/blob/master/solutions/triangleplugin/test_triangle/test_triangle.pro) | Example of a Mewa plugin |
| Color Wheel | [colorselector.pro](https://github.com/Mewatools/mewa/blob/master/src/renderer/opengl/colorwheelprogram/test_colorwheel/colorselector.pro) | OpenGL app to test the ColorWheel shader |
| Store Buttons | [test_storebuttons.pro](https://github.com/Mewatools/mewa/blob/master/solutions/storewindow/test_storebuttons/test_storebuttons.pro) | **1)** This app renders only the Store buttons (currently without text). **2)** Button rendering code is within `drawButtons()` function in file [src/storewindow/mxstoreview.cpp](src/storewindow/mxstoreview.cpp). **3)** The Store buttons Look&Feel still needs work: [store buttons discussion](https://github.com/Mewatools/mewa-artwork/discussions/5) |
| Roto Tree View | [test_rototreeview.pro](https://github.com/Mewatools/mewa/blob/master/solutions/rotonode/test_rototreeview/test_rototreeview.pro) | Test-app running roto's tree view, part of the Roto node parameters. The code that performs all the drawing is located in rotoshape.cpp, RotoShape::paint() function |

