#include "model.hpp"

using std::vector;

// initialize the random number generator
// (seed = 42, the answer to the ultimate question of life, the universe, and everything)
std::mt19937 Model::m_rng(42);

// getters
string Model::name() const {
    return m_name;
}

vector<string> Model::params() const {

    vector<string> keys;
    for (const auto& kv : m_params) {
        keys.push_back(kv.first);
    }
    return keys;
}

double Model::operator [] (string key) const {
    return m_params.at(key);
}

std::ostream& operator << (std::ostream& os, const Model& model) {
    os << model.name() << " ";
    for (const auto& param : model.params())
        os << param << ": " << model[param] << std::endl;
    return os;
}

BlackScholes::BlackScholes(double r, double sigma, double d)
    : Model("Black-Scholes", {{"r", r}, {"sigma", sigma}, {"d", d}}) { 
    // check if the parameters are valid
    if (sigma < 0.0)
        throw std::invalid_argument("sigma must be non-negative");
}

Vec<double> BlackScholes::simulate(const Vec<double>& S_0, double dt) const {

    // retrieve the parameters
    double r = m_params.at("r");
    double sigma = m_params.at("sigma");
    double d = m_params.at("d");

    // simulate the model
    Vec<double> S_t(S_0.size());
    for (size_t i = 0; i < S_0.size(); i++) {
        // generate the normal random variable (rescaled)
        double Z = randn() * sqrt(dt);
        S_t[i] = S_0[i] * exp((r - d - 0.5 * sigma * sigma) * dt + sigma * Z);
    }
    return S_t;
}
