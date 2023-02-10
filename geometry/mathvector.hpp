#ifndef MATHVECTOR_HPP
#define MATHVECTOR_HPP

#include "mathvector.h"


namespace GeometrySpace
{
    template <typename T>
    Vector3D<T>::Vector3D()
        : x_(0),
          y_(0),
          z_(0)
    {
    }

    template<typename T>
    Vector3D<T>::Vector3D(const T &x, const T &y, const T &z)
        : x_(x),
          y_(y),
          z_(z)
    {
    }

    template <typename T>
    Vector3D<T>::Vector3D(const Vector3D<T>& other)
        : x_(other.x()),
          y_(other.y()),
          z_(other.z())
    {
    }

    template <typename T>
    Vector3D<T>::Vector3D(const Vector4D<T>& other)
        : x_(other.x()),
          y_(other.y()),
          z_(other.z())
    {
        if (fabs(other.w()) > EPS)
        {
            x_ /= other.w();
            y_ /= other.w();
            z_ /= other.w();
        }
    }

    template <typename T>
    Vector3D<T>::Vector3D(std::initializer_list<T> list)
    {

        if (list.size() == 3)
        {
            size_t i = 0;
            for (const auto &el: list)
            {
                (*this)[i] = el;
                i++;
            }
        }
        else
        {
            throw "Invalid list size\n";
        }
    }

    template <typename T>
    Vector3D<T> &Vector3D<T>::operator = (const Vector3D<T>& other)
    {
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
        return *this;
    }

    template <typename T>
    Vector3D<T> &Vector3D<T>::operator = (const Vector4D<T>& other)
    {
        if (fabs(other.w()) > EPS)
        {
            x_ = other.x() / other.w();
            y_ = other.y() / other.w();
            z_ = other.z() / other.w();
        }
        else
        {
            x_ = other.x();
            y_ = other.y();
            z_ = other.z();
        }
        return *this;
    }

    template<typename T>
    Vector3D<T> Vector3D<T>::operator + (const Vector3D<T> &other) const
    {
        Vector3D<T> result(*this);
        result.x_ += other.x();
        result.y_ += other.y();
        result.z_ += other.z();
        return result;
    }

    template<typename T>
    Vector3D<T> Vector3D<T>::operator - (const Vector3D<T> &other) const
    {
        Vector3D<T> result(*this);
        result.x_ -= other.x();
        result.y_ -= other.y();
        result.z_ -= other.z();
        return result;
    }

    template<typename T>
    Vector3D<T> &Vector3D<T>::operator += (const Vector3D<T> &other)
    {
        x_ += other.x();
        y_ += other.y();
        z_ += other.z();
        return *this;
    }

    template<typename T>
    Vector3D<T> &Vector3D<T>::operator -= (const Vector3D<T> &other)
    {
        x_ -= other.x();
        y_ -= other.y();
        z_ -= other.z();
        return *this;
    }

    template<typename T>
    Vector3D<T> Vector3D<T>::operator * (double f) const
    {
        Vector3D<T> result(*this);
        result.x_ *= f;
        result.y_ *= f;
        result.z_ *= f;
        return result;
    }

    template<typename T>
    T &Vector3D<T>::operator [](int index)
    {
        if (index == 0) return x_;
        if (index == 1) return y_;
        if (index == 2) return z_;
        throw std::out_of_range("Index out of range");
    }

    template<typename T>
    const T &Vector3D<T>::operator [](int index) const
    {
        if (index == 0) return x_;
        if (index == 1) return y_;
        if (index == 2) return z_;
        throw std::out_of_range("Index out of range");
    }

    template<typename T>
    T Vector3D<T>::operator * (const Vector3D<T> &other) const
    {
        return x_ * other.x() + y_ * other.y() + z_ * other.z();
    }

    template <typename T>
    Vector3D<T> Vector3D<T>::operator ^ (const Vector3D<T>& other) const
    {
        Vector3D result;
        result.x_ = y_ * other.z() - z_ * other.y();
        result.y_ = z_ * other.x() - x_ * other.z();
        result.z_ = x_ * other.y() - y_ * other.x();
        return result;
    }

    template <typename T>
    double Vector3D<T>::module() const
    {
        return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    }

    template <typename T>
    void Vector3D<T>::normalize()
    {
        double mod = module();
        x_ /= mod;
        y_ /= mod;
        z_ /= mod;
    }

    template<typename T>
    Vector3D<T> Vector3D<T>::operator * (const Matrix<T> &other) const
    {
        if ((other.countRows() == 3 && other.countCols() == 3) ||
            (other.countRows() == 4 && other.countCols() == 4))
        {
            Vector3D<T> result;
            result.x_ = x_ * other[0][0] + y_ * other[1][0] + z_ * other[2][0];
            result.y_ = x_ * other[0][1] + y_ * other[1][1] + z_ * other[2][1];
            result.z_ = x_ * other[0][2] + y_ * other[1][2] + z_ * other[2][2];
            return result;
        }
        throw("Invalid matrix size");
    }

    template<typename T>
    Vector3D<T> Vector3D<T>::operator ^ (const Matrix<T> &other) const
    {
        if (other.countRows() == 4 && other.countCols() == 4)
        {
            Vector3D<T> result;
            result.x_ = x_ * other[0][0] + y_ * other[1][0] + z_ * other[2][0] + other[3][0];
            result.y_ = x_ * other[0][1] + y_ * other[1][1] + z_ * other[2][1] + other[3][1];
            result.z_ = x_ * other[0][2] + y_ * other[1][2] + z_ * other[2][2] + other[3][2];
            return result;
        }
        throw("Invalid matrix size");
    }

    template <typename T>
    Vector4D<T>::Vector4D()
        : x_(0),
          y_(0),
          z_(0),
          w_(0)
    {
    }

    template<typename T>
    Vector4D<T>::Vector4D(const T &x, const T &y, const T &z, const T &w)
        : x_(x),
          y_(y),
          z_(z),
          w_(w)
    {
    }

    template <typename T>
    Vector4D<T>::Vector4D(const Vector3D<T>& other)
        : x_(other.x()),
          y_(other.y()),
          z_(other.z()),
          w_(0)
    {
    }

    template <typename T>
    Vector4D<T>::Vector4D(const Vector3D<T>& other, const T &w)
        : x_(other.x()),
          y_(other.y()),
          z_(other.z()),
          w_(w)
    {
    }

    template <typename T>
    Vector4D<T>::Vector4D(const Vector4D<T>& other)
        : x_(other.x()),
          y_(other.y()),
          z_(other.z()),
          w_(other.w())
    {
    }

    template <typename T>
    Vector4D<T>::Vector4D(std::initializer_list<T> list)
    {

        if (list.size() == 4)
        {
            size_t i = 0;
            for (const auto &el: list)
            {
                (*this)[i] = el;
                i++;
            }
        }
        else
        {
            throw "Invalid list size\n";
        }
    }

    template <typename T>
    Vector4D<T> &Vector4D<T>::operator = (const Vector4D<T>& other)
    {
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
        w_ = other.w();
        return *this;
    }

    template <typename T>
    Vector4D<T> &Vector4D<T>::operator = (const Vector3D<T>& other)
    {
        x_ = other.x();
        y_ = other.y();
        z_ = other.z();
        w_ = 0;
        return *this;
    }

    template<typename T>
    Vector4D<T> Vector4D<T>::operator + (const Vector4D<T> &other) const
    {
        Vector4D result(*this);
        result.x_ += other.x();
        result.y_ += other.y();
        result.z_ += other.z();
        result.w_ += other.w();
        return result;
    }

    template<typename T>
    Vector4D<T> Vector4D<T>::operator - (const Vector4D<T> &other) const
    {
        Vector4D result(*this);
        result.x_ -= other.x();
        result.y_ -= other.y();
        result.z_ -= other.z();
        result.w_ -= other.w();
        return result;
    }

    template<typename T>
    Vector4D<T> &Vector4D<T>::operator += (const Vector4D<T> &other)
    {
        x_ += other.x();
        y_ += other.y();
        z_ += other.z();
        w_ += other.w();
        return *this;
    }

    template<typename T>
    Vector4D<T> &Vector4D<T>::operator -= (const Vector4D<T> &other)
    {
        x_ -= other.x();
        y_ -= other.y();
        z_ -= other.z();
        w_ -= other.w();
        return *this;
    }

    template<typename T>
    Vector4D<T> Vector4D<T>::operator * (double f) const
    {
        Vector4D<T> result(*this);
        result.x_ *= f;
        result.y_ *= f;
        result.z_ *= f;
        result.w_ *= f;
        return result;
    }

    template<typename T>
    T &Vector4D<T>::operator [](int index)
    {
        if (index == 0) return x_;
        if (index == 1) return y_;
        if (index == 2) return z_;
        if (index == 3) return w_;
        throw std::out_of_range("Index out of range");
    }

    template<typename T>
    const T &Vector4D<T>::operator [](int index) const
    {
        if (index == 0) return x_;
        if (index == 1) return y_;
        if (index == 2) return z_;
        if (index == 3) return w_;
        throw std::out_of_range("Index out of range");
    }

    template<typename T>
    T Vector4D<T>::operator * (const Vector4D<T> &other) const
    {
        return x_ * other.x() + y_ * other.y() + z_ * other.z() + w_ * other.w();
    }

    template <typename T>
    double Vector4D<T>::module() const
    {
        return sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
    }

    template <typename T>
    void Vector4D<T>::normalize()
    {
        double mod = module();
        x_ /= mod;
        y_ /= mod;
        z_ /= mod;
        w_ /= mod;
    }

    template<typename T>
    Vector4D<T> Vector4D<T>::operator * (const Matrix<T> &other) const
    {
        if (other.countRows() == 4 && other.countCols() == 4)
        {
            Vector4D<T> result;
            result.x_ = x_ * other[0][0] + y_ * other[1][0] + z_ * other[2][0] + w_ * other[3][0];
            result.y_ = x_ * other[0][1] + y_ * other[1][1] + z_ * other[2][1] + w_ * other[3][1];
            result.z_ = x_ * other[0][2] + y_ * other[1][2] + z_ * other[2][2] + w_ * other[3][2];
            result.w_ = x_ * other[0][3] + y_ * other[1][3] + z_ * other[2][3] + w_ * other[3][3];
            return result;
        }
        throw("Invalid matrix size");
    }
}

#endif // MATHVECTOR_HPP
