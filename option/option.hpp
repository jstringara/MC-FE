#ifndef OPTION_HPP
#define OPTION_HPP

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

#include "vec.hpp"
#include "matrix.hpp"

using std::map;
using std::string;
using std::vector;

// class to describe the payoff of an option
class Option {

protected:
    // map to hold payoff parameters
    map<string, double> m_params;

public:
    Option(map<string, double> params) : m_params(params) {}
    // pure virtual function to compute the payoff of an option
    virtual Vec<double> payoff(Matrix<double> S, Vec<double> DF) const = 0; // vector
};

class EU_Call : public Option {

public:
    // constructor
    EU_Call(double K) : Option({{"K", K}}) {
        // check that K is positive
        if (K < 0.0)
            throw std::invalid_argument("K must be non-negative");
    }
    // compute the payoff of a European call option (only single value)
    Vec<double> payoff(Matrix<double> S, Vec<double> DF) const override;
};

class EU_Put : public Option {

public:
    // constructor
    EU_Put(double K) : Option({{"K", K}}) {
        // check that K is positive
        if (K < 0.0)
            throw std::invalid_argument("K must be non-negative");
    }
    // compute the payoff of a European put option (only single value)
    Vec<double> payoff(Matrix<double> S, Vec<double> DF) const override;
};

// cliquet option payoff
class ClOption : public Option {

public:
    // constructor
    ClOption(double L) : Option({{"L", L}}) {
        // check that L is positive
        if (L < 0.0)
            throw std::invalid_argument("L must be non-negative");
    }
    // compute the payoff of a cliquet option (only vector of values)
    Vec<double> payoff(Matrix<double> S, Vec<double> DF) const override;
};

#endif // !#ifndef OPTION_HPP
