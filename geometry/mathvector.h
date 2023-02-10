#ifndef MATHVECTOR_H
#define MATHVECTOR_H

#include "geometry.h"
#include <iostream>

namespace GeometrySpace
{
    template <typename T>
    class Vector3D
    {
    private:
        T x_;
        T y_;
        T z_;
    public:
        inline T x() const { return x_; }
        inline T y() const { return y_; }
        inline T z() const { return z_; }

        inline void setX(const T &x) { x_ = x; }
        inline void setY(const T &y) { y_ = y; }
        inline void setZ(const T &z) { z_ = z; }

        Vector3D();
        Vector3D(const T &x, const T &y, const T &z);
        Vector3D(const Vector3D<T> &other);
        Vector3D(const Vector4D<T> &other);
        Vector3D(std::initializer_list<T> list);

        Vector3D<T> &operator = (const Vector4D<T> &point);
        Vector3D<T> &operator = (const Vector3D<T> &other);

        Vector3D<T> operator + (const Vector3D<T> &other) const;
        Vector3D<T> operator - (const Vector3D<T> &other) const;

        Vector3D<T> &operator += (const Vector3D<T> &other);
        Vector3D<T> &operator -= (const Vector3D<T> &other);

        Vector3D<T> operator * (double f) const;
        Vector3D<T> operator * (const Matrix<T> &other) const;
        Vector3D<T> operator ^ (const Matrix<T> &other) const;
        T operator * (const Vector3D<T> &other) const;
        Vector3D<T> operator ^ (const Vector3D<T>& other) const;

        T &operator [] (int index);
        const T &operator [] (int index) const;

        double module() const;
        void normalize();
    };

    template <typename T>
    class Vector4D
    {
    private:
        T x_;
        T y_;
        T z_;
        T w_;
    public:
        inline T x() const { return x_; }
        inline T y() const { return y_; }
        inline T z() const { return z_; }
        inline T w() const { return w_; }

        inline void setX(const T &x) { x_ = x; }
        inline void setY(const T &y) { y_ = y; }
        inline void setZ(const T &z) { z_ = z; }
        inline void setW(const T &w) { w_ = w; }

        Vector4D();
        Vector4D(const T &x, const T &y, const T &z, const T &w);
        Vector4D(const Vector3D<T> &other);
        Vector4D(const Vector3D<T> &other, const T &w);
        Vector4D(const Vector4D<T> &other);
        Vector4D(std::initializer_list<T> list);

        Vector4D<T> &operator = (const Vector4D<T> &point);
        Vector4D<T> &operator = (const Vector3D<T> &other);

        Vector4D<T> operator + (const Vector4D<T> &other) const;
        Vector4D<T> operator - (const Vector4D<T> &other) const;

        Vector4D<T> &operator += (const Vector4D<T> &other);
        Vector4D<T> &operator -= (const Vector4D<T> &other);

        Vector4D<T> operator * (double f) const;
        Vector4D<T> operator * (const Matrix<T> &other) const;

        T operator * (const Vector4D<T> &other) const;

        T &operator [] (int index);
        const T &operator [] (int index) const;

        double module() const;
        void normalize();
    };
}
#endif // MATHVECTOR_H
