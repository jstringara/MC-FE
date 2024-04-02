#ifndef DENSE_MATRIX_HH
#define DENSE_MATRIX_HH

#include <istream>
#include <vector>
#include <array>

using std::vector;
using std::array;

class matrix final {

    typedef vector<double> container_type;
    // for when the matrix is only a slice of a bigger matrix
    typedef vector<double*> pointer_container_type;

public:
    typedef container_type::value_type value_type;
    typedef container_type::size_type size_type;
    typedef container_type::pointer pointer;
    typedef container_type::const_pointer const_pointer;
    typedef container_type::reference reference;
    typedef container_type::const_reference const_reference;

private:
    size_type m_rows = 0, m_columns = 0;
    container_type m_data;
    pointer_container_type m_pointers = {}; // starts empty

    size_type sub2ind (size_type i, size_type j) const;

public:
    // constructors
    matrix (void) = default;
    matrix (size_type rows, size_type columns, const_reference value = 0.0);
    explicit matrix (std::istream &);

    // copy constructor
    matrix& operator= (const matrix&);

    void read (std::istream &);
    void swap (matrix &);

    // elements access
    reference operator () (size_type, size_type);
    const_reference operator () (size_type, size_type) const;

    // slicing (only two indexes)
    matrix operator () (array<size_type, 2>, array<size_type, 2>);
    const matrix operator () (array<size_type, 2>, array<size_type, 2>) const;

    // slicing (multiple indexes)
    matrix operator () (vector<size_type>, vector<size_type>);
    const matrix operator () (vector<size_type>, vector<size_type>) const;

    size_type rows (void) const;
    size_type columns (void) const;

    matrix transposed (void) const;

    pointer data (void);
    const_pointer data (void) const;

    void print (std::ostream&) const;

    void to_csv (std::ostream&) const;

};

matrix operator * (matrix const &, matrix const &);

void swap (matrix &, matrix &);

#endif // DENSE_MATRIX_HH
