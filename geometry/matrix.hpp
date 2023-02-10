#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "matrix.h"

namespace GeometrySpace
{
    template <typename T>
    Matrix<T>::Matrix(const int &rows, const int &cols)
        : _rows(rows), _cols(cols)
    {
        try
        {
            data = new T* [_rows];
            for (int i = 0; i < _rows; i++)
            {
                data[i] = new T[_cols];
                for (int j = 0; j < _cols; j++)
                    data[i][j] = 0;
            }
        }
        catch (std::bad_alloc)
        {
            throw "Matrix allocation error";
        }
    }

    template <typename T>
    Matrix<T>::Matrix(const Matrix<T> &other)
        : _rows(other.countRows()), _cols(other.countCols())
    {
        try
        {
            data = new T* [_rows];
            for (int i = 0; i < _rows; i++)
            {
                data[i] = new T[_cols];
                for (int j = 0; j < _cols; j++)
                    data[i][j] = other[i][j];
            }
        }
        catch (std::bad_alloc)
        {
            throw "Matrix allocation error";
        }
    }

    template <typename T>
    Matrix<T>::~Matrix()
    {
        for (int i = 0; i < _rows; i++)
        {
            delete [] data[i];
        }
        delete [] data;
    }

    template <typename T>
    void Matrix<T>::makeIdentity()
    {
        if (_rows == _cols)
        {
            for (int i = 0; i < _rows; i++)
            {
                for (int j = 0; j < _cols; j++)
                {
                    if (i == j)
                        data[i][j] = 1;
                    else
                        data[i][j] = 0;
                }
            }
        }
        else
            throw "Not a square matrix";
    }

    template <typename T>
    T* Matrix<T>::operator [] (int index)
    {
        if (index < _rows)
            return data[index];

        throw std::out_of_range("Index out of range");
    }

    template <typename T>
    const T* Matrix<T>::operator [] (const int &index) const
    {
        if (index < _rows)
            return data[index];

        throw std::out_of_range("Index out of range");
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator * (const Matrix &other) const
    {
        if (_cols == other.countRows())
        {
            Matrix result(_rows, _cols);
            for (int i = 0; i < _rows; i++)
            {
                for (int j = 0; j < other.countCols(); j++)
                {
                    for (int k = 0; k < _cols; k++)
                    {
                        result[i][j] += (*this)[i][k] * other[k][j];
                    }
                }
            }
            return result;
        }

        throw "Incompatible sizes of the matrices";
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator + (const Matrix &other) const
    {
        if (_cols == other.countCols() && _rows == other.countRows())
        {
            Matrix result(*this);
            for (int i = 0; i < _rows; i++)
                for (int j = 0; j < _cols; j++)
                    result[i][j] += other[i][j];
            return result;
        }

        throw "Incompatible sizes of the matrices";
    }

    template <typename T>
    Matrix<T> Matrix<T>::operator - (const Matrix &other) const
    {
        if (_cols == other.countCols() && _rows == other.countRows())
        {
            Matrix result(*this);
            for (int i = 0; i < _rows; i++)
                for (int j = 0; j < _cols; j++)
                    result[i][j] -= other[i][j];
            return result;
        }

        throw "Incompatible sizes of the matrices";
    }

    template <typename T>
    void Matrix<T>::transpose()
    {
        for (int i = 0; i < _rows; i++)
        {
            for (int j = 0; j < _cols; j++)
            {
                T tmp = data[j][i];
                data[j][i] = data[i][j];
                data[i][j] = tmp;
            }
        }
    }

//    template <typename T>
//    void Matrix<T>::inverse()
//    {
//        if (_rows == _cols)
//        {
//            // Getting (A | E) Matrix
//            Matrix result(_rows, _cols * 2);
//            for (int i = 0; i < _rows; i++)
//                for (int j = 0; j < _cols; j++)
//                    result[i][j] = data[i][j];
//            for (int i = 0; i < _rows; i++)
//                result[i][i + _cols] = 1;

//            // Zeroing lower triangle
//            for (int i = 0; i < _rows - 1; i++)
//            {
//                for (int j = result.countCols() - 1; j >= 0; j--)
//                    result[i][j] /= result[i][i];
//                for (int k = i + 1; k < _rows; k++)
//                {
//                    double coef = result[k][i];
//                    for (int j = 0; j < result.countCols(); j++)
//                        result[k][j] -= result[i][j] * coef;
//                }
//            }

//            // Zeroing upper triangle
//            for (int j = result.countCols() - 1; j >= _rows - 1; j--)
//                result[_rows - 1][j] /= result[_rows - 1][_rows - 1];

//            for (int i = _rows - 1; i > 0; i--)
//            {
//                for (int k = i - 1; k >= 0; k--)
//                {
//                    double coef = result[k][i];
//                    for (int j = k + 2; j < result.countCols(); j++)
//                        result[k][j] -= result[i][j] * coef;
//                }
//            }

//            for (int i = 0; i < _rows; i++)
//                for (int j = 0; j < _cols; j++)
//                    data[i][j] = result[i][j + _cols];
//        }
//        else
//        {
//            throw "Not a square matrix";
//        }
//    }

    template <typename T>
    bool Matrix<T>::isZero()
    {
        for (int i = 0; i < _rows; i++)
        {
            for (int j = 0; j < _cols; j++)
            {
                if (data[i][j] != 0)
                    return false;
            }
        }
        return true;
    }

    template <typename T>
    Matrix<T> &Matrix<T>::operator = (const Matrix &other)
    {
        if (this == &other) return *this;
        try
        {
            if (other._rows != _rows || other._cols != _cols)
            {
                for (int i = 0; i < _rows; i++)
                {
                    delete [] data[i];
                }
                delete [] data;
                _rows = other._rows;
                _cols = other._cols;
                data = new T* [_rows];
            }
            for (int i = 0; i < _rows; i++)
            {
                data[i] = new T[_cols];
                for (int j = 0; j < _cols; j++)
                    data[i][j] = other[i][j];
            }

        }
        catch(std::bad_alloc)
        {
            throw "Allocation error";
        }
        return *this;
    }

    template <typename T>
    Matrix<T> Matrix<T>::getCofactor(const int &p, const int &q, const int &n)
    {
        int i = 0, j = 0;
        Matrix<T> temp(4, 4);
        // Looping for each element of the matrix
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                //  Copying into temporary matrix only those element
                //  which are not in given row and column
                if (row != p && col != q)
                {
                    temp[i][j++] = data[row][col];

                    // Row is filled, so increase row index and
                    // reset col index
                    if (j == n - 1)
                    {
                        j = 0;
                        i++;
                    }
                }
            }
        }
        return temp;
    }

    template <typename T>
    T Matrix<T>::getDeterminant(Matrix<T> matrix, const int &n)
    {
        T D = 0;
        if (n == 1) return matrix[0][0];
        Matrix<T> temp(4, 4); // To store cofactors

        int sign = 1;  // To store sign multiplier

        // Iterate for each element of first row
        for (int f = 0; f < n; f++)
        {
            // Getting Cofactor of A[0][f]
            temp = matrix.getCofactor(0, f, n);
            D += sign * matrix[0][f] * getDeterminant(temp, n - 1);

            // terms are to be added with alternate sign
            sign = -sign;
        }

        return D;
    }

    template <typename T>
    Matrix<T> Matrix<T>::adjoint()
    {
        // temp is used to store cofactors of A[][]
        int sign = 1;
        Matrix<T> temp(4, 4);
        Matrix<T> adj(4, 4);
        for (int i=0; i<4; i++)
        {
            for (int j=0; j<4; j++)
            {
                // Get cofactor of A[i][j]
                temp = getCofactor(i, j, 4);

                // sign of adj[j][i] positive if sum of row
                // and column indexes is even.
                sign = ((i+j)%2==0)? 1: -1;

                // Interchanging rows and columns to get the
                // transpose of the cofactor matrix
                adj[j][i] = (sign)*(getDeterminant(temp, 3));
            }
        }
        return adj;
    }

    // Function to calculate and store inverse, returns false if
    // matrix is singular

    template <typename T>
    void Matrix<T>::inverse()
    {
        // Find determinant of A[][]
        T det = getDeterminant(*this, 4);

        // Find adjoint
        Matrix<T> adj(4,4);
        adj = adjoint();

        // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
        for (int i=0; i<4; i++)
            for (int j=0; j<4; j++)
                data[i][j] = adj[i][j]/float(det);
    }

}

#endif // MATRIX_HPP
