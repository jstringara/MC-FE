#ifndef DENSE_MATRIX_HH
#define DENSE_MATRIX_HH

#include <istream>
#include <vector>

class matrix final {

    typedef std::vector<double> container_type;

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

    size_type sub2ind (size_type i, size_type j) const;

public:
    // constructors
    matrix (void) = default;
    matrix (size_type rows, size_type columns, const_reference value = 0.0);
    explicit matrix (std::istream &);

    void read (std::istream &);
    void swap (matrix &);

    reference operator () (size_type i, size_type j);
    const_reference operator () (size_type i, size_type j) const;

    size_type rows (void) const;
    size_type columns (void) const;

    matrix transposed (void) const;

    pointer data (void);
    const_pointer data (void) const;

    void print (std::ostream& os) const;

    void to_csv (std::ostream& os) const;

};

matrix operator * (matrix const &, matrix const &);

void swap (matrix &, matrix &);

#endif // DENSE_MATRIX_HH
