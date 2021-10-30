/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXIMAGE_H
#define MXIMAGE_H

#include "mxvector.h"


class MxImage
{
public:
  MxImage();
  ~MxImage();

  void alloc( const MxVector2I &size );
  void fillWithBlack();

  unsigned char* bits();
  int width() const;
  int height() const;
  const MxVector2I& size() const;
   int byteCount() const;
  unsigned char* scanLine( int i ) const;
void pasteBitmapAtPos( const MxVector2I &pos, const unsigned char *imageBits, int imageWidth, int imageHeight );

////// DATA MEMBERS /////////
  struct ImageData
  {
    MxVector2I size;
    unsigned char *data;
  };

  ImageData pImageData;


};



#endif

