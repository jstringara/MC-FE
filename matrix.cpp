#include <iomanip>
#include <sstream>
#include <iostream>
#include <numeric>

#include "matrix.hpp"

using std::vector;
using std::array;

matrix::matrix (size_type rows, size_type columns, const_reference value)
    : m_rows (rows), m_columns (columns), m_data (m_rows * m_columns, value) {}

matrix::matrix (std::istream & in) {
    read (in);
}

matrix::size_type matrix::sub2ind (size_type i, size_type j) const {
    return i * m_columns + j;
}

void matrix::read (std::istream & in) {
    std::string line;
    std::getline (in, line);

    std::istringstream first_line(line);
    first_line >> m_rows >> m_columns;
    m_data.resize (m_rows * m_columns);

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

void matrix::swap (matrix & rhs) {
    using std::swap;
    swap (m_rows, rhs.m_rows);
    swap (m_columns, rhs.m_columns);
    swap (m_data, rhs.m_data);
}

// copy constructor
// if the lhs matrix is a slice, copy to the original matrix
matrix& matrix::operator= (const matrix& rhs) {
    // check that dimensions agree
    if (m_rows != rhs.m_rows || m_columns != rhs.m_columns)
        throw std::invalid_argument ("dimensions mismatch");
    // lhs holds data
    if (m_pointers.empty()) {
        // copy the data
        for (size_type i = 0; i < m_rows; ++i)
            for (size_type j = 0; j < m_columns; ++j)
                operator () (i, j) = rhs(i, j);
    }
    // lhs is a slice, substitute the real data underneath
    if (!m_pointers.empty()){
        // copy the data
        for (size_type i = 0; i < m_rows; ++i)
            for (size_type j = 0; j < m_columns; ++j)
            *m_pointers[sub2ind (i, j)] = rhs(i, j);
    }

    return *this;
}

// elements access
matrix::reference matrix::operator () (size_type i, size_type j) {
    // if there are no pointers, return real data
    if (m_pointers.empty())
        return m_data[sub2ind (i, j)];
    return *m_pointers[sub2ind (i, j)];
}

matrix::const_reference matrix::operator () (size_type i, size_type j) const {
    if (m_pointers.empty())
        return m_data[sub2ind (i, j)];
    return *m_pointers[sub2ind (i, j)];
}

// slicing with array indexes
matrix matrix::operator () (array<size_type, 2> rows_arr, array<size_type, 2> cols_arr) {
    // check that the arrays make sense
    if (rows_arr[0] > rows_arr[1] || cols_arr[0] > cols_arr[1])
        throw std::invalid_argument ("invalid slice");
    // generate the two full vectors
    vector<size_type> rows(rows_arr[1]-rows_arr[0]+1);
    std::iota (rows.begin(), rows.end(), rows_arr[0]);
    vector<size_type> cols(cols_arr[1]-cols_arr[0]+1);
    std::iota(cols.begin(),cols.end(), cols_arr[0]);

    // return the vector version
    return operator () (rows, cols);
}
// slicing
matrix matrix::operator () (vector<size_type> rows, vector<size_type> columns) {

    // if both empty, throw exception
    if (rows.empty() && columns.empty())
        throw std::invalid_argument ("empty slice");
    // if only rows is empty, return all columns
    if (rows.empty()){
        // create a vector with m_rows elements
        rows = vector<size_type> (m_rows);
        std::iota (rows.begin(), rows.end(), 0);
    }
    // if only columns is empty, return all rows
    if (columns.empty()){
        // create a vector with m_columns elements
        columns = vector<size_type> (m_columns);
        std::iota (columns.begin(), columns.end(), 0);
    }

    // the new matrix data is a reference to old matrix data

    // create a new matrix with the new dimensions
    matrix M (rows.size(), columns.size());

    // make the new matrix point to the old data (shallow copy)
    for (size_type i = 0; i < M.rows (); ++i)
        for (size_type j = 0; j < M.columns (); ++j)
            M.m_pointers.push_back(&operator () (rows[i], columns[j]));

    return M;
}

const matrix matrix::operator () (vector<size_type> rows, vector<size_type> columns) const {
    return const_cast<matrix*>(this)->operator () (rows, columns);
}

matrix::size_type matrix::rows (void) const {
    return m_rows;
}

matrix::size_type matrix::columns (void) const {
    return m_columns;
}

matrix matrix::transposed (void) const {
    matrix At (m_columns, m_rows);

    for (size_type i = 0; i < m_columns; ++i)
        for (size_type j = 0; j < m_rows; ++j)
            At(i, j) = operator () (j, i);

    return At;
}

matrix::pointer matrix::data (void) {
    return m_data.data ();
}

matrix::const_pointer matrix::data (void) const {
    return m_data.data ();
}

void matrix::print (std::ostream& os) const {
    using size_type = matrix::size_type;

    os << "(" << m_rows << "x" << m_columns << ")\n";

    for (size_type i = 0; i < m_rows; ++i) {
        for (size_type j = 0; j < m_columns; ++j)
            os << operator () (i,j) << " ";
        os << "\n";
    }
}

void matrix::to_csv (std::ostream& os) const {
    for (size_type i = 0; i < m_rows; ++i) {
        for (size_type j = 0; j < m_columns; ++j) {
            char del = (j < m_columns - 1) ? ',' : '\n';
            os << std::setprecision(5) << operator () (i,j) << del;
        }
    }
}

matrix operator * (matrix const & A, matrix const & B) {
    using size_type = matrix::size_type;

    matrix C (A.rows (), B.columns ());

    for (size_type i = 0; i < A.rows (); ++i)
        for (size_type j = 0; j < B.columns (); ++j)
            for (size_type k = 0; k < A.columns (); ++k)
                C(i, j) += A(i, k) * B(k, j);

    return C;
}

void swap (matrix & A, matrix & B) {
    A.swap (B);
}
