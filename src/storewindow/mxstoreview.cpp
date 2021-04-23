/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#include "mxstoreview.h"
#include "mxvectordraw.h"
#include "mxpainter.h"
#include "mxstoreaddon.h"
#include "plusminussvg.h"
#include "heartsvg.h"
#include "downarrowsvg.h"
#include "mxthemecolors.h"


MxStoreView::MxStoreView()
{
    // \TODO set it based on text size
    pButtonHeight = 25.0f;
}

MxStoreView::~MxStoreView()
{
}


void MxStoreView::drawButtons(const MxRectF &rect, MxPainter &painter, const MxStoreAddon &entry , int buttonUnderMouse, bool isMousePressed )
{
    // button rendering happens here
    // \TODO set nice colors
    // \TODO decide for buttons with rounded corners or squared corners
    // \TODO set button border thickness and color

    MxVectorDraw &geometryPainter = painter.vectorDraw();
    MxTextDraw &textPainter = painter.textDraw();

    MxRectF buttonsRects[3];
    calcButtonsRects( rect, buttonsRects );
    const float radius = 8.0f;
    MxVector4UC contourColor(91, 94, 99, 255);

    const float outerRadius = 8.0f;
    const float innerRadius = 6.0f;
    const float border = 1.0f;


    MxVector4UC fillColor;
    if( entry.isInstalled() ) {
        fillColor = MxVector4UC(157, 57, 57, 255); // red
    } else {
        fillColor = MxVector4UC(35, 112, 42, 255); // green
    }
    MxRectF buttonRect = buttonsRects[InstallButton];



    const float textInButtonsW = 78.0f;
    const float currW = buttonRect.width();
    bool showIconsOnly = currW < textInButtonsW;



    if( entry.isInstalling() ) // draw progress cursor
    {
        geometryPainter.roundedRect( buttonRect, outerRadius, contourColor);
        buttonRect.adjust(border, -border, border, -border);
        geometryPainter.roundedRect( buttonRect, innerRadius, MxThemeColors::windowBackground );

        MxVector2F cursorPos = buttonsRects[InstallButton].center();
        geometryPainter.loadingSymbol( cursorPos, buttonsRects[InstallButton].height() / 2.0f );
        update();
    }
    else
    {


        if( buttonUnderMouse == MxStoreView::InstallButton )
        {
            if( isMousePressed ) {
                MxVector4UC colors[4];
                /* colors[3] = MxVector4UC(80, 157, 80, 255);
                colors[2] = MxVector4UC(80, 194, 80, 255);
                colors[1] = MxVector4UC(80, 242, 80, 255);
                colors[0] = MxVector4UC(80, 244, 80, 255);*/
                colors[0] = MxVector4UC(35, 112, 42, 255);
                colors[1] = MxVector4UC(43, 141, 56, 255);
                colors[2] = MxVector4UC(48, 149, 55, 255);
                colors[3] = MxVector4UC(52, 169, 64, 255);
                geometryPainter.roundedRectV4Gradient( buttonsRects[InstallButton], radius, colors  );
            } else { // hover
                MxVector4UC colors[4];
                colors[3] = MxVector4UC(157, 157, 157, 255);
                colors[2] = MxVector4UC(194, 194, 194, 255);
                colors[1] = MxVector4UC(242, 242, 242, 255);
                colors[0] = MxVector4UC(244, 244, 244, 255);
                geometryPainter.roundedRectV4Gradient( buttonsRects[InstallButton], radius, colors  );
            }
        }
        else { // rest state


            MxRectF iconRect = buttonRect;

            if( showIconsOnly )
            {
                // center icon
                float centerX = iconRect.left() + iconRect.width() / 2.0f;
                float iconHalfW = iconRect.height() / 2.0f;
                iconRect.setLeft( centerX - iconHalfW );
                iconRect.setRight( centerX + iconHalfW );


            } else {
                iconRect.setLeft( iconRect.right() - iconRect.height() );
                //const float halfBorder = border / 2.0f;
                //iconRect.adjust(0, -8, halfBorder, -halfBorder );
            }


            iconRect.adjust(radius, -radius, radius, -radius );
            geometryPainter.roundedRect( buttonRect, outerRadius, contourColor);
            buttonRect.adjust(border, -border, border, -border);
            geometryPainter.roundedRect( buttonRect, innerRadius, fillColor);



            if( entry.isInstalled() )
            {
                // draw uninstall button
                PlusMinusSvg svg( PlusMinusSvg::MinusSymbol );
                painter.drawSvg( &svg , iconRect);

                if( ! showIconsOnly ) {
                    MxRectF textRect = buttonRect;
                    textRect.setRight( iconRect.left() );
                    const char *uninstallStr = "Uninstall";
                    textPainter.drawTextCentered(uninstallStr, textRect, MxThemeColors::whiteText );
                }
            }
            else
            {
                PlusMinusSvg svg( PlusMinusSvg::PlusSymbol );
                painter.drawSvg( &svg , iconRect);
                if( ! showIconsOnly ) {
                    MxRectF textRect = buttonRect;
                    textRect.setRight( iconRect.left() );
                    const char *installStr = "Install";
                    textPainter.drawTextCentered(installStr, textRect, MxThemeColors::whiteText);
                }
            }
        }
    }



    buttonRect = buttonsRects[UpdateButton];
    MxVector4UC updateColor(51, 110, 167, 255);

    // update button
    if( entry.hasUpdateAvailable() )
    {

        MxRectF iconRect = buttonsRects[UpdateButton];
        if( showIconsOnly )
        {
            // center icon
            float centerX = iconRect.left() + iconRect.width() / 2.0f;
            float iconHalfW = iconRect.height() / 2.0f;
            iconRect.setLeft( centerX - iconHalfW );
            iconRect.setRight( centerX + iconHalfW );

        } else {
            iconRect.setLeft( iconRect.right() - iconRect.height() );
        }

        if( buttonUnderMouse == MxStoreView::UpdateButton ) {
            if( isMousePressed ) {
                MxVector4UC colors[4];
                colors[3] = MxVector4UC(80, 157, 80, 255);
                colors[2] = MxVector4UC(80, 194, 80, 255);
                colors[1] = MxVector4UC(80, 242, 80, 255);
                colors[0] = MxVector4UC(80, 244, 80, 255);
                geometryPainter.roundedRectV4Gradient( buttonsRects[UpdateButton], radius, colors  );
            } else { // hover
                MxVector4UC colors[4];
                /*colors[3] = MxVector4UC(157, 157, 157, 255);
                colors[2] = MxVector4UC(194, 194, 194, 255);
                colors[1] = MxVector4UC(242, 242, 242, 255);
                colors[0] = MxVector4UC(244, 244, 244, 255);*/
                colors[0] = MxVector4UC(47, 28, 110, 255);
                colors[1] = MxVector4UC(64, 45, 135, 255);
                colors[2] = MxVector4UC(74, 49, 168, 255);
                colors[3] = MxVector4UC(76, 51, 168, 255);
                geometryPainter.roundedRectV4Gradient( buttonsRects[UpdateButton], radius, colors  );
            }
        } else {
            /*MxVector4UC colors[4];
            colors[0] = MxVector4UC(47, 28, 110, 255);
            colors[1] = MxVector4UC(64, 45, 135, 255);
            colors[2] = MxVector4UC(74, 49, 168, 255);
            colors[3] = MxVector4UC(76, 51, 168, 255);

            geometryPainter.roundedRectV4Gradient( buttonsRects[UpdateButton], radius, colors  );*/
            geometryPainter.roundedRect( buttonRect, outerRadius, contourColor);
            buttonRect.adjust(border, -border, border, -border);
            geometryPainter.roundedRect( buttonRect, innerRadius, updateColor);




            iconRect.adjust(radius, -radius, radius, -radius );
            DownArrowSvg svg;
            painter.drawSvg( &svg , iconRect);

            if( ! showIconsOnly ) {
                MxRectF textRect = buttonRect;
                textRect.setRight( iconRect.left() );
                const char *updateStr = "Update";
                textPainter.drawTextCentered(updateStr, textRect, MxThemeColors::whiteText);
            }
        }
    }







    MxRectF &likeButtonRect = buttonsRects[LikeButton];
    geometryPainter.roundedRect( likeButtonRect, outerRadius, contourColor);
    likeButtonRect.adjust(border, -border, border, -border);
    fillColor = MxThemeColors::windowBackground;
    geometryPainter.roundedRect( likeButtonRect, innerRadius, fillColor);


    // draw like button
    if( entry.isUpdatingScore() ) // draw progress cursor
    {
        MxVector2F cursorPos = buttonsRects[LikeButton].center();
        geometryPainter.loadingSymbol( cursorPos, buttonsRects[LikeButton].height() / 2.0f );
        update();
    }
    else if( buttonUnderMouse == MxStoreView::LikeButton )
    {
        //fillColor = MxVector4UC(60, 20, 200, 255);
    } else {
        MxRectF heartRect = buttonsRects[LikeButton];
        float middleX = heartRect.center().x();

        float heartSize = heartRect.height();
        float halfSize = heartSize / 2.0f;
        heartRect.setLeft(middleX - halfSize);
        heartRect.setRight(middleX + halfSize);
        const float iconMarginY = 4.0f;
        heartRect.adjust(0, 0, iconMarginY, -iconMarginY);
        float textY = heartRect.bottom();


        MxVector4UC heartFill;
        if( entry.userLike() ) {
            heartFill = MxVector4UC(200, 60, 60, 255);
        } else {
            heartFill = MxThemeColors::whiteText;
        }
        HeartSvg heart( heartFill );
        painter.drawSvg( &heart , heartRect);

        // draw number of likes
        // \TODO optimize ???
        char valueStr[8];
        sprintf(valueStr, "%d", entry.totalLikes() );

        //textPainter.drawText( valueStr, MxVector2F(heartRect.right(), textY, MxThemeColors::whiteText ) );
    }


}

void MxStoreView::calcButtonsRects( const MxRectF &rowRect, MxRectF buttonRects[3] )
{
    // detail state depends on width
    buttonRects[2].setToNull();
    buttonRects[1].setToNull();


    const float buttonH = buttonHeight();
    const float minButtonWidth = buttonH;
    float buttonsWidthSum = rowRect.width() - (3.0f*(margin()));
    int buttonCount = (int)( buttonsWidthSum / minButtonWidth);
    if( buttonCount > 3 ) {
        buttonCount = 3;
    }
    Q_ASSERT( buttonCount >= 1 );

    float buttonW = buttonsWidthSum / buttonCount;
    float buttonBottom = rowRect.bottom() + margin();
    float buttonTop = buttonBottom + buttonH;
    float buttonRight = rowRect.right() - margin();
    float buttonLeft = buttonRight - buttonW;
    //const float halfMargin = IMAGE_MARGIN / 2.0f;
    for(int i=buttonCount-1; i>=0; --i) // start with buttons from the left
    {
        buttonRects[i] = MxRectF(buttonLeft , buttonRight, buttonBottom, buttonTop);
        buttonRight -= (buttonW + margin());
        buttonLeft -= (buttonW + margin());
    }
}

float MxStoreView::buttonHeight() const
{
    return pButtonHeight;
}

float MxStoreView::margin() const
{
    return 8.0f;
}
