/****************************************************************************
** Copyright (C) 2020-2022 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxlineedit.h"
#include "mxmouseevent.h"
#include "mxfont.h"
#include "mxeventhandler.h"
#include "mxapplication.h"
#include "mxpainter.h"
#include "mxthemecolors.h"



#define MXLINEEDIT_BORDER_LEFT 3.0f




/*!
    \class MxLineEdit
    \brief Widget for editing one-line of text.

    \TODO context menu (invoked by a right mouse click) with copy&paste options

    \TODO support the following key bindings:
    \target desc
    \table
    \header \i Keypress \i Action
    \row \i Left Arrow \i Moves the cursor one character to the left.
    \row \i Shift+Left Arrow \i Moves and selects text one character to the left.
    \row \i Right Arrow \i Moves the cursor one character to the right.
    \row \i Shift+Right Arrow \i Moves and selects text one character to the right.
    \row \i Home \i Moves the cursor to the beginning of the line.
    \row \i End \i Moves the cursor to the end of the line.
    \row \i Backspace \i Deletes the character to the left of the cursor.
    \row \i Ctrl+Backspace \i Deletes the word to the left of the cursor.
    \row \i Delete \i Deletes the character to the right of the cursor.
    \row \i Ctrl+Delete \i Deletes the word to the right of the cursor.
    \row \i Ctrl+A \i Select all.
    \row \i Ctrl+C \i Copies the selected text to the clipboard.
    \row \i Ctrl+Insert \i Copies the selected text to the clipboard.
    \row \i Ctrl+K \i Deletes to the end of the line.
    \row \i Ctrl+V \i Pastes the clipboard text into line edit.
    \row \i Shift+Insert \i Pastes the clipboard text into line edit.
    \row \i Ctrl+X \i Deletes the selected text and copies it to the clipboard.
    \row \i Shift+Delete \i Deletes the selected text and copies it to the clipboard.
    \row \i Ctrl+Z \i Undoes the last operation.
    \row \i Ctrl+Y \i Redoes the last undone operation.
    \endtable

    Any other key sequence that represents a valid character, will
    cause the character to be inserted into the line edit.

*/
MxLineEdit::MxLineEdit()
{


    pReadOnly = false;


    pTextChangedEventReceiver = NULL;

    pTextChanged = true;

    pGlyphPositions.reserve(32);
    pCursorIndex = 0;
}


MxLineEdit::~MxLineEdit()
{
}

/*!
    Returns a recommended size for the MxLineEdit.

    The width returned, in pixels, is a negative value. The negative sign indicates the
widget can grow above its minimum size. The minimum size is the absolute value.

    \sa MxWidget
*/
MxVector2F MxLineEdit::sizePolicy()
{
// minimum width enough for 20 characters
    const MxFont *f = MxWidget::application()->pPainterBuffer.font();
    float w = 20 * f->metric( MxFont::SpaceAdvance );
    float h = f->metric( MxFont::Height );
    MxVector2F policy(-w, h + 6.0f);
    return policy;
}

MxString &MxLineEdit::text()
{
    return pText;
}

void MxLineEdit::setReadOnly( bool readOnly )
{
    pReadOnly = readOnly;
}


void MxLineEdit::setTextChangedReceiver(MxEventHandler *receiver, int msg )
{
    pTextChangedEventReceiver = receiver;
    pTextChangedMsg = msg;
}



void MxLineEdit::paint( MxPainter &painter )
{

    MxRectF area(size());

    const MxVector4UC contourColor(51, 82, 132, 255);
    const MxVector4UC fillColor( 37, 48, 67, 255);
     MxVectorDraw &lbTriangles = painter.vectorDraw();

     lbTriangles.fillRect( area, contourColor );
    MxRectF contourRect = area;

     contourRect.adjust( 1.0f, -1.0f, 1.0f, -1.0f );

 lbTriangles.fillRect( contourRect, fillColor );

    drawText(painter);



}


void MxLineEdit::mousePressEvent( MxMouseEvent &event )
{
    // calc cursor position
    pCursorIndex = updateCursorPosition( event.pos().x() );
    update();

    event.accept( this );
}


void MxLineEdit::drawText(MxPainter &painter)
{
    if( ! pText.isNull() )
    {
        MxRectF area(size());
        MxTextDraw &whiteTextArray = painter.textDraw();
        const int textHeight = whiteTextArray.pFont->metric( MxStaticFont::Ascender ) + painter.font()->metric( MxStaticFont::Descender );
        float centeredY = (area.height() - (float)textHeight) / 2.0f;
        whiteTextArray.drawElidedLineText( pText, MxVector2F(MXLINEEDIT_BORDER_LEFT, centeredY), MxThemeColors::whiteText, area.right() );
    }
}


void MxLineEdit::updateCursorPosArray()
{
    int count = pText.size();

    if(pTextChanged)
    {
        // update widths array
        pGlyphPositions.reserve(count+1);
        pGlyphPositions.resize(count+1);

         const MxFont *f = MxWidget::application()->pPainterBuffer.font();
        float currX = MXLINEEDIT_BORDER_LEFT;
        for(int i=0; i<count; ++i)
        {
            unsigned short unicode = pText[i];
            if( unicode == ' ')
            {
                currX += f->metric( MxFont::SpaceAdvance );
            }
            const MxFont::CharDetails &charInfo = f->charDetails( unicode );
            pGlyphPositions[i] = (int)currX + charInfo.xOffset;
            currX += charInfo.xAdvance;
        }
        pGlyphPositions[count] = currX;

        pTextChanged = false;
    }
}

int MxLineEdit::updateCursorPosition( float mouseX )
{
    int count = pText.size();
    updateCursorPosArray();

    int cursorIndex=0;
    float cursorPos = 0.0f;
    float previousDistance = 999999.0f; // very large number
    count++;
    do {


        cursorPos = pGlyphPositions[cursorIndex];
        float distance = abs( (int)(mouseX - cursorPos));

        if( distance < previousDistance )
        {
            previousDistance = distance;
        }
        else
        {

            break;
        }

        cursorIndex++;

    } while( cursorIndex < count );

    cursorIndex--;

    return cursorIndex;
}




