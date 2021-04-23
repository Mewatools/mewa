## what is mewa
Mewa is a new node-graph based compositor.

## why another compositor
Mewa is a new take on compositing software that aims to:
- Deliver higher performance
- More intuitive UX/UI
- Easier to extend/customize through scripting

## source code
Mewa source is organized into modules so that each module can be tested and developed independently from the whole application.

Each module can be worked on as a self contained Qt application. Current modules and respective Qt application:
- **Color Wheel**: [src/gui/colorselector/test_colorwheel/](src/gui/colorselector/test_colorwheel/)
- **Store buttons**: [src/storewindow/test_storebuttons/](src/storewindow/test_storebuttons/)
  - This app renders only the Store buttons (currently without text)
  - Button rendering code is within *drawButtons()* function in file [src/storewindow/mxstoreview.cpp](src/storewindow/mxstoreview.cpp) 
  - The Store buttons Look&Feel still needs work : [store buttons discussion](https://github.com/Mewatools/mewa-artwork/discussions/5)
