#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>
#include <iostream>

#include "vec.hpp"

using std::vector;

template <class T> class Matrix {

    // matrix is a vector of vectors representing columns
    typedef vector<Vec<T>> container_type;

public:
    typedef typename container_type::value_type column_type;
    typedef typename column_type::size_type size_type;
    typedef typename container_type::pointer pointer;
    typedef typename container_type::const_pointer const_pointer;
    typedef typename column_type::value_type value_type;

private:
    size_type m_rows = 0, m_columns = 0;
    container_type m_data;

public:
    // constructors
    Matrix<T> (void) = default;
    Matrix<T> (size_type rows, size_type cols, value_type value = T());
    explicit Matrix<T>(size_type rows, size_type cols, container_type values);

    // elements access
    column_type operator [] (size_type);

    // size access
    size_type rows (void) const;
    size_type columns (void) const;

    // row and column access
    column_type col (size_type j) const;
    column_type row (size_type i) const;

    // row insertion
    void insert_row (size_type i, column_type const & row);

    pointer data (void);
    const_pointer data (void) const;

};

template <class T>
Matrix<T>::Matrix (size_type rows, size_type cols, value_type value)
    : m_rows (rows), m_columns (cols), m_data (cols, Vec<T>(rows, value)) {}
 
template <class T>
Matrix<T>::Matrix(size_type rows, size_type cols, container_type values)
    : m_rows (rows), m_columns (cols), m_data (values) {
    // check that the dimensions are correct
    if (m_rows * m_columns != m_data.size() * m_data[0].size())
        throw std::invalid_argument ("Matrix::Matrix: wrong size");
}

// elements access
template <class T>
typename Matrix<T>::column_type Matrix<T>::operator [] (size_type j) {
    return m_data[j];
}

template <class T>
typename Matrix<T>::size_type Matrix<T>::rows (void) const {
    return m_rows;
}

template <class T>
typename Matrix<T>::size_type Matrix<T>::columns (void) const {
    return m_columns;
}

template <class T>
typename Matrix<T>::column_type Matrix<T>::row(size_type i) const {
    column_type row(m_columns); // recast
    for (size_type j = 0; j < m_columns; ++j)
        row[j] = operator [] (i);
    return row;
}

template <class T>
void Matrix<T>::insert_row(size_type i, column_type const & row) {
    // check that the row index is valid
    if (i >= m_rows)
        throw std::out_of_range ("row index out of range");
    // check that the row size is correct
    if (row.size() != m_columns)
        throw std::invalid_argument ("row size mismatch");
    for (size_type j = 0; j < m_columns; ++j)
        *this[j][i] = row[j];
}

template <class T>
typename Matrix<T>::pointer Matrix<T>::data (void) {
    return m_data.data();
}

template <class T>
typename Matrix<T>::const_pointer Matrix<T>::data (void) const {
    return m_data.data();
}

template <class T>
std::ostream& operator << (std::ostream & os, Matrix<T> const & m) {
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.columns(); ++j)
            os << m[i][j] << " ";
        os << std::endl;
    }
    return os;
}

template <class T>
Matrix<T> operator * (Matrix<T> const & A, Matrix<T> const & B) {

    // check the class type has a * operator defined
    if (!std::is_arithmetic<T>::value)
        throw std::invalid_argument ("no operator * defined for class type");

    using size_type = typename Matrix<T>::size_type;

    Matrix<T> C (A.rows (), B.columns ());

    for (size_type i = 0; i < A.rows (); ++i)
        for (size_type j = 0; j < B.columns (); ++j)
            for (size_type k = 0; k < A.columns (); ++k)
                C[i][j] += A[i][k] * B[k][j];

    return C;
}

#endif // MATRIX_HH
