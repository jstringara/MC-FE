#include <sstream>
#include <iostream>

#include "matrix.hpp"

using std::vector;

matrix::matrix (size_type rows, size_type cols, const_reference value)
    : m_rows (rows), m_columns (cols), m_data (cols, vector<double>(rows,value)) {}
 
matrix::matrix(size_type rows, size_type cols, container_type values)
    : m_rows (rows), m_columns (cols), m_data (values) {
    // check that the dimensions are correct
    if (m_rows * m_columns != m_data.size() * m_data[0].size())
        throw std::invalid_argument ("dimensions mismatch");
}

matrix::matrix (std::istream & in) {
    read (in);
}

void matrix::read (std::istream & in) {
    std::string line;
    std::getline (in, line);

    std::istringstream first_line(line);
    first_line >> m_rows >> m_columns;
    // resize the data vector
    m_data.resize(m_columns, vector<double>(m_rows));

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
matrix::reference matrix::operator () (size_type i, size_type j) {
    return m_data[j][i];
}

matrix::const_reference matrix::operator () (size_type i, size_type j) const {
    return m_data[j][i];
}

matrix::size_type matrix::rows (void) const {
    return m_rows;
}

matrix::size_type matrix::columns (void) const {
    return m_columns;
}

matrix::column_type matrix::col(size_type j) const {
    // check that the column index is valid
    if (j >= m_columns)
        throw std::out_of_range ("column index out of range");
    return m_data[j];
}

void matrix::insert_col(size_type j, column_type const & col) {
    // check that the column index is valid
    if (j >= m_columns)
        throw std::out_of_range ("column index out of range");
    // check that the column size is correct
    if (col.size() != m_rows)
        throw std::invalid_argument ("column size mismatch");
    m_data[j] = col;
}

matrix::column_type matrix::row(size_type i) const {
    column_type row(m_columns);
    for (size_type j = 0; j < m_columns; ++j)
        row[j] = operator () (i, j);
    return row;
}

void matrix::insert_row(size_type i, column_type const & row) {
    // check that the row index is valid
    if (i >= m_rows)
        throw std::out_of_range ("row index out of range");
    // check that the row size is correct
    if (row.size() != m_columns)
        throw std::invalid_argument ("row size mismatch");
    for (size_type j = 0; j < m_columns; ++j)
        operator () (i, j) = row[j];
}

matrix matrix::transposed (void) const {

    // create the transposed matrix
    matrix At (m_columns, m_rows);

    for (size_type i = 0; i < m_rows; ++i)
        for (size_type j = 0; j < m_columns; ++j)
            At(j, i) = operator () (i, j);

    return At;
}

matrix::pointer matrix::data (void) {
    return m_data.data();
}

matrix::const_pointer matrix::data (void) const {
    return m_data.data();
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
            os << operator () (i,j) << del;
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

