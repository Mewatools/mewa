/****************************************************************************
** Copyright (C) 2020-2021 Mewatools <hugo@mewatools.com>
** SPDX-License-Identifier: MIT License
****************************************************************************/
/****************************************************************************
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
** SPDX-License-Identifier: Qt commercial license
****************************************************************************/
#ifndef MXMATRIX_H
#define MXMATRIX_H


#include "mxdebug.h"
#include "mxvector.h"


class MxMatrix
{
public:
  inline MxMatrix(){ flagBits = General; }

  MxMatrix(const float* array);

  float	m11 () const;
  float	m22 () const;
  float	dx () const;
  float	dy () const;


  inline const float& operator()(int row, int column) const;
  inline float& operator()(int row, int column);

  inline bool isIdentity() const;
  void setToIdentity();

  inline void fill(float value);

  MxMatrix inverted(bool *invertible = 0) const;

  inline MxMatrix& operator*=(const MxMatrix& other);
  inline bool operator==(const MxMatrix& other) const;
  inline bool operator!=(const MxMatrix& other) const;

  friend MxMatrix operator+(const MxMatrix& m1, const MxMatrix& m2);
  friend MxMatrix operator-(const MxMatrix& m1, const MxMatrix& m2);
  friend MxMatrix operator*(const MxMatrix& m1, const MxMatrix& m2);

  friend MxVector3F operator*(const MxMatrix& matrix, const MxVector3F& vector);
  friend MxVector2F operator*(const MxMatrix& matrix, const MxVector2F& point);

  void scale(float x, float y);
  void scale(float x, float y, float z);
  void scale(float factor);
  void translate(float x, float y);
  void translate(float x, float y, float z);

  void ortho(float left, float right, float bottom, float top, float nearPlane, float farPlane);
  void frustum(float left, float right, float bottom, float top, float nearPlane, float farPlane);
  void perspective(float angle, float aspect, float nearPlane, float farPlane);

  static void perspectiveMatrix( MxMatrix &matrix, double width, double height, double nearPlane, double middle, double farPlane );


  inline float *data() { return m[0]; }
  inline const float *constData() const { return m[0]; }

  void optimize();

  void rotate(float angle, float x, float y, float z);
  MxMatrix orthonormalInverse() const;

//private:
  float m[4][4];          // Column-major order to match OpenGL.
  int flagBits;           // Flag bits from the enum below.

  // When matrices are multiplied, the flag bits are or-ed together.
  enum {
      Identity        = 0x0000, // Identity matrix
      Translation     = 0x0001, // Contains a translation
      Scale           = 0x0002, // Contains a scale
      Rotation2D      = 0x0004, // Contains a rotation about the Z axis
      Rotation        = 0x0008, // Contains an arbitrary rotation
      Perspective     = 0x0010, // Last row is different from (0, 0, 0, 1)
      General         = 0x001f  // General matrix, unknown contents
  };

};




inline const float& MxMatrix::operator()(int aRow, int aColumn) const
{
  Q_ASSERT(aRow >= 0 && aRow < 4 && aColumn >= 0 && aColumn < 4);
  return m[aColumn][aRow];
}

inline float& MxMatrix::operator()(int aRow, int aColumn)
{
  Q_ASSERT(aRow >= 0 && aRow < 4 && aColumn >= 0 && aColumn < 4);
  flagBits = General;
  return m[aColumn][aRow];
}



inline bool MxMatrix::isIdentity() const
{
  if (flagBits == Identity)
    return true;
  if (m[0][0] != 1.0f || m[0][1] != 0.0f || m[0][2] != 0.0f)
    return false;
  if (m[0][3] != 0.0f || m[1][0] != 0.0f || m[1][1] != 1.0f)
    return false;
  if (m[1][2] != 0.0f || m[1][3] != 0.0f || m[2][0] != 0.0f)
    return false;
  if (m[2][1] != 0.0f || m[2][2] != 1.0f || m[2][3] != 0.0f)
    return false;
  if (m[3][0] != 0.0f || m[3][1] != 0.0f || m[3][2] != 0.0f)
    return false;
  return (m[3][3] == 1.0f);
}



inline void MxMatrix::fill(float value)
{
    // \TODO replace with memset
  /*m[0][0] = value;
  m[0][1] = value;
  m[0][2] = value;
  m[0][3] = value;
  m[1][0] = value;
  m[1][1] = value;
  m[1][2] = value;
  m[1][3] = value;
  m[2][0] = value;
  m[2][1] = value;
  m[2][2] = value;
  m[2][3] = value;
  m[3][0] = value;
  m[3][1] = value;
  m[3][2] = value;
  m[3][3] = value;*/
    for (int col = 0; col < 4; ++col)
        for (int row = 0; row < 4; ++row)
            m[col][row] = value;

  flagBits = General;
}


inline MxMatrix& MxMatrix::operator*=(const MxMatrix& other)
{
    flagBits |= other.flagBits;

    if (flagBits < Rotation2D) {
        m[3][0] += m[0][0] * other.m[3][0];
        m[3][1] += m[1][1] * other.m[3][1];
        m[3][2] += m[2][2] * other.m[3][2];

        m[0][0] *= other.m[0][0];
        m[1][1] *= other.m[1][1];
        m[2][2] *= other.m[2][2];
        return *this;
    }

    float m0, m1, m2;
    m0 = m[0][0] * other.m[0][0]
            + m[1][0] * other.m[0][1]
            + m[2][0] * other.m[0][2]
            + m[3][0] * other.m[0][3];
    m1 = m[0][0] * other.m[1][0]
            + m[1][0] * other.m[1][1]
            + m[2][0] * other.m[1][2]
            + m[3][0] * other.m[1][3];
    m2 = m[0][0] * other.m[2][0]
            + m[1][0] * other.m[2][1]
            + m[2][0] * other.m[2][2]
            + m[3][0] * other.m[2][3];
    m[3][0] = m[0][0] * other.m[3][0]
            + m[1][0] * other.m[3][1]
            + m[2][0] * other.m[3][2]
            + m[3][0] * other.m[3][3];
    m[0][0] = m0;
    m[1][0] = m1;
    m[2][0] = m2;

    m0 = m[0][1] * other.m[0][0]
            + m[1][1] * other.m[0][1]
            + m[2][1] * other.m[0][2]
            + m[3][1] * other.m[0][3];
    m1 = m[0][1] * other.m[1][0]
            + m[1][1] * other.m[1][1]
            + m[2][1] * other.m[1][2]
            + m[3][1] * other.m[1][3];
    m2 = m[0][1] * other.m[2][0]
            + m[1][1] * other.m[2][1]
            + m[2][1] * other.m[2][2]
            + m[3][1] * other.m[2][3];
    m[3][1] = m[0][1] * other.m[3][0]
            + m[1][1] * other.m[3][1]
            + m[2][1] * other.m[3][2]
            + m[3][1] * other.m[3][3];
    m[0][1] = m0;
    m[1][1] = m1;
    m[2][1] = m2;

    m0 = m[0][2] * other.m[0][0]
            + m[1][2] * other.m[0][1]
            + m[2][2] * other.m[0][2]
            + m[3][2] * other.m[0][3];
    m1 = m[0][2] * other.m[1][0]
            + m[1][2] * other.m[1][1]
            + m[2][2] * other.m[1][2]
            + m[3][2] * other.m[1][3];
    m2 = m[0][2] * other.m[2][0]
            + m[1][2] * other.m[2][1]
            + m[2][2] * other.m[2][2]
            + m[3][2] * other.m[2][3];
    m[3][2] = m[0][2] * other.m[3][0]
            + m[1][2] * other.m[3][1]
            + m[2][2] * other.m[3][2]
            + m[3][2] * other.m[3][3];
    m[0][2] = m0;
    m[1][2] = m1;
    m[2][2] = m2;

    m0 = m[0][3] * other.m[0][0]
            + m[1][3] * other.m[0][1]
            + m[2][3] * other.m[0][2]
            + m[3][3] * other.m[0][3];
    m1 = m[0][3] * other.m[1][0]
            + m[1][3] * other.m[1][1]
            + m[2][3] * other.m[1][2]
            + m[3][3] * other.m[1][3];
    m2 = m[0][3] * other.m[2][0]
            + m[1][3] * other.m[2][1]
            + m[2][3] * other.m[2][2]
            + m[3][3] * other.m[2][3];
    m[3][3] = m[0][3] * other.m[3][0]
            + m[1][3] * other.m[3][1]
            + m[2][3] * other.m[3][2]
            + m[3][3] * other.m[3][3];
    m[0][3] = m0;
    m[1][3] = m1;
    m[2][3] = m2;
    return *this;
}

inline bool MxMatrix::operator==(const MxMatrix& other) const
{
    return m[0][0] == other.m[0][0] &&
           m[0][1] == other.m[0][1] &&
           m[0][2] == other.m[0][2] &&
           m[0][3] == other.m[0][3] &&
           m[1][0] == other.m[1][0] &&
           m[1][1] == other.m[1][1] &&
           m[1][2] == other.m[1][2] &&
           m[1][3] == other.m[1][3] &&
           m[2][0] == other.m[2][0] &&
           m[2][1] == other.m[2][1] &&
           m[2][2] == other.m[2][2] &&
           m[2][3] == other.m[2][3] &&
           m[3][0] == other.m[3][0] &&
           m[3][1] == other.m[3][1] &&
           m[3][2] == other.m[3][2] &&
           m[3][3] == other.m[3][3];
}

inline bool MxMatrix::operator!=(const MxMatrix& other) const
{
    return m[0][0] != other.m[0][0] ||
           m[0][1] != other.m[0][1] ||
           m[0][2] != other.m[0][2] ||
           m[0][3] != other.m[0][3] ||
           m[1][0] != other.m[1][0] ||
           m[1][1] != other.m[1][1] ||
           m[1][2] != other.m[1][2] ||
           m[1][3] != other.m[1][3] ||
           m[2][0] != other.m[2][0] ||
           m[2][1] != other.m[2][1] ||
           m[2][2] != other.m[2][2] ||
           m[2][3] != other.m[2][3] ||
           m[3][0] != other.m[3][0] ||
           m[3][1] != other.m[3][1] ||
           m[3][2] != other.m[3][2] ||
           m[3][3] != other.m[3][3];
}


inline MxMatrix operator+(const MxMatrix& m1, const MxMatrix& m2)
{
  MxMatrix m;
 /* m.m[0][0] = m1.m[0][0] + m2.m[0][0];
  m.m[0][1] = m1.m[0][1] + m2.m[0][1];
  m.m[0][2] = m1.m[0][2] + m2.m[0][2];
  m.m[0][3] = m1.m[0][3] + m2.m[0][3];
  m.m[1][0] = m1.m[1][0] + m2.m[1][0];
  m.m[1][1] = m1.m[1][1] + m2.m[1][1];
  m.m[1][2] = m1.m[1][2] + m2.m[1][2];
  m.m[1][3] = m1.m[1][3] + m2.m[1][3];
  m.m[2][0] = m1.m[2][0] + m2.m[2][0];
  m.m[2][1] = m1.m[2][1] + m2.m[2][1];
  m.m[2][2] = m1.m[2][2] + m2.m[2][2];
  m.m[2][3] = m1.m[2][3] + m2.m[2][3];
  m.m[3][0] = m1.m[3][0] + m2.m[3][0];
  m.m[3][1] = m1.m[3][1] + m2.m[3][1];
  m.m[3][2] = m1.m[3][2] + m2.m[3][2];
  m.m[3][3] = m1.m[3][3] + m2.m[3][3];*/
  for (int row = 0; row < 4; ++row)
      for (int col = 0; col < 4; ++col)
          m.m[col][row] = m1.m[col][row] + m2.m[col][row];
  return m;
}

inline MxMatrix operator-(const MxMatrix& m1, const MxMatrix& m2)
{
  MxMatrix m;
  m.m[0][0] = m1.m[0][0] - m2.m[0][0];
  m.m[0][1] = m1.m[0][1] - m2.m[0][1];
  m.m[0][2] = m1.m[0][2] - m2.m[0][2];
  m.m[0][3] = m1.m[0][3] - m2.m[0][3];
  m.m[1][0] = m1.m[1][0] - m2.m[1][0];
  m.m[1][1] = m1.m[1][1] - m2.m[1][1];
  m.m[1][2] = m1.m[1][2] - m2.m[1][2];
  m.m[1][3] = m1.m[1][3] - m2.m[1][3];
  m.m[2][0] = m1.m[2][0] - m2.m[2][0];
  m.m[2][1] = m1.m[2][1] - m2.m[2][1];
  m.m[2][2] = m1.m[2][2] - m2.m[2][2];
  m.m[2][3] = m1.m[2][3] - m2.m[2][3];
  m.m[3][0] = m1.m[3][0] - m2.m[3][0];
  m.m[3][1] = m1.m[3][1] - m2.m[3][1];
  m.m[3][2] = m1.m[3][2] - m2.m[3][2];
  m.m[3][3] = m1.m[3][3] - m2.m[3][3];
  return m;
}

inline MxMatrix operator*(const MxMatrix& m1, const MxMatrix& m2)
{
    int flagBits = m1.flagBits | m2.flagBits;
    if (flagBits < MxMatrix::Rotation2D) {
        MxMatrix m = m1;
        m.m[3][0] += m.m[0][0] * m2.m[3][0];
        m.m[3][1] += m.m[1][1] * m2.m[3][1];
        m.m[3][2] += m.m[2][2] * m2.m[3][2];

        m.m[0][0] *= m2.m[0][0];
        m.m[1][1] *= m2.m[1][1];
        m.m[2][2] *= m2.m[2][2];
        m.flagBits = flagBits;
        return m;
    }

    MxMatrix m;
    m.m[0][0] = m1.m[0][0] * m2.m[0][0]
              + m1.m[1][0] * m2.m[0][1]
              + m1.m[2][0] * m2.m[0][2]
              + m1.m[3][0] * m2.m[0][3];
    m.m[0][1] = m1.m[0][1] * m2.m[0][0]
              + m1.m[1][1] * m2.m[0][1]
              + m1.m[2][1] * m2.m[0][2]
              + m1.m[3][1] * m2.m[0][3];
    m.m[0][2] = m1.m[0][2] * m2.m[0][0]
              + m1.m[1][2] * m2.m[0][1]
              + m1.m[2][2] * m2.m[0][2]
              + m1.m[3][2] * m2.m[0][3];
    m.m[0][3] = m1.m[0][3] * m2.m[0][0]
              + m1.m[1][3] * m2.m[0][1]
              + m1.m[2][3] * m2.m[0][2]
              + m1.m[3][3] * m2.m[0][3];

    m.m[1][0] = m1.m[0][0] * m2.m[1][0]
              + m1.m[1][0] * m2.m[1][1]
              + m1.m[2][0] * m2.m[1][2]
              + m1.m[3][0] * m2.m[1][3];
    m.m[1][1] = m1.m[0][1] * m2.m[1][0]
              + m1.m[1][1] * m2.m[1][1]
              + m1.m[2][1] * m2.m[1][2]
              + m1.m[3][1] * m2.m[1][3];
    m.m[1][2] = m1.m[0][2] * m2.m[1][0]
              + m1.m[1][2] * m2.m[1][1]
              + m1.m[2][2] * m2.m[1][2]
              + m1.m[3][2] * m2.m[1][3];
    m.m[1][3] = m1.m[0][3] * m2.m[1][0]
              + m1.m[1][3] * m2.m[1][1]
              + m1.m[2][3] * m2.m[1][2]
              + m1.m[3][3] * m2.m[1][3];

    m.m[2][0] = m1.m[0][0] * m2.m[2][0]
              + m1.m[1][0] * m2.m[2][1]
              + m1.m[2][0] * m2.m[2][2]
              + m1.m[3][0] * m2.m[2][3];
    m.m[2][1] = m1.m[0][1] * m2.m[2][0]
              + m1.m[1][1] * m2.m[2][1]
              + m1.m[2][1] * m2.m[2][2]
              + m1.m[3][1] * m2.m[2][3];
    m.m[2][2] = m1.m[0][2] * m2.m[2][0]
              + m1.m[1][2] * m2.m[2][1]
              + m1.m[2][2] * m2.m[2][2]
              + m1.m[3][2] * m2.m[2][3];
    m.m[2][3] = m1.m[0][3] * m2.m[2][0]
              + m1.m[1][3] * m2.m[2][1]
              + m1.m[2][3] * m2.m[2][2]
              + m1.m[3][3] * m2.m[2][3];

    m.m[3][0] = m1.m[0][0] * m2.m[3][0]
              + m1.m[1][0] * m2.m[3][1]
              + m1.m[2][0] * m2.m[3][2]
              + m1.m[3][0] * m2.m[3][3];
    m.m[3][1] = m1.m[0][1] * m2.m[3][0]
              + m1.m[1][1] * m2.m[3][1]
              + m1.m[2][1] * m2.m[3][2]
              + m1.m[3][1] * m2.m[3][3];
    m.m[3][2] = m1.m[0][2] * m2.m[3][0]
              + m1.m[1][2] * m2.m[3][1]
              + m1.m[2][2] * m2.m[3][2]
              + m1.m[3][2] * m2.m[3][3];
    m.m[3][3] = m1.m[0][3] * m2.m[3][0]
              + m1.m[1][3] * m2.m[3][1]
              + m1.m[2][3] * m2.m[3][2]
              + m1.m[3][3] * m2.m[3][3];
    m.flagBits = flagBits;
    return m;
}



inline MxVector3F operator*(const MxMatrix& matrix, const MxVector3F& vector)
{
    float x, y, z, w;
    if (matrix.flagBits == MxMatrix::Identity) {
        return vector;
    } else if (matrix.flagBits < MxMatrix::Rotation2D) {
        // Translation | Scale
        return MxVector3F(vector.x() * matrix.m[0][0] + matrix.m[3][0],
                         vector.y() * matrix.m[1][1] + matrix.m[3][1],
                         vector.z() * matrix.m[2][2] + matrix.m[3][2]);
    } else if (matrix.flagBits < MxMatrix::Rotation) {
        // Translation | Scale | Rotation2D
        return MxVector3F(vector.x() * matrix.m[0][0] + vector.y() * matrix.m[1][0] + matrix.m[3][0],
                         vector.x() * matrix.m[0][1] + vector.y() * matrix.m[1][1] + matrix.m[3][1],
                         vector.z() * matrix.m[2][2] + matrix.m[3][2]);
    } else {
        x = vector.x() * matrix.m[0][0] +
            vector.y() * matrix.m[1][0] +
            vector.z() * matrix.m[2][0] +
            matrix.m[3][0];
        y = vector.x() * matrix.m[0][1] +
            vector.y() * matrix.m[1][1] +
            vector.z() * matrix.m[2][1] +
            matrix.m[3][1];
        z = vector.x() * matrix.m[0][2] +
            vector.y() * matrix.m[1][2] +
            vector.z() * matrix.m[2][2] +
            matrix.m[3][2];
        w = vector.x() * matrix.m[0][3] +
            vector.y() * matrix.m[1][3] +
            vector.z() * matrix.m[2][3] +
            matrix.m[3][3];
        if (w == 1.0f)
            return MxVector3F(x, y, z);
        else
            return MxVector3F(x / w, y / w, z / w);
    }
}




inline MxVector2F operator*(const MxMatrix& matrix, const MxVector2F& point)
{
    float xin, yin;
    float x, y, w;
    xin = point.x();
    yin = point.y();
    if (matrix.flagBits == MxMatrix::Identity) {
        return point;
    } else if (matrix.flagBits < MxMatrix::Rotation2D) {
        // Translation | Scale
        return MxVector2F(xin * matrix.m[0][0] + matrix.m[3][0],
                       yin * matrix.m[1][1] + matrix.m[3][1]);
    } else if (matrix.flagBits < MxMatrix::Perspective) {
        return MxVector2F(xin * matrix.m[0][0] + yin * matrix.m[1][0] + matrix.m[3][0],
                       xin * matrix.m[0][1] + yin * matrix.m[1][1] + matrix.m[3][1]);
    } else {
        x = xin * matrix.m[0][0] +
            yin * matrix.m[1][0] +
            matrix.m[3][0];
        y = xin * matrix.m[0][1] +
            yin * matrix.m[1][1] +
            matrix.m[3][1];
        w = xin * matrix.m[0][3] +
            yin * matrix.m[1][3] +
            matrix.m[3][3];
        if (w == 1.0f) {
            return MxVector2F(float(x), float(y));
        } else {
            return MxVector2F(float(x / w), float(y / w));
        }
    }
}


#endif
