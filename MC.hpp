#ifndef MC_HPP
#define MC_HPP

#include "model.hpp"
#include "option.hpp"

// class representing the Monte Carlo simulation
class MC {

private:
    // model to use for the simulation
    Model* m_model;
    // option to price
    Option* m_option;

public:

    // constructor
    MC(Model* model, Option* option) : m_model(model), m_option(option) {};

    // simulation, returns a map with {mean, lower and upper IC at 95%}
    map<string, double> simulate(size_t N_sim, size_t N_steps, double S_0, double T,
        vector<double> DF) const;

};

#endif // !#ifndef MC_HPP
