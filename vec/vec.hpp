#ifndef VEC_HH
#define VEC_HH

#include <algorithm>
#include <iostream>
#include <type_traits>
#include <vector>
#include <numeric>

using std::vector;

template <class T> class Vec {

    // vec is just a wrapper around a vector with a few operators
    typedef vector<T> container_type;

public:
    typedef typename container_type::size_type size_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::pointer pointer;
    typedef typename container_type::const_pointer const_pointer;
    typedef typename container_type::iterator iterator;

private:
    size_type m_size = 0;
    container_type m_data;

public:
    // constructors
    Vec<T> (void) = default;
    Vec<T> (size_type size, const_reference value = T());
    Vec<T> (container_type values);
    explicit Vec<T> (size_type size, container_type values);
    explicit Vec<T> (std::istream &);

    void read (std::istream &);

    // elements access
    reference operator [] (size_type);
    const_reference operator [] (size_type) const;

    // size access
    size_type size (void) const;

    // addition
    Vec<T> operator + (const Vec<T> &) const;
    Vec<T> operator + (const T &) const;
    Vec<T> operator += (const Vec<T> &);
    Vec<T> operator += (const T &);

    // subtraction
    Vec<T> operator - (const Vec<T> &) const;
    Vec<T> operator - (const T &) const;
    Vec<T> operator -= (const Vec<T> &);
    Vec<T> operator -= (const T &);

    friend Vec<T> operator - (const T & s, const Vec<T> & v) {
        // check that the operator is defined for the type T
        if (std::is_arithmetic<T>::value == false)
            throw std::invalid_argument("operator -: operator - not defined for type T");
        // subtract the vector from the scalar
        Vec<T> result(v.size());
        std::transform (v.m_data.cbegin(), v.m_data.cend(), result.m_data.begin(),
            [&s](const T & x) { return s - x; });
        return result;
    };

    // multiplication
    Vec<T> operator * (const T &) const;
    T operator * (const Vec<T> &) const;
    T operator *= (const Vec<T> &);
    Vec<T> operator *= (const T &);

    // scalar division
    Vec<T> operator / (const T &) const;
    Vec<T> operator /= (const T &);

    // element wise max
    Vec<T> operator ^ (const Vec<T> &) const;
    Vec<T> operator ^ (const T & s) const;

    // element wise min
    Vec<T> operator | (const Vec<T> &) const;
    Vec<T> operator | (const T & s) const;

    // mean
    T mean (void) const; 
    // variance
    T var (void) const;

};

// constructors
template <class T>
Vec<T>::Vec (size_type size, const_reference value)
    : m_size (size), m_data (size, value) {}

template <class T>
Vec<T>::Vec (size_type size, container_type values)
    : m_size (size), m_data (values) {
    // check that the dimensions are correct
    if (m_data.size() != m_size)
        throw std::invalid_argument("Vec::Vec: wrong size");
}

template <class T>
Vec<T>::Vec (container_type values)
    : m_size (values.size()), m_data (values) {}

template <class T>
Vec<T>::Vec (std::istream & is) {
    read(is);
}

template <class T>
void Vec<T>::read (std::istream & is) {
    is >> m_size;
    m_data.resize(m_size);
    for (size_type i = 0; i < m_size; ++i)
        is >> m_data[i];
}

// access elements
template <class T>
typename Vec<T>::reference Vec<T>::operator [] (size_type i) {
    return m_data[i];
}

template <class T>
typename Vec<T>::const_reference Vec<T>::operator [] (size_type i) const {
    return m_data[i];
}

// size access
template <class T>
typename Vec<T>::size_type Vec<T>::size (void) const {
    return m_size;
}

// operators

// addition

template <class T>
Vec<T> Vec<T>::operator + (const T & s) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator +: operator + not defined for type T");
    // add the scalar to the vector
    Vec<T> result(m_size);
    std::transform (m_data.cbegin(), m_data.cend(), result.m_data.begin(),
        [&s](const T & x) { return x + s; });
    return result;
}

template <class T>
Vec<T> Vec<T>::operator + (const Vec<T> & v) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator +: operator + not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator +: wrong size");
    // add the two vectors together element-wise
    Vec<T> result(m_size);
    std::transform (m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), result.m_data.begin(),
        std::plus<T>());
    return result;
}

// reverse addition
template <class T>
inline Vec<T> operator + (const T & s, const Vec<T> & v) {
    return v + s;
}

template <class T>
Vec<T> Vec<T>::operator += (const Vec<T> & v) {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator +=: operator + not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator +=: wrong size");
    // add the two vectors together element-wise
    std::transform (m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), m_data.begin(),
        std::plus<T>());
    return *this;
}

template <class T>
Vec<T> Vec<T>::operator += (const T & s) {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator +=: operator + not defined for type T");
    // add the scalar to the vector
    std::transform (m_data.cbegin(), m_data.cend(), m_data.begin(),
        [&s](const T & x) { return x + s; });
    return *this;
}

// subtraction
template <class T>
Vec<T> Vec<T>::operator - (const T & s) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator -: operator - not defined for type T");
    // subtract the scalar from the vector
    Vec<T> result(m_size);
    std::transform (m_data.cbegin(), m_data.cend(), result.m_data.begin(),
        [&s](const T & x) { return x - s; });
    return result;
}

template <class T>
Vec<T> Vec<T>::operator - (const Vec<T> & v) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator -: operator - not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator -: wrong size");
    // subtract the two vectors element-wise
    Vec<T> result(m_size, m_data);
    std::transform (m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), result.m_data.begin(),
        std::minus<T>());
    return result;
}


template <class T>
Vec<T> Vec<T>::operator -= (const Vec<T> & v) {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator -=: operator - not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator -=: wrong size");
    // subtract the two vectors element-wise
    std::transform (m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), m_data.begin(),
        std::minus<T>());
    return *this;
}

template <class T>
Vec<T> Vec<T>::operator -= (const T & s) {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator -=: operator - not defined for type T");
    // subtract the scalar from the vector
    std::transform (m_data.cbegin(), m_data.cend(), m_data.begin(),
        [&s](const T & x) { return x - s; });
    return *this;
}

// multiplication

template <class T>
Vec<T> Vec<T>::operator * (const T & s) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator *: operator * not defined for type T");
    // multiply the vector by the scalar
    Vec<T> result(m_size);
    std::transform (m_data.cbegin(), m_data.cend(), result.m_data.begin(),
        [&s](const T & x) { return x * s; });
    return result;
}

template <class T>
T Vec<T>::operator * (const Vec<T> & v) const {
    // check that the product and sum are defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator *: operator * or operator + not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator *: wrong sizes");
    // compute the dot product
    Vec<T> product(m_size);
    std::transform(m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), product.m_data.begin(),
        std::multiplies<T>());
    T res = std::accumulate(product.m_data.cbegin(), product.m_data.cend(), T(), std::plus<T>());

    return res;
}

template <class T>
inline Vec<T> operator * (const T & s, const Vec<T> & v) {
    return v * s;
}

template <class T>
Vec<T> Vec<T>::operator *= (const T & s) {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator *=: operator * not defined for type T");
    // multiply the vector by the scalar
    std::transform (m_data.cbegin(), m_data.cend(), m_data.begin(),
        [&s](const T & x) { return x * s; });
    return *this;
}

template <class T>
T Vec<T>::operator *= (const Vec<T> & v) {
    // check that the product and sum are defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator *=: operator * or operator + not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator *=: wrong sizes");
    // compute the dot product
    Vec<T> product(m_size);
    std::transform(m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), product.m_data.begin(),
        std::multiplies<T>());
    T res = std::accumulate(product.m_data.cbegin(), product.m_data.cend(), T(), std::plus<T>());
    m_data[0] = res;
    return *this;
}

// scalar division
template <class T>
Vec<T> Vec<T>::operator / (const T & s) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator /: operator / not defined for type T");
    // divide the vector by the scalar (we rely on the type to catc division by zero)
    Vec<T> result(m_size);
    std::transform (m_data.cbegin(), m_data.cend(), result.m_data.begin(),
        [&s](const T & x) { return x / s; });
    return result;
}

template <class T>
Vec<T> Vec<T>::operator /= (const T & s) {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator /=: operator / not defined for type T");
    // divide the vector by the scalar (we rely on the type to catc division by zero)
    std::transform (m_data.cbegin(), m_data.cend(), m_data.begin(),
        [&s](const T & x) { return x / s; });
    return *this;
}

// element wise max
template<class T>
Vec<T> Vec<T>::operator ^ (const Vec<T> & v) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator ^: operator ^ not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator ^: wrong size");
    // compute the element wise max
    Vec<T> result(m_size);
    std::transform(m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), result.m_data.begin(),
        [](const T & x, const T & y) { return std::max(x, y); });
    return result;
}

template<class T>
Vec<T> Vec<T>::operator ^ (const T & s) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator ^: operator ^ not defined for type T");
    // compute the element wise max
    Vec<T> result(m_size);
    std::transform(m_data.cbegin(), m_data.cend(), result.m_data.begin(),
        [&s](const T & x) { return std::max(x, s); });
    return result;
}

template<class T>
inline Vec<T> operator ^ (const T & s, const Vec<T> & v) {
    return v ^ s;
}

// element wise min
template <class T>
Vec<T> Vec<T>::operator | (const Vec<T> & v) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator |: operator | not defined for type T");
    // check that the size match up
    if (m_size != v.m_size)
        throw std::invalid_argument("Vec::operator |: wrong size");
    // compute the element wise min
    Vec<T> result(m_size);
    std::transform(m_data.cbegin(), m_data.cend(), v.m_data.cbegin(), result.m_data.begin(),
        [](const T & x, const T & y) { return std::min(x, y); });
    return result;
}

template <class T>
Vec<T> Vec<T>::operator | (const T & s) const {
    // check that the operator is defined for the type T
    if (std::is_arithmetic<T>::value == false)
        throw std::invalid_argument("Vec::operator |: operator | not defined for type T");
    // compute the element wise min
    Vec<T> result(m_size);
    std::transform(m_data.cbegin(), m_data.cend(), result.m_data.begin(),
        [&s](const T & x) { return std::min(x, s); });
    return result;
}

template <class T>
inline Vec<T> operator | (const T & s, const Vec<T> & v) {
    return v | s;
}

// mean
template <class T>
T Vec<T>::mean (void) const {
    return std::accumulate(m_data.cbegin(), m_data.cend(), T()) / m_size;
}

// variance
template <class T>
T Vec<T>::var (void) const {
    T m = mean();
    return std::accumulate(m_data.cbegin(), m_data.cend(), T(), 
        [m](T accumulator, T value) {
            return accumulator + (value - m) * (value - m);
        }) / (m_size - 1);
}

// stream output 
template <class T>
std::ostream & operator << (std::ostream & os, const Vec<T> & v) {
    // all vectors are printed as column vectors
    for (typename Vec<T>::size_type i = 0; i < v.size(); ++i)
        os << v[i] << std::endl;
    return os;
}

#endif // !#ifndef VEC_HH
