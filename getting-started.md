\page starter_page Getting started

# Basic widget example

Compile and run the Qt project [examples/canvaswidget/canvaswidget.pro](examples/canvaswidget/canvaswidget.pro)

![CanvasWidget example](doc/images/canvasExample.png)

The code of interest is in the function paint(). A copy of the CanvasWidget paint() implementation is below:

\code
void CanvasWidget::paint( MxPainter &painter )
{
    const MxVector2F &areaSize = size(); // get window size
    MxRectF areaRect(areaSize);
    MxVectorDraw &paint = painter.vectorDraw(); // get SVG painter

    MxVector4UC bgColor(80, 80, 80, 255);
    MxVector4UC itemColor;
    if( pIsHovering ) { // pick square color
        itemColor = MxVector4UC(180, 180, 20, 255);
    } else {
        itemColor = MxVector4UC(160, 160, 220, 255);
    }

    paint.fillRect( areaRect, bgColor); // fill window with background color
    paint.fillRect( pDraggingRect, itemColor); // draw square
}
\endcode

**Draw with Mewa triangles**

Now lets draw a mewa triangle.

To draw Mewa triangles we call MxVectorDraw::triangle().

\code
void CanvasWidget::paint( MxPainter &painter )
{
    const MxVector2F &areaSize = size(); // get window size
    MxRectF areaRect(areaSize);
    MxVectorDraw &paint = painter.vectorDraw(); // get SVG painter

    MxVector4UC bgColor(80, 80, 80, 255);
    MxVector4UC itemColor;
    if( pIsHovering ) { // pick square color
        itemColor = MxVector4UC(180, 180, 20, 255);
    } else {
        itemColor = MxVector4UC(160, 160, 220, 255);
    }

    paint.fillRect( areaRect, bgColor); // fill window with background color
    
    // draw Mewa triangle
    MxVector2F a(20.0f, 20.0f);
    MxVector2F b(180.0f, 180.0f);
    MxVector2F c(300.0f, 20.0f);
    paint.triangle( a, itemColor,
                    b, itemColor,
                    c, itemColor, 
                    MxVectorDraw::InnerCurveFill );
}
\endcode
