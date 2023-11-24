/**********************************************************
  This file is autogenerated.
  Change font_h.template
************************************************************/
#ifndef MXSTATICFONT_H
#define MXSTATICFONT_H


#include "mxfont.h"



class MxStaticFont : public MxFont
{
public:
    MxStaticFont()
    {pTexture = 0;}

    virtual ~MxStaticFont(){}



    virtual void loadGL( MxRenderer *renderer );
    virtual GLuint texture() const;



    virtual float metric( MetricName name ) const;
    virtual const MxFont::CharDetails& charDetails( const char32_t character ) const;
    int width( const char *str, int size ) const;
    //int widthOfNullTerminated( const char *str ) const;
    int width( const MxString &str ) const;

    static MxFont::CharDetails pCharDetailsList[94];

    GLuint pTexture;


};

#endif
