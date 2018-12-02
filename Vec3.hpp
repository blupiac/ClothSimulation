//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <math.h>

template <typename Type>
class Vec3 
{

public:

    inline Vec3()
    {
        m_vec[0] = m_vec[1] = m_vec[2] = 0.0;
    }

    inline Vec3(Type a, Type b, Type c)
    {
        m_vec[0] = a;
        m_vec[1] = b;
        m_vec[2] = c;
    }

    ~Vec3() {}

    inline const Type& operator[] (int idx) const
    {
        return m_vec[idx];
    };

    inline Type& operator[] (int idx)
    {
        return m_vec[idx];
    };

    inline const Vec3 operator+ (const Vec3 & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] + p[0];
        res[1] = m_vec[1] + p[1];
        res[2] = m_vec[2] + p[2];
        return (res);
    };

    inline const Vec3 operator+ (const Type & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] + p;
        res[1] = m_vec[1] + p;
        res[2] = m_vec[2] + p;
        return (res);
    };

    inline const Vec3 operator- (const Vec3 & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] - p[0];
        res[1] = m_vec[1] - p[1];
        res[2] = m_vec[2] - p[2];
        return (res);
    };

    inline const Vec3 operator- (const Type & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] - p;
        res[1] = m_vec[1] - p;
        res[2] = m_vec[2] - p;
        return (res);
    };

    inline const Vec3 operator* (const Vec3 & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] * p[0];
        res[1] = m_vec[1] * p[1];
        res[2] = m_vec[2] * p[2];
        return (res);
    };

    inline const Vec3 operator* (const Type & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] * p;
        res[1] = m_vec[1] * p;
        res[2] = m_vec[2] * p;
        return (res);
    };

    inline const Vec3 operator/ (const Vec3 & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] / p[0];
        res[1] = m_vec[1] / p[1];
        res[2] = m_vec[2] / p[2];
        return (res);
    };

    inline const Vec3 operator/ (const Type & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] / p;
        res[1] = m_vec[1] / p;
        res[2] = m_vec[2] / p;
        return (res);
    };

    inline const float dot (const Vec3 & p) const
    {
        float res = 0.0f;
        res += m_vec[0] * p[0];
        res += m_vec[1] * p[1];
        res += m_vec[2] * p[2];
        return (res);
    };

    inline const Vec3 cross (const Vec3 & p) const
    {
        Vec3 res;
        res[0] = m_vec[1] * p[2] - m_vec[2] * p[1];
        res[1] = m_vec[2] * p[0] - m_vec[0] * p[2];
        res[2] = m_vec[0] * p[1] - m_vec[1] * p[0];
        return (res);
    };

    inline const Vec3 normalize () const
    {
        Vec3 res;
        float magnitude = std::sqrt(m_vec[0] * m_vec[0] +
                                    m_vec[1] * m_vec[1] + 
                                    m_vec[2] * m_vec[2]);

        if(magnitude != 0.0f)
        {
            res[0] = m_vec[0] / magnitude;
            res[1] = m_vec[1] / magnitude;
            res[2] = m_vec[2] / magnitude;
            return (res);
        }
        else
        {
            res[0] = m_vec[0];
            res[1] = m_vec[1];
            res[2] = m_vec[2];
            return (res);
        }
    };

private:

    Type m_vec[3];

};

template <class Type>
std::ostream & operator<< (std::ostream & output, const Vec3<Type> & v)
{
    output << std::fixed << v[0] << " " << v[1] << " " << v[2];
    return output;
}

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;