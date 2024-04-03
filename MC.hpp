#ifndef MC_HPP
#define MC_HPP

#include "model.hpp"
#include "option.hpp"
#include "matrix.hpp"

// class representing the Monte Carlo simulation
class MC {

private:
    // model to use for the simulation
    Model* m_model;
    // option to price
    Option* m_option;
    // result of the simulation
    matrix m_result;

    // compute the IC and mean (helper function)
    map<string, double> compute_IC_and_mean(vector<double> DF) const;

public:

    // constructor
    MC(Model* model, Option* option) : m_model(model), m_option(option) {};

    // return the result of the simulation
    matrix simulate(size_t N_sim, size_t N_steps, double S_0, double T);

    // compute the price and IC at 95%
    map<string, double> price(vector<double> DF, double S_0, double T, size_t N_sim, 
        size_t N_steps);

};

#endif // !#ifndef MC_HPP
