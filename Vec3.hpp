//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#pragma once

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

    inline Vec3 operator+ (const Vec3 & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] + p[0];
        res[1] = m_vec[1] + p[1];
        res[2] = m_vec[2] + p[2];
        return (res);
    };

    inline Vec3& operator- (const Vec3 & p) const
    {
        Vec3 res;
        res[0] = m_vec[0] - p[0];
        res[1] = m_vec[1] - p[1];
        res[2] = m_vec[2] - p[2];
        return (res);
    };

private:

    Type m_vec[3];

};

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
using Vec3i = Vec3<int>;