#ifndef DENSE_MATRIX_HH
#define DENSE_MATRIX_HH

#include <istream>
#include <vector>
#include <array>

using std::vector;
using std::array;

class matrix {

    // matrix is a vector of vectors representing columns
    typedef vector<vector<double>> container_type;

public:
    typedef container_type::value_type column_type;
    typedef column_type::value_type value_type;
    typedef column_type::size_type size_type;
    typedef container_type::pointer pointer;
    typedef container_type::const_pointer const_pointer;
    typedef column_type::reference reference;
    typedef column_type::const_reference const_reference;

private:
    size_type m_rows = 0, m_columns = 0;
    container_type m_data;

public:
    // constructors
    matrix (void) = default;
    matrix (size_type rows, size_type cols, const_reference value = 0.0);
    explicit matrix(size_type rows, size_type cols, container_type values);
    explicit matrix (std::istream &);

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

    matrix transposed (void) const;

    pointer data (void);
    const_pointer data (void) const;

    void print (std::ostream&) const;

    void to_csv (std::ostream&) const;

};

matrix operator * (matrix const &, matrix const &);

#endif // DENSE_MATRIX_HH
