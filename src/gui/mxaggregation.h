/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
#ifndef MXAGGREGATION_H
#define MXAGGREGATION_H


class MxAbstractAtlas;
class MxApplication;


/*!
 * \brief The MxGuiAggregation class aggregates UI related data members to allow its access from one place
 */
class MxAggregation
{
public:
    MxAggregation();
    static MxAggregation* instance();

    /*! Returns display Dpi. Used on tablet devices only. */
    float displayScale() const;
    const MxAbstractAtlas * iconAtlas() const;
    MxApplication * application() const;

private:
    friend class MxApplication;

    ////////// aggregated data members ///////////
    MxAbstractAtlas *pIconAtlas;
    MxApplication *pApplication;

    static MxAggregation * sAggregation;

};

#endif // MXAGGREGATION_H
