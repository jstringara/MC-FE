#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <map>
#include <random>
#include <string>

using std::vector;
using std::string;
using std::map;
using std::ostream;
using std::normal_distribution;

#include "vec.hpp"
   
// model class used to model the underlying stock dynamics
class Model {

protected:
    // random number generator (static member variable)
    static std::mt19937 m_rng;
    // name of the model
    string m_name;
    // parameters of the model {name, value}
    map<string, double> m_params;

public:
    // constructor
    Model(string name, map<string, double> params) : m_name(name), m_params(params) {};
    // pure virtual function to simulate
    virtual Vec<double> simulate(const Vec<double>& S, double dt) const = 0;

    // getters
    string name() const;

    vector<string> params() const;

    double operator [] (string key) const;

};

// Black-Scholes model
class BlackScholes : public Model {
    
public:
    // constructor
    BlackScholes(double r, double sigma, double d=0.0);

    // simulate the model with Black Scholes dynamics (vectorized)
    Vec<double> simulate(const Vec<double>& S, double dt) const override;

private:
    // generate a random number from a normal distribution
    double randn() const {
        normal_distribution<double> dist(0.0, 1.0);
        return dist(m_rng);
    }

};

#endif // !#ifndef MODEL_HPP
