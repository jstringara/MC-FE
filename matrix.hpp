#ifndef MATRIX_HH
#define MATRIX_HH

#include <istream>
#include <vector>
#include <sstream>
#include <iostream>

using std::vector;

template <class T> class Matrix {

    // matrix is a vector of vectors representing columns
    typedef vector<vector<T>> container_type;

public:
    typedef typename container_type::value_type column_type;
    typedef typename column_type::size_type size_type;
    typedef typename container_type::pointer pointer;
    typedef typename container_type::const_pointer const_pointer;
    typedef typename column_type::reference reference;
    typedef typename column_type::const_reference const_reference;

private:
    size_type m_rows = 0, m_columns = 0;
    container_type m_data;

public:
    // constructors
    Matrix<T> (void) = default;
    Matrix<T> (size_type rows, size_type cols, const_reference value = T());
    explicit Matrix<T>(size_type rows, size_type cols, container_type values);
    explicit Matrix<T> (std::istream &);

    void read (std::istream &);

    // elements access
    reference operator () (size_type, size_type);
    const_reference operator () (size_type, size_type) const;

    // size access
    size_type rows (void) const;
    size_type columns (void) const;

    // row and column access
    column_type col(size_type j) const;
    column_type row(size_type i) const;

    // row and column insertion
    void insert_col(size_type j, column_type const & col);
    void insert_row(size_type i, column_type const & row);

    Matrix<T> transposed (void) const;

    pointer data (void);
    const_pointer data (void) const;

    void print (std::ostream&) const;

    void to_csv (std::ostream&) const;

};

template <class T>
Matrix<T>::Matrix (size_type rows, size_type cols, const_reference value)
    : m_rows (rows), m_columns (cols), m_data (cols, vector<T>(rows,value)) {}
 
template <class T>
Matrix<T>::Matrix(size_type rows, size_type cols, container_type values)
    : m_rows (rows), m_columns (cols), m_data (values) {
    // check that the dimensions are correct
    if (m_rows * m_columns != m_data.size() * m_data[0].size())
        throw std::invalid_argument ("dimensions mismatch");
}

template <class T>
Matrix<T>::Matrix (std::istream & in) {
    read (in);
}

template <class T>
void Matrix<T>::read (std::istream & in) {
    std::string line;
    std::getline (in, line);

    std::istringstream first_line(line);
    first_line >> m_rows >> m_columns;
    // resize the data vector
    m_data.resize(m_columns, vector<T>(m_rows));

    for (size_type i = 0; i < m_rows; ++i) {
        std::getline (in, line);
        std::istringstream current_line(line);

        for (size_type j = 0; j < m_columns; ++j) {
            /* alternative syntax: current_line >> operator () (i, j);
             * or: current_line >> m_data[sub2ind (i, j)];
             */
            current_line >> (*this)(i, j);
        }
    }
}

// elements access
template <class T>
typename Matrix<T>::reference Matrix<T>::operator () (size_type i, size_type j) {
    return m_data[j][i];
}

template <class T>
typename Matrix<T>::const_reference Matrix<T>::operator () (size_type i, size_type j) const {
    return m_data[j][i];
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
typename Matrix<T>::column_type Matrix<T>::col(size_type j) const {
    // check that the column index is valid
    if (j >= m_columns)
        throw std::out_of_range ("column index out of range");
    return m_data[j];
}

template <class T>
void Matrix<T>::insert_col(size_type j, column_type const & col) {
    // check that the column index is valid
    if (j >= m_columns)
        throw std::out_of_range ("column index out of range");
    // check that the column size is correct
    if (col.size() != m_rows)
        throw std::invalid_argument ("column size mismatch");
    m_data[j] = col;
}

template <class T>
typename Matrix<T>::column_type Matrix<T>::row(size_type i) const {
    column_type row(m_columns); // recast
    for (size_type j = 0; j < m_columns; ++j)
        row[j] = operator () (i, j);
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
        operator () (i, j) = row[j];
}

template <class T>
Matrix<T> Matrix<T>::transposed (void) const {

    // create the transposed matrix
    Matrix At (m_columns, m_rows);

    for (size_type i = 0; i < m_rows; ++i)
        for (size_type j = 0; j < m_columns; ++j)
            At(j, i) = operator () (i, j);

    return At;
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
void Matrix<T>::print (std::ostream& os) const {
    using size_type = typename Matrix<T>::size_type;

    os << "(" << m_rows << "x" << m_columns << ")\n";

    for (size_type i = 0; i < m_rows; ++i) {
        for (size_type j = 0; j < m_columns; ++j)
            os << operator () (i,j) << " ";
        os << "\n";
    }
}

template <class T>
void Matrix<T>::to_csv (std::ostream& os) const {
    for (size_type i = 0; i < m_rows; ++i) {
        for (size_type j = 0; j < m_columns; ++j) {
            char del = (j < m_columns - 1) ? ',' : '\n';
            os << operator () (i,j) << del;
        }
    }
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
                C(i, j) += A(i, k) * B(k, j);

    return C;
}

#endif // MATRIX_HH
