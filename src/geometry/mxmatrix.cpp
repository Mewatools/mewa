/****************************************************************************
** Copyright (C) 2020-2025 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
/****************************************************************************
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
** SPDX-License-Identifier: Qt commercial license
****************************************************************************/
#include "mxmatrix.h"
#include <string.h> // for memcpy

#include "mxfuzzy.h"
#include "mxgeometryutils.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif


MxMatrix::MxMatrix(const float* array)
{
  memcpy(data(), array, 4*4*sizeof(float));
  flagBits = General;
}

float MxMatrix::m11 () const
{
    return m[0][0];
}

float MxMatrix::m22 () const
{
    return m[1][1];
}

float MxMatrix::dx() const
{
    return m[3][0];
}

float MxMatrix::dy() const
{
    return m[3][1];
}

void MxMatrix::setToIdentity()
{
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
  flagBits = Identity;
}





static inline double matrixDet2(const double m[4][4], int col0, int col1, int row0, int row1)
{
    return m[col0][row0] * m[col1][row1] - m[col0][row1] * m[col1][row0];
}


// The 4x4 matrix inverse algorithm is based on that described at:
// http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q24
// Some optimization has been done to avoid making copies of 3x3
// sub-matrices and to unroll the loops.

// Calculate the determinant of a 3x3 sub-matrix.
//     | A B C |
// M = | D E F |   det(M) = A * (EI - HF) - B * (DI - GF) + C * (DH - GE)
//     | G H I |
static inline double matrixDet3
    (const double m[4][4], int col0, int col1, int col2,
     int row0, int row1, int row2)
{
    return m[col0][row0] * matrixDet2(m, col1, col2, row1, row2)
            - m[col1][row0] * matrixDet2(m, col0, col2, row1, row2)
            + m[col2][row0] * matrixDet2(m, col0, col1, row1, row2);
}


// Calculate the determinant of a 4x4 matrix.
static inline double matrixDet4(const double m[4][4])
{
    double det;
    det  = m[0][0] * matrixDet3(m, 1, 2, 3, 1, 2, 3);
    det -= m[1][0] * matrixDet3(m, 0, 2, 3, 1, 2, 3);
    det += m[2][0] * matrixDet3(m, 0, 1, 3, 1, 2, 3);
    det -= m[3][0] * matrixDet3(m, 0, 1, 2, 1, 2, 3);
    return det;
}


static inline void copyToDoubles(const float m[4][4], double mm[4][4])
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            mm[i][j] = double(m[i][j]);
}


/*!
    Returns the inverse of this matrix.  Returns the identity if
    this matrix cannot be inverted; i.e. determinant() is zero.
    If \a invertible is not null, then true will be written to
    that location if the matrix can be inverted; false otherwise.

    If the matrix is recognized as the identity or an orthonormal
    matrix, then this function will quickly invert the matrix
    using optimized routines.

    \sa determinant(), normalMatrix()
*/
MxMatrix MxMatrix::inverted(bool *invertible) const
{
    // Handle some of the easy cases first.
    if (flagBits == Identity) {
        if (invertible)
            *invertible = true;
        return MxMatrix();
    } else if (flagBits == Translation) {
        MxMatrix inv;
        inv.m[3][0] = -m[3][0];
        inv.m[3][1] = -m[3][1];
        inv.m[3][2] = -m[3][2];
        inv.flagBits = Translation;
        if (invertible)
            *invertible = true;
        return inv;
    } else if (flagBits < Rotation2D) {
        // Translation | Scale
        if (m[0][0] == 0 || m[1][1] == 0 || m[2][2] == 0) {
            if (invertible)
                *invertible = false;
            return MxMatrix();
        }
        MxMatrix inv;
        inv.m[0][0] = 1.0f / m[0][0];
        inv.m[1][1] = 1.0f / m[1][1];
        inv.m[2][2] = 1.0f / m[2][2];
        inv.m[3][0] = -m[3][0] * inv.m[0][0];
        inv.m[3][1] = -m[3][1] * inv.m[1][1];
        inv.m[3][2] = -m[3][2] * inv.m[2][2];
        inv.flagBits = flagBits;

        if (invertible)
            *invertible = true;
        return inv;
    } else if ((flagBits & ~(Translation | Rotation2D | Rotation)) == Identity) {
        if (invertible)
            *invertible = true;
        return orthonormalInverse();
    } else if (flagBits < Perspective) {
        MxMatrix inv; // The "1" says to not load the identity.

        double mm[4][4];
        copyToDoubles(m, mm);

        double det = matrixDet3(mm, 0, 1, 2, 0, 1, 2);
        if (det == 0.0f) {
            if (invertible)
                *invertible = false;
            return MxMatrix();
        }
        det = 1.0f / det;

        inv.m[0][0] =  matrixDet2(mm, 1, 2, 1, 2) * det;
        inv.m[0][1] = -matrixDet2(mm, 0, 2, 1, 2) * det;
        inv.m[0][2] =  matrixDet2(mm, 0, 1, 1, 2) * det;
        inv.m[0][3] = 0;
        inv.m[1][0] = -matrixDet2(mm, 1, 2, 0, 2) * det;
        inv.m[1][1] =  matrixDet2(mm, 0, 2, 0, 2) * det;
        inv.m[1][2] = -matrixDet2(mm, 0, 1, 0, 2) * det;
        inv.m[1][3] = 0;
        inv.m[2][0] =  matrixDet2(mm, 1, 2, 0, 1) * det;
        inv.m[2][1] = -matrixDet2(mm, 0, 2, 0, 1) * det;
        inv.m[2][2] =  matrixDet2(mm, 0, 1, 0, 1) * det;
        inv.m[2][3] = 0;
        inv.m[3][0] = -inv.m[0][0] * m[3][0] - inv.m[1][0] * m[3][1] - inv.m[2][0] * m[3][2];
        inv.m[3][1] = -inv.m[0][1] * m[3][0] - inv.m[1][1] * m[3][1] - inv.m[2][1] * m[3][2];
        inv.m[3][2] = -inv.m[0][2] * m[3][0] - inv.m[1][2] * m[3][1] - inv.m[2][2] * m[3][2];
        inv.m[3][3] = 1;
        inv.flagBits = flagBits;

        if (invertible)
            *invertible = true;
        return inv;
    }

    MxMatrix inv; // The "1" says to not load the identity.

    double mm[4][4];
    copyToDoubles(m, mm);

    double det = matrixDet4(mm);
    if (det == 0.0f) {
        if (invertible)
            *invertible = false;
        return MxMatrix();
    }
    det = 1.0f / det;

    inv.m[0][0] =  matrixDet3(mm, 1, 2, 3, 1, 2, 3) * det;
    inv.m[0][1] = -matrixDet3(mm, 0, 2, 3, 1, 2, 3) * det;
    inv.m[0][2] =  matrixDet3(mm, 0, 1, 3, 1, 2, 3) * det;
    inv.m[0][3] = -matrixDet3(mm, 0, 1, 2, 1, 2, 3) * det;
    inv.m[1][0] = -matrixDet3(mm, 1, 2, 3, 0, 2, 3) * det;
    inv.m[1][1] =  matrixDet3(mm, 0, 2, 3, 0, 2, 3) * det;
    inv.m[1][2] = -matrixDet3(mm, 0, 1, 3, 0, 2, 3) * det;
    inv.m[1][3] =  matrixDet3(mm, 0, 1, 2, 0, 2, 3) * det;
    inv.m[2][0] =  matrixDet3(mm, 1, 2, 3, 0, 1, 3) * det;
    inv.m[2][1] = -matrixDet3(mm, 0, 2, 3, 0, 1, 3) * det;
    inv.m[2][2] =  matrixDet3(mm, 0, 1, 3, 0, 1, 3) * det;
    inv.m[2][3] = -matrixDet3(mm, 0, 1, 2, 0, 1, 3) * det;
    inv.m[3][0] = -matrixDet3(mm, 1, 2, 3, 0, 1, 2) * det;
    inv.m[3][1] =  matrixDet3(mm, 0, 2, 3, 0, 1, 2) * det;
    inv.m[3][2] = -matrixDet3(mm, 0, 1, 3, 0, 1, 2) * det;
    inv.m[3][3] =  matrixDet3(mm, 0, 1, 2, 0, 1, 2) * det;
    inv.flagBits = flagBits;

    if (invertible)
        *invertible = true;
    return inv;
}



/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    components \a x, and \a y.

    \sa translate(), rotate()
*/
void MxMatrix::scale(float x, float y)
{
    if (flagBits == Identity) {
        m[0][0] = x;
        m[1][1] = y;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= x;
        m[1][1] *= y;
    } else if (flagBits == Translation) {
        m[0][0] = x;
        m[1][1] = y;
        flagBits |= Scale;
    } else {
        m[0][0] *= x;
        m[0][1] *= x;
        m[0][2] *= x;
        m[0][3] *= x;
        m[1][0] *= y;
        m[1][1] *= y;
        m[1][2] *= y;
        m[1][3] *= y;
        flagBits = General;
    }
}

/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    components \a x, \a y, and \a z.

    \sa translate(), rotate()
*/
void MxMatrix::scale(float x, float y, float z)
{
    if (flagBits == Identity) {
        m[0][0] = x;
        m[1][1] = y;
        m[2][2] = z;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= x;
        m[1][1] *= y;
        m[2][2] *= z;
    } else if (flagBits == Translation) {
        m[0][0] = x;
        m[1][1] = y;
        m[2][2] = z;
        flagBits |= Scale;
    } else {
        m[0][0] *= x;
        m[0][1] *= x;
        m[0][2] *= x;
        m[0][3] *= x;
        m[1][0] *= y;
        m[1][1] *= y;
        m[1][2] *= y;
        m[1][3] *= y;
        m[2][0] *= z;
        m[2][1] *= z;
        m[2][2] *= z;
        m[2][3] *= z;
        flagBits = General;
    }
}

/*!
    Multiplies this matrix by another that scales coordinates by the
    given \a factor.

    \sa translate(), rotate()
*/
void MxMatrix::scale(float factor)
{
    if (flagBits < Scale) {
        m[0][0] = factor;
        m[1][1] = factor;
        m[2][2] = factor;
    } else if (flagBits < Rotation2D) {
        m[0][0] *= factor;
        m[1][1] *= factor;
        m[2][2] *= factor;
    } else if (flagBits < Rotation) {
        m[0][0] *= factor;
        m[0][1] *= factor;
        m[1][0] *= factor;
        m[1][1] *= factor;
        m[2][2] *= factor;
    } else {
        m[0][0] *= factor;
        m[0][1] *= factor;
        m[0][2] *= factor;
        m[0][3] *= factor;
        m[1][0] *= factor;
        m[1][1] *= factor;
        m[1][2] *= factor;
        m[1][3] *= factor;
        m[2][0] *= factor;
        m[2][1] *= factor;
        m[2][2] *= factor;
        m[2][3] *= factor;
    }
    flagBits |= Scale;
}

/*!
    Multiplies this matrix by another that translates coordinates
    by the components \a x, and \a y.

    \sa scale(), rotate()
*/
void MxMatrix::translate(float x, float y)
{
    if (flagBits == Identity) {
        m[3][0] = x;
        m[3][1] = y;
    } else if (flagBits == Translation) {
        m[3][0] += x;
        m[3][1] += y;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * x;
        m[3][1] = m[1][1] * y;
    } else if (flagBits == (Translation | Scale)) {
        m[3][0] += m[0][0] * x;
        m[3][1] += m[1][1] * y;
    } else if (flagBits < Rotation) {
        m[3][0] += m[0][0] * x + m[1][0] * y;
        m[3][1] += m[0][1] * x + m[1][1] * y;
    } else {
        m[3][0] += m[0][0] * x + m[1][0] * y;
        m[3][1] += m[0][1] * x + m[1][1] * y;
        m[3][2] += m[0][2] * x + m[1][2] * y;
        m[3][3] += m[0][3] * x + m[1][3] * y;
    }
    flagBits |= Translation;
}



/*!
    Multiplies this matrix by another that translates coordinates
    by the components \a x, \a y, and \a z.

    \sa scale(), rotate()
*/
void MxMatrix::translate(float x, float y, float z)
{
    if (flagBits == Identity) {
        m[3][0] = x;
        m[3][1] = y;
        m[3][2] = z;
    } else if (flagBits == Translation) {
        m[3][0] += x;
        m[3][1] += y;
        m[3][2] += z;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * x;
        m[3][1] = m[1][1] * y;
        m[3][2] = m[2][2] * z;
    } else if (flagBits == (Translation | Scale)) {
        m[3][0] += m[0][0] * x;
        m[3][1] += m[1][1] * y;
        m[3][2] += m[2][2] * z;
    } else if (flagBits < Rotation) {
        m[3][0] += m[0][0] * x + m[1][0] * y;
        m[3][1] += m[0][1] * x + m[1][1] * y;
        m[3][2] += m[2][2] * z;
    } else {
        m[3][0] += m[0][0] * x + m[1][0] * y + m[2][0] * z;
        m[3][1] += m[0][1] * x + m[1][1] * y + m[2][1] * z;
        m[3][2] += m[0][2] * x + m[1][2] * y + m[2][2] * z;
        m[3][3] += m[0][3] * x + m[1][3] * y + m[2][3] * z;
    }
    flagBits |= Translation;

}



/*!
    Multiplies this matrix by another that applies an orthographic
    projection for a window with lower-left corner (\a left, \a bottom),
    upper-right corner (\a right, \a top), and the specified \a nearPlane
    and \a farPlane clipping planes.

    \sa frustum(), perspective()
*/
void MxMatrix::ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    Q_ASSERT(left != right && bottom != top && nearPlane != farPlane);


        // Construct the projection.
        float width = right - left;
        float invheight = top - bottom;
        float clip = farPlane - nearPlane;
        MxMatrix m;
        m.m[0][0] = 2.0f / width;
        m.m[1][0] = 0.0f;
        m.m[2][0] = 0.0f;
        m.m[3][0] = -(left + right) / width;
        m.m[0][1] = 0.0f;
        m.m[1][1] = 2.0f / invheight;
        m.m[2][1] = 0.0f;
        m.m[3][1] = -(top + bottom) / invheight;
        m.m[0][2] = 0.0f;
        m.m[1][2] = 0.0f;
        m.m[2][2] = -2.0f / clip;
        m.m[3][2] = -(nearPlane + farPlane) / clip;
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = 0.0f;
        m.m[3][3] = 1.0f;
        m.flagBits = Translation | Scale;

        // Apply the projection.
        *this *= m;
}


/*!
    Multiplies this matrix by another that applies a perspective
    frustum projection for a window with lower-left corner (\a left, \a bottom),
    upper-right corner (\a right, \a top), and the specified \a nearPlane
    and \a farPlane clipping planes.

    \sa ortho(), perspective()
*/
void MxMatrix::frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    Q_ASSERT(left != right && bottom != top && nearPlane != farPlane);


        // Construct the projection.
        MxMatrix m;
        float width = right - left;
        float invheight = top - bottom;
        float clip = farPlane - nearPlane;
        m.m[0][0] = 2.0f * nearPlane / width;
        m.m[1][0] = 0.0f;
        m.m[2][0] = (left + right) / width;
        m.m[3][0] = 0.0f;
        m.m[0][1] = 0.0f;
        m.m[1][1] = 2.0f * nearPlane / invheight;
        m.m[2][1] = (top + bottom) / invheight;
        m.m[3][1] = 0.0f;
        m.m[0][2] = 0.0f;
        m.m[1][2] = 0.0f;
        m.m[2][2] = -(nearPlane + farPlane) / clip;
        m.m[3][2] = -2.0f * nearPlane * farPlane / clip;
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = -1.0f;
        m.m[3][3] = 0.0f;
        m.flagBits = General;

        // Apply the projection.
        *this *= m;
}

void MxMatrix::perspective(float angle, float aspect, float nearPlane, float farPlane)
{
    // Bail out if the projection volume is zero-sized.
        Q_ASSERT(nearPlane != farPlane && aspect != 0.0f);
            //return;

        // Construct the projection.
        MxMatrix m;
        float radians = (angle / 2.0f) * M_PI / 180.0f;
        float sine = sinf(radians);
        if (sine == 0.0f)
            return;
        float cotan = cosf(radians) / sine;
        float clip = farPlane - nearPlane;
        m.m[0][0] = cotan / aspect;
        m.m[1][0] = 0.0f;
        m.m[2][0] = 0.0f;
        m.m[3][0] = 0.0f;
        m.m[0][1] = 0.0f;
        m.m[1][1] = cotan;
        m.m[2][1] = 0.0f;
        m.m[3][1] = 0.0f;
        m.m[0][2] = 0.0f;
        m.m[1][2] = 0.0f;
        m.m[2][2] = -(nearPlane + farPlane) / clip;
        m.m[3][2] = -(2.0f * nearPlane * farPlane) / clip;
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = -1.0f;
        m.m[3][3] = 0.0f;
        m.flagBits = General;

        // Apply the projection.
        *this *= m;
}


void MxMatrix::perspectiveMatrix( MxMatrix &matrix, double width, double height, double nearPlane, double middle, double farPlane )
{
    double fov = atan(height/2.0*middle) * 2.0;
    double vvHeight = 2.0 * tan(fov/2.0) * nearPlane;
    double vvNear = (vvHeight / 2.0) / tan(fov / 2.0);
    matrix.setToIdentity();
    matrix.frustum(0.0, width, 0.0, height, vvNear, farPlane);
    matrix.translate( 0.0f, 0.0f, -nearPlane ); // \TODO translate to middle range ??
}


/*!
    Optimize the usage of this matrix from its current elements.

    Some operations such as translate(), scale(), and rotate() can be
    performed more efficiently if the matrix being modified is already
    known to be the identity, a previous translate(), a previous
    scale(), etc.

    Normally the MxMatrix class keeps track of this special type internally
    as operations are performed.  However, if the matrix is modified
    directly with operator()() or data(), then MxMatrix will lose track of
    the special type and will revert to the safest but least efficient
    operations thereafter.

    By calling optimize() after directly modifying the matrix,
    the programmer can force MxMatrix to recover the special type if
    the elements appear to conform to one of the known optimized types.

    \sa operator()(), data(), translate()
*/
void MxMatrix::optimize()
{
    // If the last element is not 1, then it can never be special.
    if (m[3][3] != 1.0f) {
        flagBits = General;
        return;
    }

    // If the upper three elements m12, m13, and m21 are not all zero,
    // or the lower elements below the diagonal are not all zero, then
    // the matrix can never be special.
    if (m[1][0] != 0.0f || m[2][0] != 0.0f || m[2][1] != 0.0f) {
        flagBits = General;
        return;
    }
    if (m[0][1] != 0.0f || m[0][2] != 0.0f || m[0][3] != 0.0f ||
        m[1][2] != 0.0f || m[1][3] != 0.0f || m[2][3] != 0.0f) {
        flagBits = General;
        return;
    }

    // Determine what we have in the remaining regions of the matrix.
    bool identityAlongDiagonal
        = (m[0][0] == 1.0f && m[1][1] == 1.0f && m[2][2] == 1.0f);
    bool translationPresent
        = (m[3][0] != 0.0f || m[3][1] != 0.0f || m[3][2] != 0.0f);

    // Now determine the special matrix type.
    if (translationPresent && identityAlongDiagonal)
        flagBits = Translation;
    else if (translationPresent)
        flagBits = (Translation | Scale);
    else if (identityAlongDiagonal)
        flagBits = Identity;
    else
        flagBits = Scale;
}




////////////// This is to make MxMatrix::rotate compile ////////////////
/*
#ifndef qxFuzzyIsNull
    #define qxFuzzyIsNull(a) (a<0.00001)
#endif

static inline bool qxFuzzyCompare(double p1, double p2)
{
    return (abs(p1 - p2) <= 0.000000000001 * min(abs(p1), abs(p2)));
}

static inline bool qxFuzzyCompare(float p1, float p2)
{
    return (abs(p1 - p2) <= 0.00001f * min(abs(p1), abs(p2)));
}*/
/////////////////////////////////////////////////////////////////////////


/*!
    Multiplies this matrix by another that rotates coordinates through
    \a angle degrees about the vector (\a x, \a y, \a z).

    \sa scale(), translate()
*/
void MxMatrix::rotate(float angle, float x, float y, float z)
{
    if (angle == 0.0f)
        return;
    float c, s;
    if (angle == 90.0f || angle == -270.0f) {
        s = 1.0f;
        c = 0.0f;
    } else if (angle == -90.0f || angle == 270.0f) {
        s = -1.0f;
        c = 0.0f;
    } else if (angle == 180.0f || angle == -180.0f) {
        s = 0.0f;
        c = -1.0f;
    } else {
        float a = angle * M_PI / 180.0f;
        c = cosf(a);
        s = sinf(a);
    }
    if (x == 0.0f) {
        if (y == 0.0f) {
            if (z != 0.0f) {
                // Rotate around the Z axis.
                if (z < 0)
                    s = -s;
                float tmp;
                m[0][0] = (tmp = m[0][0]) * c + m[1][0] * s;
                m[1][0] = m[1][0] * c - tmp * s;
                m[0][1] = (tmp = m[0][1]) * c + m[1][1] * s;
                m[1][1] = m[1][1] * c - tmp * s;
                m[0][2] = (tmp = m[0][2]) * c + m[1][2] * s;
                m[1][2] = m[1][2] * c - tmp * s;
                m[0][3] = (tmp = m[0][3]) * c + m[1][3] * s;
                m[1][3] = m[1][3] * c - tmp * s;

                flagBits |= Rotation2D;
                return;
            }
        } else if (z == 0.0f) {
            // Rotate around the Y axis.
            if (y < 0)
                s = -s;
            float tmp;
            m[2][0] = (tmp = m[2][0]) * c + m[0][0] * s;
            m[0][0] = m[0][0] * c - tmp * s;
            m[2][1] = (tmp = m[2][1]) * c + m[0][1] * s;
            m[0][1] = m[0][1] * c - tmp * s;
            m[2][2] = (tmp = m[2][2]) * c + m[0][2] * s;
            m[0][2] = m[0][2] * c - tmp * s;
            m[2][3] = (tmp = m[2][3]) * c + m[0][3] * s;
            m[0][3] = m[0][3] * c - tmp * s;

            flagBits |= Rotation;
            return;
        }
    } else if (y == 0.0f && z == 0.0f) {
        // Rotate around the X axis.
        if (x < 0)
            s = -s;
        float tmp;
        m[1][0] = (tmp = m[1][0]) * c + m[2][0] * s;
        m[2][0] = m[2][0] * c - tmp * s;
        m[1][1] = (tmp = m[1][1]) * c + m[2][1] * s;
        m[2][1] = m[2][1] * c - tmp * s;
        m[1][2] = (tmp = m[1][2]) * c + m[2][2] * s;
        m[2][2] = m[2][2] * c - tmp * s;
        m[1][3] = (tmp = m[1][3]) * c + m[2][3] * s;
        m[2][3] = m[2][3] * c - tmp * s;

        flagBits |= Rotation;
        return;
    }

    float len = x * x + y * y + z * z;
    if (!MxFuzzy::fuzzyCompare(len, float(1)) && !MxFuzzy::fuzzyIsNull(len)) {
        len = sqrt(len);
        x /= len;
        y /= len;
        z /= len;
    }
    float ic = 1.0f - c;
    MxMatrix rot;
    rot.m[0][0] = x * x * ic + c;
    rot.m[1][0] = x * y * ic - z * s;
    rot.m[2][0] = x * z * ic + y * s;
    rot.m[3][0] = 0.0f;
    rot.m[0][1] = y * x * ic + z * s;
    rot.m[1][1] = y * y * ic + c;
    rot.m[2][1] = y * z * ic - x * s;
    rot.m[3][1] = 0.0f;
    rot.m[0][2] = x * z * ic - y * s;
    rot.m[1][2] = y * z * ic + x * s;
    rot.m[2][2] = z * z * ic + c;
    rot.m[3][2] = 0.0f;
    rot.m[0][3] = 0.0f;
    rot.m[1][3] = 0.0f;
    rot.m[2][3] = 0.0f;
    rot.m[3][3] = 1.0f;
    rot.flagBits = Rotation;
    *this *= rot;
}


// Helper routine for inverting orthonormal matrices that consist
// of just rotations and translations.
MxMatrix MxMatrix::orthonormalInverse() const
{
    MxMatrix result;  // The '1' says not to load identity

    result.m[0][0] = m[0][0];
    result.m[1][0] = m[0][1];
    result.m[2][0] = m[0][2];

    result.m[0][1] = m[1][0];
    result.m[1][1] = m[1][1];
    result.m[2][1] = m[1][2];

    result.m[0][2] = m[2][0];
    result.m[1][2] = m[2][1];
    result.m[2][2] = m[2][2];

    result.m[0][3] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][3] = 0.0f;

    result.m[3][0] = -(result.m[0][0] * m[3][0] + result.m[1][0] * m[3][1] + result.m[2][0] * m[3][2]);
    result.m[3][1] = -(result.m[0][1] * m[3][0] + result.m[1][1] * m[3][1] + result.m[2][1] * m[3][2]);
    result.m[3][2] = -(result.m[0][2] * m[3][0] + result.m[1][2] * m[3][1] + result.m[2][2] * m[3][2]);
    result.m[3][3] = 1.0f;

    result.flagBits = flagBits;

    return result;
}
