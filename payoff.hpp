#ifndef PAYOFF_HPP
#define PAYOFF_HPP

#include <map>
#include <string>
#include <vector>
#include <stdexcept>

using std::map;
using std::string;
using std::vector;

// class to describe the payoff of an option
class Payoff {

protected:
    // map to hold payoff parameters
    map<string, double> m_params;

public:
    Payoff(map<string, double> params) : m_params(params) {}
    // pure virtual function to compute the payoff of an option
    virtual double operator()(double S) const = 0; // single value
    virtual double operator()(vector<double> S) const = 0; // vector
};

class EU_Call : public Payoff {

public:
    // constructor
    EU_Call(double K) : Payoff({{"K", K}}) {
        // check that K is positive
        if (K < 0.0)
            throw std::invalid_argument("K must be non-negative");
    }
    // compute the payoff of a European call option (only single value)
    double operator()(double S) const override;
    double operator()(vector<double> S) const override { return 0.0; }
};

class EU_Put : public Payoff {

public:
    // constructor
    EU_Put(double K) : Payoff({{"K", K}}) {
        // check that K is positive
        if (K < 0.0)
            throw std::invalid_argument("K must be non-negative");
    }
    // compute the payoff of a European put option (only single value)
    double operator()(double S) const override;
    double operator()(vector<double> S) const override { return 0.0; }
};

// cliquet option payoff
class ClOption : public Payoff {

public:
    // constructor
    ClOption(double L) : Payoff({{"L", L}}) {
        // check that L is positive
        if (L < 0.0)
            throw std::invalid_argument("L must be non-negative");
    }
    // compute the payoff of a cliquet option (only vector of values)
    double operator()(double S) const override { return 0.0; }
    double operator()(vector<double> S) const override;
};

#endif // !#ifndef PAYOFF_HPP
