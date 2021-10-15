/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef ROTOSHAPE_H
#define ROTOSHAPE_H

#include "mxlist.h"
#include "mxvector.h"
#include "rototreeitem.h"


class MxOrthoTransform;
class RotoTransform;


// A shape can have 1 point only, used by point tracker
class RotoShape : public RotoTreeItem
{
public:
    RotoShape( RotoTransform *parent );

    virtual ~RotoShape();


    void toggleInvert();
    bool isInverted() const;
    void toggleLock();
    bool isLocked() const;
    void toggleVisibility();
    bool isVisible() const;



    RotoTreeItem::ItemType itemType() const;
    virtual RotoTreeItem* siblingAtRow(int row) const;
    virtual void paint( MxPainter &painter, DrawData &drawData ) const;
    virtual bool isSelected() const;
    virtual RotoTreeItem* itemAtRow(int *rowIndex , int *transformCount);
    virtual void clearSelection();
    virtual void selectRow();

    // open or closed. Only applies to strokes
    bool isClosed() const { return pProperties & Close; };
    // fill or stroke
    bool isFilled() const { return pProperties & Filled; };
    // bool isStroke() const { return !isFilled(); };


    void endCreating() { pProperties |= Close; }


    enum Operation
    {
        OR,
        AND,
        XOR
    };
    Operation pOperation; // always OR


    enum ShapeProperties
    {
        OpenStroke = 0x0,
        Close = 0x01,
        Filled = 0x02,

        // buttons
        Locked = 0x4,
        Visible = 0x8,
        Invert = 0x10
    };
    unsigned char pProperties;

    enum CurveInterface
    {
        QuadraticInterface, // lets call it Mewa spline (not a cardinal x-spline, but almost)
        BSplineInterface,
        CubicInterface // bezier curve
    };
    CurveInterface pCurveInterface;

    enum BlendMode
    {
        Add, // Adds the object to other objects.
        Subtract, // Subtracts the object from other objects.
        Multipl, // Produces a result where there is a union of pixels from two objects.
        Difference, // Produces a result where a value exists in each object, but not in both.
        Max /* Looks at the luminance information for each object and selects the value—
      whichever is brighter—as the result. Darker pixels are replaced while brighter
        pixels do not change*/
    };

    BlendMode pBlendMode;
    MxVector4UC pColor;




    int pSelectionCount; // points from this shape currently selected

};



#endif

