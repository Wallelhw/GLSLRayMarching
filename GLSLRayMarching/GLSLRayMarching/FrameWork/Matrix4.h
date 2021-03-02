///////////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2016, Lin Koon Wing Macgyver, macgyvercct@yahoo.com.hk			 //
//																				 //
// Author : Mac Lin									                             //
// Module : Magnum Engine v1.0.0												 //
// Date   : 14/Jun/2016											                 //
//																				 //
///////////////////////////////////////////////////////////////////////////////////
#ifndef _Matrix4_h_
#define _Matrix4_h_

#include "Maths.h"
#include "Vector4.h"
#include "Vector3.h"

static const int RowStartIdxs[4] = { 0, 4, 8, 12 };

template<class T>
class TMatrix4
{
public:
    TMatrix4(bool bZero = true)
    {
        if (bZero)
        {
            SetZero();
        }
        else
        {
            SetIdentity();
        }
    }

    TMatrix4(const TMatrix4& mat)
    {
        size_t uiSize = 16 * sizeof(T);

        memcpy(m, mat.m, uiSize);
    }

    TMatrix4(T m00, T m01, T m02, T m03,
        T m10, T m11, T m12, T m13,
        T m20, T m21, T m22, T m23,
        T m30, T m31, T m32, T m33)
    {
        m[0] = m00;
        m[1] = m01;
        m[2] = m02;
        m[3] = m03;
        m[4] = m10;
        m[5] = m11;
        m[6] = m12;
        m[7] = m13;
        m[8] = m20;
        m[9] = m21;
        m[10] = m22;
        m[11] = m23;
        m[12] = m30;
        m[13] = m31;
        m[14] = m32;
        m[15] = m33;
    }

    TMatrix4(const T entry[16], bool rowMajor = true)
    {
        if (rowMajor)
        {
            size_t uiSize = 16 * sizeof(T);
            memcpy(m, entry, uiSize);
        }
        else
        {
            m[0] = entry[0];
            m[1] = entry[4];
            m[2] = entry[8];
            m[3] = entry[12];
            m[4] = entry[1];
            m[5] = entry[5];
            m[6] = entry[9];
            m[7] = entry[13];
            m[8] = entry[2];
            m[9] = entry[6];
            m[10] = entry[10];
            m[11] = entry[14];
            m[12] = entry[3];
            m[13] = entry[7];
            m[14] = entry[11];
            m[15] = entry[15];
        }
    }

    // input Mrc is in row r, column c.
    void Set(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33)
    {
        m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
        m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
        m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
        m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
    }

    void SetZero()
    {
        size_t uiSize = 16 * sizeof(T);
        memset(m, 0, uiSize);
    }

    void SetIdentity()
    {
        m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
        m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
        m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }

    void SetTranslate(float x, float y, float z)
    {
        m[0] = 1; m[1] = 0; m[2] = 0; m[3] = x;
        m[4] = 0; m[5] = 1; m[6] = 0; m[7] = y;
        m[8] = 0; m[9] = 0; m[10] = 1; m[11] = z;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }

    void SetRotateX(float angle)
    {
        SetIdentity();

        float radian = angle * Math::Degree2Radian;
        float cosine = Math::Cos(radian);
        float sine = Math::Sin(radian);

        m[5] = cosine;
        m[9] = sine;
        m[6] = -sine;
        m[10] = cosine;
    }

    void SetRotateY(float angle)
    {
        SetIdentity();

        float radian = angle * Math::Degree2Radian;
        float cosine = Math::Cos(radian);
        float sine = Math::Sin(radian);

        m[0] = cosine;
        m[8] = -sine;
        m[2] = sine;
        m[10] = cosine;
    }

    void SetRotateZ(float angle)
    {
        SetIdentity();

        float radian = angle * Math::Degree2Radian;
        float cosine = Math::Cos(radian);
        float sine = Math::Sin(radian);

        m[0] = cosine;
        m[4] = sine;
        m[1] = -sine;
        m[5] = cosine;
    }

    void SetRotateZXY(float zAngle, float xAngle, float yAngle)
    {
        SetTranslateRotZXYScale(0, 0, 0, zAngle, xAngle, yAngle, 1);
    }

    void SetRotateZYX(float zAngle, float yAngle, float xAngle)
    {
        SetTranslateRotZYXScale(0, 0, 0, zAngle, yAngle, xAngle, 1);
    }

    void SetRotateAxisAngle(const TVector3<T>& axis, float angle)
    {
        SetIdentity();

        float radian = angle * Math::Degree2Radian;
        float cos = Math::Cos(-radian);
        float sin = Math::Sin(-radian);
        float oneMinusCos = 1.0f - cos;
        float x2 = axis[0] * axis[0];
        float y2 = axis[1] * axis[1];
        float z2 = axis[2] * axis[2];
        float xyM = axis[0] * axis[1] * oneMinusCos;
        float xzM = axis[0] * axis[2] * oneMinusCos;
        float yzM = axis[1] * axis[2] * oneMinusCos;
        float xSin = axis[0] * sin;
        float ySin = axis[1] * sin;
        float zSin = axis[2] * sin;

        m[0] = x2 * oneMinusCos + cos;
        m[1] = xyM + zSin;
        m[2] = xzM - ySin;

        m[4] = xyM - zSin;
        m[5] = y2 * oneMinusCos + cos;
        m[6] = yzM + xSin;

        m[8] = xzM + ySin;
        m[9] = yzM - xSin;
        m[10] = z2 * oneMinusCos + cos;
    }

    void SetScale(float scale)
    {
        SetScale(scale, scale, scale);
    }

    void SetScale(float x, float y, float z)
    {
        SetIdentity();

        m[0] = x;
        m[5] = y;
        m[10] = z;
    }

    void SetTranslateRotZXYScale(float tx, float ty, float tz, float rz, float rx, float ry, float scale)
    {
        SetRotationZXY(rz, rx, ry);

        m[0] *= scale;
        m[4] *= scale;
        m[8] *= scale;

        m[1] *= scale;
        m[5] *= scale;
        m[9] *= scale;

        m[2] *= scale;
        m[6] *= scale;
        m[10] *= scale;

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetTranslateRotZYXScale(float tx, float ty, float tz, float rz, float ry, float rx, float scale)
    {
        SetRotationZYX(rz, ry, rx);

        m[0] *= scale;
        m[4] *= scale;
        m[8] *= scale;

        m[1] *= scale;
        m[5] *= scale;
        m[9] *= scale;

        m[2] *= scale;
        m[6] *= scale;
        m[10] *= scale;

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetTranslateRotYXZScale(float tx, float ty, float tz, float ry, float rx, float rz, float scale)
    {
        SetRotationYXZ(ry, rx, rz);

        m[0] *= scale;
        m[4] *= scale;
        m[8] *= scale;

        m[1] *= scale;
        m[5] *= scale;
        m[9] *= scale;

        m[2] *= scale;
        m[6] *= scale;
        m[10] *= scale;

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetTranslateRotYZXScale(float tx, float ty, float tz, float ry, float rz, float rx, float scale)
    {
        SetRotationYZX(ry, rz, rx);

        m[0] *= scale;
        m[4] *= scale;
        m[8] *= scale;

        m[1] *= scale;
        m[5] *= scale;
        m[9] *= scale;

        m[2] *= scale;
        m[6] *= scale;
        m[10] *= scale;

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetTranslateRotXYZScale(float tx, float ty, float tz, float rx, float ry, float rz, float scale)
    {
        SetRotationXYZ(rx, ry, rz);

        m[0] *= scale;
        m[4] *= scale;
        m[8] *= scale;

        m[1] *= scale;
        m[5] *= scale;
        m[9] *= scale;

        m[2] *= scale;
        m[6] *= scale;
        m[10] *= scale;

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetTranslateRotXZYScale(float tx, float ty, float tz, float rx, float rz, float ry, float scale)
    {
        SetRotationXZY(rx, rz, ry);

        m[0] *= scale;
        m[4] *= scale;
        m[8] *= scale;

        m[1] *= scale;
        m[5] *= scale;
        m[9] *= scale;

        m[2] *= scale;
        m[6] *= scale;
        m[10] *= scale;

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetTranslateRotateAxisAngleScale(float tx, float ty, float tz, const TVector3<T>& axis, const float angle, float scale)
    {
        SetRotateAxisAngle(axis, angle);

        m[0] *= scale;
        m[4] *= scale;
        m[8] *= scale;

        m[1] *= scale;
        m[5] *= scale;
        m[9] *= scale;

        m[2] *= scale;
        m[6] *= scale;
        m[10] *= scale;

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetTranslateScale(float tx, float ty, float tz, float scale)
    {
        SetScale(scale);

        m[3] = tx;
        m[7] = ty;
        m[11] = tz;
    }

    void SetLookAt(const TVector3<T>& position, const TVector3<T>& objective, const TVector3<T>& up)
    {
        SetLookAtScale(position, objective, up, 1);
    }

    void SetLookAtScale(const TVector3<T>& position, const TVector3<T>& objective, const TVector3<T>& up, float scale)
    {
        // Right Hand
        TVector3<T> zaxis = position - objective; zaxis.Normalize();
        TVector3<T> yaxis = up - zaxis * up.Dot(zaxis); yaxis.Normalize();
        TVector3<T> xaxis = yaxis.Cross(zaxis);

        xaxis *= scale;
        yaxis *= scale;
        zaxis *= scale;

        m[0] = xaxis.X();
        m[1] = yaxis.X();
        m[2] = zaxis.X();
        m[3] = position.X();
        m[4] = xaxis.Y();
        m[5] = yaxis.Y();
        m[6] = zaxis.Y();
        m[7] = position.Y();
        m[8] = xaxis.Z();
        m[9] = yaxis.Z();
        m[10] = zaxis.Z();
        m[11] = position.Z();
        m[12] = 0;
        m[13] = 0;
        m[14] = 0;
        m[15] = 1;
    }

    void SetStandOn(const TVector3<T>& position, const TVector3<T>& objective, const TVector3<T>& up)
    {
        SetStandOnScale(position, objective, up, 1);
    }

    void SetStandOnScale(const TVector3<T>& position, const TVector3<T>& objective, const TVector3<T>& up, float scale)
    {
        Vector3 yaxis = up; yaxis.Normalize();
        Vector3 zaxis = position - objective;
        zaxis = zaxis - up * zaxis.Dot(up); zaxis.Normalize();
        Vector3 xaxis = yaxis.Cross(zaxis);

        xaxis *= scale;
        yaxis *= scale;
        zaxis *= scale;

        m[0] = xaxis.X();
        m[1] = yaxis.X();
        m[2] = zaxis.X();
        m[3] = position.X();
        m[4] = xaxis.Y();
        m[5] = yaxis.Y();
        m[6] = zaxis.Y();
        m[7] = position.Y();
        m[8] = xaxis.Z();
        m[9] = yaxis.Z();
        m[10] = zaxis.Z();
        m[11] = position.Z();
        m[12] = 0;
        m[13] = 0;
        m[14] = 0;
        m[15] = 1;
    }

    void SetPerspective(float w, float h, float n, float f)
    {
        SetPerspectiveOffCenter(-w / 2, w / 2, -h / 2, h / 2, n, f);
    }

    void SetPerspectiveFov(float fovY, float aspect, float n, float f)
    {
        assert(fovY < 180);

        float radianFovY = fovY * Math::Degree2Radian;
        float tanHalfFovY = Math::Tan(radianFovY / 2);
        float t = n * tanHalfFovY;
        float r = t * aspect;

        SetPerspectiveOffCenter(-r, r, -t, t, n, f);
    }

    void SetPerspectiveOffCenter(float l, float r, float b, float t, float n, float f)
    {
        m[0] = 2 * n / (r - l);
        m[1] = 0;
        m[2] = (r + l) / (r - l);
        m[3] = 0;

        m[4] = 0;
        m[5] = 2 * n / (t - b);
        m[6] = (t + b) / (t - b);
        m[7] = 0;

        m[8] = 0;
        m[9] = 0;
        m[10] = -(f + n) / (f - n);
        m[11] = -2 * f * n / (f - n);

        m[12] = 0;
        m[13] = 0;
        m[14] = -1;
        m[15] = 0;
    }

    void SetOrthogonal(float w, float h, float n, float f)
    {
        SetOrthogonalOffCenter(-w / 2, w / 2, -h / 2, h / 2, n, f);
    }

    void SetOrthogonalOffCenter(float l, float r, float b, float t, float n, float f)
    {
        m[0] = 2 / (r - l);
        m[1] = 0;
        m[2] = 0;
        m[3] = -(r + l) / (r - l);

        m[4] = 0;
        m[5] = 2 / (t - b);
        m[6] = 0;
        m[7] = -(t + b) / (t - b);

        m[8] = 0;
        m[9] = 0;
        m[10] = -2 / (f - n);
        m[11] = -(f + n) / (f - n);

        m[12] = 0;
        m[13] = 0;
        m[14] = 0;
        m[15] = 1;
    }

    void Orthonormalize()
    {
        float invLength = Math::InvSqrt(m[0] * m[0] + m[4] * m[4] + m[8] * m[8]);

        m[0] *= invLength;
        m[4] *= invLength;
        m[8] *= invLength;

        // Compute q1.
        float dot0 = m[0] * m[1] + m[4] * m[5] + m[8] * m[9];

        m[1] -= dot0 * m[0];
        m[5] -= dot0 * m[4];
        m[9] -= dot0 * m[8];

        invLength = Math::InvSqrt(m[1] * m[1] + m[5] * m[5] + m[9] * m[9]);

        m[1] *= invLength;
        m[5] *= invLength;
        m[9] *= invLength;

        // compute q2
        float dot1 = m[1] * m[2] + m[5] * m[6] + m[9] * m[10];

        dot0 = m[0] * m[2] + m[4] * m[6] + m[8] * m[10];

        m[2] -= dot0 * m[0] + dot1 * m[1];
        m[6] -= dot0 * m[4] + dot1 * m[5];
        m[10] -= dot0 * m[8] + dot1 * m[9];

        invLength = Math::InvSqrt(m[2] * m[2] + m[6] * m[6] + m[10] * m[10]);

        m[2] *= invLength;
        m[6] *= invLength;
        m[10] *= invLength;
    }

    operator const float* () const
    {
        return m;
    }

    operator float* ()
    {
        return m;
    }

    const float* operator[] (int row) const
    {
        return &m[RowStartIdxs[row]];
    }

    float* operator[] (int row)
    {
        return &m[RowStartIdxs[row]];
    }

    float operator() (int row, int col) const
    {
        return m[I(row, col)];
    }

    float& operator() (int row, int col)
    {
        return m[I(row, col)];
    }

    void SetRow(int row, const TVector4<T>& v)
    {
        assert(0 <= row && row < 4);
        for (int col = 0, i = RowStartIdxs[row]; col < 4; col++, i++)
        {
            m[i] = v[col];
        }
    }

    TVector4<T> GetRow(int row) const
    {
        assert(0 <= row && row < 4);
        Vector4 v;
        for (int col = 0, i = RowStartIdxs[row]; col < 4; col++, i++)
        {
            v[col] = m[i];
        }
        return v;
    }

    void SetColumn(int col, const TVector4<T>& v)
    {
        assert(0 <= col && col < 4);
        for (int row = 0, i = col; row < 4; row++, i += 4)
        {
            m[i] = v[row];
        }
    }

    TVector4<T> GetColumn(int col) const
    {
        assert(0 <= col && col < 4);
        Vector4 v;
        for (int row = 0, i = col; row < 4; row++, i += 4)
        {
            v[row] = m[i];
        }
        return v;
    }

    void GetColumnMajor(float* columnMajor) const
    {
        for (int row = 0, i = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                columnMajor[i++] = m[I(col, row)];
            }
        }
    }

    TMatrix4& operator= (const TMatrix4& mat)
    {
        size_t uiSize = 16 * sizeof(T);

        memcpy(m, mat.m, uiSize);

        return *this;
    }

    int CompareArrays(const TMatrix4& mat) const
    {
        return memcmp(m, mat.m, 16 * sizeof(T));
    }

    inline bool operator== (const TMatrix4& mat) const
    {
        return CompareArrays(mat) == 0;
    }

    inline bool operator!= (const TMatrix4& mat) const
    {
        return CompareArrays(mat) != 0;
    }

    inline bool operator<  (const TMatrix4& mat) const
    {
        return CompareArrays(mat) < 0;
    }

    inline bool operator<= (const TMatrix4& mat) const
    {
        return CompareArrays(mat) <= 0;
    }

    inline bool operator>  (const TMatrix4& mat) const
    {
        return CompareArrays(mat) > 0;
    }

    inline bool operator>= (const TMatrix4& mat) const
    {
        return CompareArrays(mat) >= 0;
    }

    TMatrix4 operator+ (const TMatrix4& mat) const
    {
        TMatrix4 sum;
        for (int i = 0; i < 16; i++)
        {
            sum.m[i] = m[i] + mat.m[i];
        }
        return sum;
    }

    TMatrix4 operator- (const TMatrix4& mat) const
    {
        TMatrix4 sum;
        for (int i = 0; i < 16; i++)
        {
            sum.m[i] = m[i] - mat.m[i];
        }
        return sum;
    }

    TMatrix4 operator* (const TMatrix4& mat) const
    {
        TMatrix4 result;

        result.m[0] = m[0] * mat.m[0] + m[1] * mat.m[4] + m[2] * mat.m[8] + m[3] * mat.m[12];
        result.m[1] = m[0] * mat.m[1] + m[1] * mat.m[5] + m[2] * mat.m[9] + m[3] * mat.m[13];
        result.m[2] = m[0] * mat.m[2] + m[1] * mat.m[6] + m[2] * mat.m[10] + m[3] * mat.m[14];
        result.m[3] = m[0] * mat.m[3] + m[1] * mat.m[7] + m[2] * mat.m[11] + m[3] * mat.m[15];

        result.m[4] = m[4] * mat.m[0] + m[5] * mat.m[4] + m[6] * mat.m[8] + m[7] * mat.m[12];
        result.m[5] = m[4] * mat.m[1] + m[5] * mat.m[5] + m[6] * mat.m[9] + m[7] * mat.m[13];
        result.m[6] = m[4] * mat.m[2] + m[5] * mat.m[6] + m[6] * mat.m[10] + m[7] * mat.m[14];
        result.m[7] = m[4] * mat.m[3] + m[5] * mat.m[7] + m[6] * mat.m[11] + m[7] * mat.m[15];

        result.m[8] = m[8] * mat.m[0] + m[9] * mat.m[4] + m[10] * mat.m[8] + m[11] * mat.m[12];
        result.m[9] = m[8] * mat.m[1] + m[9] * mat.m[5] + m[10] * mat.m[9] + m[11] * mat.m[13];
        result.m[10] = m[8] * mat.m[2] + m[9] * mat.m[6] + m[10] * mat.m[10] + m[11] * mat.m[14];
        result.m[11] = m[8] * mat.m[3] + m[9] * mat.m[7] + m[10] * mat.m[11] + m[11] * mat.m[15];

        result.m[12] = m[12] * mat.m[0] + m[13] * mat.m[4] + m[14] * mat.m[8] + m[15] * mat.m[12];
        result.m[13] = m[12] * mat.m[1] + m[13] * mat.m[5] + m[14] * mat.m[9] + m[15] * mat.m[13];
        result.m[14] = m[12] * mat.m[2] + m[13] * mat.m[6] + m[14] * mat.m[10] + m[15] * mat.m[14];
        result.m[15] = m[12] * mat.m[3] + m[13] * mat.m[7] + m[14] * mat.m[11] + m[15] * mat.m[15];

        return result;
    }

    TMatrix4 operator* (T scale) const
    {
        TMatrix4 result;
        for (int i = 0; i < 16; i++)
        {
            result.m[i] = scale * m[i];
        }
        return result;
    }

    TMatrix4 operator/ (T scale) const
    {
        TMatrix4 result;
        int i;

        if (scale != (float)0.0)
        {
            float invScalar = ((float)1.0) / scale;
            for (i = 0; i < 16; i++)
            {
                result.m[i] = invScalar * m[i];
            }
        }
        else
        {
            for (i = 0; i < 16; i++)
            {
                result.m[i] = Math::MAX_REAL;
            }
        }

        return result;
    }

    TMatrix4 operator- () const
    {
        TMatrix4 result;
        for (int i = 0; i < 16; i++)
        {
            result.m[i] = -m[i];
        }
        return result;
    }

    TMatrix4& operator+= (const TMatrix4& mat)
    {
        for (int i = 0; i < 16; i++)
        {
            m[i] += mat.m[i];
        }
        return *this;
    }

    TMatrix4& operator-= (const TMatrix4& mat)
    {
        for (int i = 0; i < 16; i++)
        {
            m[i] -= mat.m[i];
        }
        return *this;
    }

    TMatrix4& operator*= (T scalar)
    {
        for (int i = 0; i < 16; i++)
        {
            m[i] *= scalar;
        }
        return *this;
    }

    TMatrix4& operator/= (T scalar)
    {
        int i;

        if (scalar != (float)0.0)
        {
            float invScalar = ((float)1.0) / scalar;
            for (i = 0; i < 16; i++)
            {
                m[i] *= invScalar;
            }
        }
        else
        {
            for (i = 0; i < 16; i++)
            {
                m[i] = Math::MAX_REAL;
            }
        }

        return *this;
    }

    TVector4<T> operator* (const TVector4<T>& v) const
    {
        Vector4 result;
        result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3] * v[3];
        result[1] = m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7] * v[3];
        result[2] = m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11] * v[3];
        result[3] = m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15] * v[3];

        return result;
    }

    TVector3<T> operator* (const TVector3<T>& v) const
    {
        Vector3 result;
        result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
        result[1] = m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7];
        result[2] = m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11];

        return result;
    }

    TVector3<T> TimesPositionVector(const TVector3<T>& v) const
    {
        Vector3 result;
        result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
        result[1] = m[4] * v[0] + m[5] * v[1] + m[6] * v[2] + m[7];
        result[2] = m[8] * v[0] + m[9] * v[1] + m[10] * v[2] + m[11];

        return result;
    }

    TVector3<T> TimesDirectionVector(const TVector3<T>& v) const
    {
        Vector3 result;
        result[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
        result[1] = m[4] * v[0] + m[5] * v[1] + m[6] * v[2];
        result[2] = m[8] * v[0] + m[9] * v[1] + m[10] * v[2];

        return result;
    }

    TMatrix4 Transpose() const
    {
        TMatrix4 transpose;
        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                transpose.m[I(row, col)] = m[I(col, row)];
            }
        }
        return transpose;
    }

    TMatrix4 TransposeTimes(const TMatrix4& mat) const
    {
        TMatrix4 result;

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                int i = I(row, col);
                result.m[i] = 0.0;
                for (int mid = 0; mid < 4; mid++)
                {
                    result.m[i] += m[I(mid, row)] * mat.m[I(mid, col)];
                }
            }
        }
        return result;
    }

    TMatrix4 Inverse() const
    {
        float fA0 = m[0] * m[5] - m[1] * m[4];
        float fA1 = m[0] * m[6] - m[2] * m[4];
        float fA2 = m[0] * m[7] - m[3] * m[4];
        float fA3 = m[1] * m[6] - m[2] * m[5];
        float fA4 = m[1] * m[7] - m[3] * m[5];
        float fA5 = m[2] * m[7] - m[3] * m[6];
        float fB0 = m[8] * m[13] - m[9] * m[12];
        float fB1 = m[8] * m[14] - m[10] * m[12];
        float fB2 = m[8] * m[15] - m[11] * m[12];
        float fB3 = m[9] * m[14] - m[10] * m[13];
        float fB4 = m[9] * m[15] - m[11] * m[13];
        float fB5 = m[10] * m[15] - m[11] * m[14];

        float fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;
        if (Math::Sin(fDet) <= Math::ZeroTolerance)
        {
            return TMatrix4::Zero;
        }

        TMatrix4 kInv;
        kInv[0][0] = +m[5] * fB5 - m[6] * fB4 + m[7] * fB3;
        kInv[1][0] = -m[4] * fB5 + m[6] * fB2 - m[7] * fB1;
        kInv[2][0] = +m[4] * fB4 - m[5] * fB2 + m[7] * fB0;
        kInv[3][0] = -m[4] * fB3 + m[5] * fB1 - m[6] * fB0;
        kInv[0][1] = -m[1] * fB5 + m[2] * fB4 - m[3] * fB3;
        kInv[1][1] = +m[0] * fB5 - m[2] * fB2 + m[3] * fB1;
        kInv[2][1] = -m[0] * fB4 + m[1] * fB2 - m[3] * fB0;
        kInv[3][1] = +m[0] * fB3 - m[1] * fB1 + m[2] * fB0;
        kInv[0][2] = +m[13] * fA5 - m[14] * fA4 + m[15] * fA3;
        kInv[1][2] = -m[12] * fA5 + m[14] * fA2 - m[15] * fA1;
        kInv[2][2] = +m[12] * fA4 - m[13] * fA2 + m[15] * fA0;
        kInv[3][2] = -m[12] * fA3 + m[13] * fA1 - m[14] * fA0;
        kInv[0][3] = -m[9] * fA5 + m[10] * fA4 - m[11] * fA3;
        kInv[1][3] = +m[8] * fA5 - m[10] * fA2 + m[11] * fA1;
        kInv[2][3] = -m[8] * fA4 + m[9] * fA2 - m[11] * fA0;
        kInv[3][3] = +m[8] * fA3 - m[9] * fA1 + m[10] * fA0;

        float fInvDet = ((float)1.0) / fDet;
        for (int iRow = 0; iRow < 4; iRow++)
        {
            for (int iCol = 0; iCol < 4; iCol++)
            {
                kInv[iRow][iCol] *= fInvDet;
            }
        }

        return kInv;
    }

    TMatrix4 Adjoint() const
    {
        float fA0 = m[0] * m[5] - m[1] * m[4];
        float fA1 = m[0] * m[6] - m[2] * m[4];
        float fA2 = m[0] * m[7] - m[3] * m[4];
        float fA3 = m[1] * m[6] - m[2] * m[5];
        float fA4 = m[1] * m[7] - m[3] * m[5];
        float fA5 = m[2] * m[7] - m[3] * m[6];
        float fB0 = m[8] * m[13] - m[9] * m[12];
        float fB1 = m[8] * m[14] - m[10] * m[12];
        float fB2 = m[8] * m[15] - m[11] * m[12];
        float fB3 = m[9] * m[14] - m[10] * m[13];
        float fB4 = m[9] * m[15] - m[11] * m[13];
        float fB5 = m[10] * m[15] - m[11] * m[14];

        TMatrix4 kAdj;
        kAdj[0][0] = +m[5] * fB5 - m[6] * fB4 + m[7] * fB3;
        kAdj[1][0] = -m[4] * fB5 + m[6] * fB2 - m[7] * fB1;
        kAdj[2][0] = +m[4] * fB4 - m[5] * fB2 + m[7] * fB0;
        kAdj[3][0] = -m[4] * fB3 + m[5] * fB1 - m[6] * fB0;
        kAdj[0][1] = -m[1] * fB5 + m[2] * fB4 - m[3] * fB3;
        kAdj[1][1] = +m[0] * fB5 - m[2] * fB2 + m[3] * fB1;
        kAdj[2][1] = -m[0] * fB4 + m[1] * fB2 - m[3] * fB0;
        kAdj[3][1] = +m[0] * fB3 - m[1] * fB1 + m[2] * fB0;
        kAdj[0][2] = +m[13] * fA5 - m[14] * fA4 + m[15] * fA3;
        kAdj[1][2] = -m[12] * fA5 + m[14] * fA2 - m[15] * fA1;
        kAdj[2][2] = +m[12] * fA4 - m[13] * fA2 + m[15] * fA0;
        kAdj[3][2] = -m[12] * fA3 + m[13] * fA1 - m[14] * fA0;
        kAdj[0][3] = -m[9] * fA5 + m[10] * fA4 - m[11] * fA3;
        kAdj[1][3] = +m[8] * fA5 - m[10] * fA2 + m[11] * fA1;
        kAdj[2][3] = -m[8] * fA4 + m[9] * fA2 - m[11] * fA0;
        kAdj[3][3] = +m[8] * fA3 - m[9] * fA1 + m[10] * fA0;

        return kAdj;
    }

    T Determinant() const
    {
        float fA0 = m[0] * m[5] - m[1] * m[4];
        float fA1 = m[0] * m[6] - m[2] * m[4];
        float fA2 = m[0] * m[7] - m[3] * m[4];
        float fA3 = m[1] * m[6] - m[2] * m[5];
        float fA4 = m[1] * m[7] - m[3] * m[5];
        float fA5 = m[2] * m[7] - m[3] * m[6];
        float fB0 = m[8] * m[13] - m[9] * m[12];
        float fB1 = m[8] * m[14] - m[10] * m[12];
        float fB2 = m[8] * m[15] - m[11] * m[12];
        float fB3 = m[9] * m[14] - m[10] * m[13];
        float fB4 = m[9] * m[15] - m[11] * m[13];
        float fB5 = m[10] * m[15] - m[11] * m[14];
        float fDet = fA0 * fB5 - fA1 * fB4 + fA2 * fB3 + fA3 * fB2 - fA4 * fB1 + fA5 * fB0;
        return fDet;
    }

    T QForm(const TVector4<T>& u, const TVector4<T>& v) const
    {
        return u.Dot((*this) * v);
    }

    void SetRotationXYZ(T xAngle, T yAngle, T zAngle)
    {
        xAngle *= Math::Degree2Radian;
        yAngle *= Math::Degree2Radian;
        zAngle *= Math::Degree2Radian;

        float fCos, fSin;
        fCos = Math::Cos(xAngle);
        fSin = Math::Sin(xAngle);
        TMatrix4 kXMat(1.0, 0.0, 0.0, 0.0,
                      0.0, fCos, -fSin, 0.0,
                      0.0, fSin, fCos, 0.0,
                      0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(yAngle);
        fSin = Math::Sin(yAngle);
        TMatrix4 kYMat(fCos, 0.0, fSin, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                     -fSin, 0.0, fCos, 0.0,
                      0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(zAngle);
        fSin = Math::Sin(zAngle);
        TMatrix4 kZMat(fCos, -fSin, 0.0, 0.0,
            fSin, fCos, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);

        *this = kXMat * kYMat * kZMat;
    }

    void SetRotationXZY(T xAngle, T zAngle, T yAngle)
    {
        xAngle *= Math::Degree2Radian;
        yAngle *= Math::Degree2Radian;
        zAngle *= Math::Degree2Radian;

        float fCos, fSin;
        fCos = Math::Cos(xAngle);
        fSin = Math::Sin(xAngle);
        TMatrix4 kXMat(1.0, 0.0, 0.0, 0.0,
            0.0, fCos, -fSin, 0.0,
            0.0, fSin, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(yAngle);
        fSin = Math::Sin(yAngle);
        TMatrix4 kYMat(fCos, 0.0, fSin, 0.0,
            0.0, 1.0, 0.0, 0.0,
            -fSin, 0.0, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(zAngle);
        fSin = Math::Sin(zAngle);
        TMatrix4 kZMat(fCos, -fSin, 0.0, 0.0,
            fSin, fCos, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);

        *this = kXMat * kZMat * kYMat;

        return *this;
    }

    void SetRotationYXZ(T yAngle, T xAngle, T zAngle)
    {
        xAngle *= Math::Degree2Radian;
        yAngle *= Math::Degree2Radian;
        zAngle *= Math::Degree2Radian;

        float fCos, fSin;
        fCos = Math::Cos(xAngle);
        fSin = Math::Sin(xAngle);
        TMatrix4 kXMat(1.0, 0.0, 0.0, 0.0,
            0.0, fCos, -fSin, 0.0,
            0.0, fSin, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(yAngle);
        fSin = Math::Sin(yAngle);
        TMatrix4 kYMat(fCos, 0.0, fSin, 0.0,
            0.0, 1.0, 0.0, 0.0,
            -fSin, 0.0, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(zAngle);
        fSin = Math::Sin(zAngle);
        TMatrix4 kZMat(fCos, -fSin, 0.0, 0.0,
            fSin, fCos, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);

        *this = kYMat * kXMat * kZMat;

        return *this;
    }

    void SetRotationYZX(T yAngle, T zAngle, T xAngle)
    {
        xAngle *= Math::Degree2Radian;
        yAngle *= Math::Degree2Radian;
        zAngle *= Math::Degree2Radian;

        float fCos, fSin;
        fCos = Math::Cos(xAngle);
        fSin = Math::Sin(xAngle);
        TMatrix4 kXMat(1.0, 0.0, 0.0, 0.0,
            0.0, fCos, -fSin, 0.0,
            0.0, fSin, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(yAngle);
        fSin = Math::Sin(yAngle);
        TMatrix4 kYMat(fCos, 0.0, fSin, 0.0,
            0.0, 1.0, 0.0, 0.0,
            -fSin, 0.0, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(zAngle);
        fSin = Math::Sin(zAngle);
        TMatrix4 kZMat(fCos, -fSin, 0.0, 0.0,
            fSin, fCos, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);

        *this = kYMat * kZMat * kXMat;

        return *this;
    }

    void SetRotationZXY(T zAngle, T xAngle, T yAngle)
    {
        xAngle *= Math::Degree2Radian;
        yAngle *= Math::Degree2Radian;
        zAngle *= Math::Degree2Radian;

        float fCos, fSin;
        fCos = Math::Cos(xAngle);
        fSin = Math::Sin(xAngle);
        TMatrix4 kXMat(1.0, 0.0, 0.0, 0.0,
            0.0, fCos, -fSin, 0.0,
            0.0, fSin, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(yAngle);
        fSin = Math::Sin(yAngle);
        TMatrix4 kYMat(fCos, 0.0, fSin, 0.0,
            0.0, 1.0, 0.0, 0.0,
            -fSin, 0.0, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(zAngle);
        fSin = Math::Sin(zAngle);
        TMatrix4 kZMat(fCos, -fSin, 0.0, 0.0,
            fSin, fCos, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);

        *this = kZMat * kXMat * kYMat;
    }

    void SetRotationZYX(T zAngle, T yAngle, T xAngle)
    {
        xAngle *= Math::Degree2Radian;
        yAngle *= Math::Degree2Radian;
        zAngle *= Math::Degree2Radian;

        float fCos, fSin;
        fCos = Math::Cos(xAngle);
        fSin = Math::Sin(xAngle);
        TMatrix4 kXMat(1.0, 0.0, 0.0, 0.0,
            0.0, fCos, -fSin, 0.0,
            0.0, fSin, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(yAngle);
        fSin = Math::Sin(yAngle);
        TMatrix4 kYMat(fCos, 0.0, fSin, 0.0,
            0.0, 1.0, 0.0, 0.0,
            -fSin, 0.0, fCos, 0.0,
            0.0, 0.0, 0.0, 1.0);

        fCos = Math::Cos(zAngle);
        fSin = Math::Sin(zAngle);
        TMatrix4 kZMat(fCos, -fSin, 0.0, 0.0,
            fSin, fCos, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0);

        *this = kZMat * kYMat * kXMat;
    }

    bool GetRotationXYZ(T& xAngle, T& yAngle, T& zAngle) const
    {
        // rot =  cy*cz          -cy*sz           sy
        //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
        //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy
        if (m[2] < 1.0)
        {
            if (m[2] > -1.0)
            {
                xAngle = Math::ATan2(-m[6], m[10]) * Math::Degree2Radian;
                yAngle = Math::ASin(m[2]) * Math::Degree2Radian;
                zAngle = Math::ATan2(-m[1], m[0]) * Math::Degree2Radian;
                return true;
            }
            else
            {
                // WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
                xAngle = -Math::ATan2(m[4], m[5]) * Math::Degree2Radian;
                yAngle = -Math::HalfPi * Math::Degree2Radian;
                zAngle = 0.0 * Math::Degree2Radian;
                return false;
            }
        }
        else
        {
            // WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
            xAngle = Math::ATan2(m[4], m[5]) * Math::Degree2Radian;
            yAngle = Math::HalfPi * Math::Degree2Radian;
            zAngle = 0.0 * Math::Degree2Radian;
            return false;
        }
    }

    bool GetRotationXZY(T& xAngle, T& zAngle, T& yAngle) const
    {
        // rot =  cy*cz          -sz              cz*sy
        //        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
        //       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz
        if (m[1] < 1.0)
        {
            if (m[1] > -1.0)
            {
                xAngle = Math::ATan2(m[9], m[5]) * Math::Degree2Radian;
                zAngle = Math::Asin(-m[1]) * Math::Degree2Radian;
                yAngle = Math::ATan2(m[2], m[0]) * Math::Degree2Radian;
                return true;
            }
            else
            {
                // WARNING.  Not unique.  XA - YA = atan2(r20,r22)
                xAngle = Math::ATan2(m[8], m[10]) * Math::Degree2Radian;
                zAngle = Math::HalfPi * Math::Degree2Radian;
                yAngle = 0.0 * Math::Degree2Radian;
                return false;
            }
        }
        else
        {
            // WARNING.  Not unique.  XA + YA = atan2(-r20,r22)
            xAngle = Math::ATan2(-m[8], m[10]) * Math::Degree2Radian;
            zAngle = -Math::HalfPi * Math::Degree2Radian;
            yAngle = 0.0 * Math::Degree2Radian;
            return false;
        }
    }

    bool GetRotationYXZ(T& yAngle, T& xAngle, T& zAngle) const
    {
        // rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
        //        cx*sz           cx*cz          -sx
        //       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

        if (m[6] < 1.0)
        {
            if (m[6] > -1.0)
            {
                yAngle = Math::ATan2(m[2], m[10]) * Math::Degree2Radian;
                xAngle = Math::ASin(-m[6]) * Math::Degree2Radian;
                zAngle = Math::ATan2(m[4], m[5]) * Math::Degree2Radian;
                return true;
            }
            else
            {
                // WARNING.  Not unique.  YA - ZA = atan2(r01,r00)
                yAngle = Math::ATan2(m[1], m[0]) * Math::Degree2Radian;
                xAngle = Math::HalfPi * Math::Degree2Radian;
                zAngle = 0.0 * Math::Degree2Radian;
                return false;
            }
        }
        else
        {
            // WARNING.  Not unique.  YA + ZA = atan2(-r01,r00)
            yAngle = Math::ATan2(-m[1], m[0]) * Math::Degree2Radian;
            xAngle = -Math::HalfPi * Math::Degree2Radian;
            zAngle = 0.0 * Math::Degree2Radian;
            return false;
        }
    }

    bool GetRotationYZX(T& yAngle, T& zAngle, T& xAngle) const
    {
        // rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
        //        sz              cx*cz          -cz*sx
        //       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

        if (m[4] < 1.0)
        {
            if (m[4] > -1.0)
            {
                yAngle = Math::ATan2(-m[8], m[0]) * Math::Degree2Radian;
                zAngle = Math::Asin(m[4]) * Math::Degree2Radian;
                xAngle = Math::ATan2(-m[6], m[5]) * Math::Degree2Radian;
                return true;
            }
            else
            {
                // WARNING.  Not unique.  YA - XA = -atan2(r21,r22);
                yAngle = -Math::ATan2(m[9], m[10]) * Math::Degree2Radian;
                zAngle = -Math::HalfPi * Math::Degree2Radian;
                xAngle = 0.0 * Math::Degree2Radian;
                return false;
            }
        }
        else
        {
            // WARNING.  Not unique.  YA + XA = atan2(r21,r22)
            yAngle = Math::ATan2(m[9], m[10]) * Math::Degree2Radian;
            zAngle = Math::HalfPi * Math::Degree2Radian;
            xAngle = 0.0 * Math::Degree2Radian;
            return false;
        }
    }

    bool GetRotationZXY(T& zAngle, T& xAngle, T& yAngle) const
    {
        // rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
        //        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
        //       -cx*sy           sx              cx*cy

        if (m[9] < 1.0)
        {
            if (m[9] > -1.0)
            {
                zAngle = Math::ATan2(-m[1], m[5]) * Math::Degree2Radian;
                xAngle = Math::Asin(m[9]) * Math::Degree2Radian;
                yAngle = Math::ATan2(-m[8], m[10]) * Math::Degree2Radian;
                return true;
            }
            else
            {
                // WARNING.  Not unique.  ZA - YA = -atan(r02,r00)
                zAngle = -Math::ATan2(m[2], m[0]) * Math::Degree2Radian;
                xAngle = -Math::HalfPi * Math::Degree2Radian;
                yAngle = 0.0 * Math::Degree2Radian;
                return false;
            }
        }
        else
        {
            // WARNING.  Not unique.  ZA + YA = atan2(r02,r00)
            zAngle = Math::ATan2(m[2], m[0]) * Math::Degree2Radian;
            xAngle = Math::HalfPi * Math::Degree2Radian;
            yAngle = 0.0 * Math::Degree2Radian;
            return false;
        }
    }

    bool GetRotationZYX(T& zAngle, T& yAngle, T& xAngle) const
    {
        // rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
        //        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
        //       -sy              cy*sx           cx*cy

        if (m[8] < 1.0)
        {
            if (m[8] > -1.0)
            {
                zAngle = Math::ATan2(m[4], m[0]) * Math::Degree2Radian;
                yAngle = Math::Asin(-m[8]) * Math::Degree2Radian;
                xAngle = Math::ATan2(m[9], m[10]) * Math::Degree2Radian;
                return true;
            }
            else
            {
                // WARNING.  Not unique.  ZA - XA = -atan2(r01,r02)
                zAngle = -Math::ATan2(m[1], m[2]) * Math::Degree2Radian;
                yAngle = Math::HalfPi * Math::Degree2Radian;
                xAngle = 0.0 * Math::Degree2Radian;
                return false;
            }
        }
        else
        {
            // WARNING.  Not unique.  ZA + XA = atan2(-r01,-r02)
            zAngle = Math::ATan2(-m[1], -m[2]) * Math::Degree2Radian;
            yAngle = -Math::HalfPi * Math::Degree2Radian;
            xAngle = 0.0 * Math::Degree2Radian;
            return false;
        }
    }

    static const TMatrix4 Zero;
    static const TMatrix4 Identity;

    void Read(InputStream& is)
    {
        is.ReadBuffer(&m[0], sizeof(T) * 16);
    }

    void Write(OutputStream& os) const
    {
        os.WriteBuffer(&m[0], sizeof(T) * 16);
    }
private:
    // for indexing into the 1D array of the matrix, iCol+N*iRow
    static int I(int row, int col)
    {
        assert(0 <= row && row < 4 && 0 <= col && col < 4);

        // return iCol + 4*iRow;
        return col + RowStartIdxs[row];
    }

    T m[16];
};

typedef TMatrix4<bool> BMatrix4;
typedef TMatrix4<int> IMatrix4;
typedef TMatrix4<float> Matrix4;
typedef TMatrix4<double> DMatrix4;

#endif