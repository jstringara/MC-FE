#include "model.hpp"

// initialize the random number generator
// (seed = 42, the answer to the ultimate question of life, the universe, and everything)
std::mt19937 MC::Model::m_rng(42);

void MC::Model::print(std::ostream& os) const {
    os << "Model: " << m_name << std::endl;
    for (auto const& x : m_params) {
        os << x.first << ": " << x.second << std::endl;
    }
}
// simulate the model with Black Scholes dynamics
double MC::BlackScholes::simulate(double S_0, double dt) const {
    // retrieve the parameters
    double r = m_params.at("r");
    double sigma = m_params.at("sigma");
    // generate the normal random variable (rescaled)
    double Z = randn() * sqrt(dt);
    // simulate the model
    double S_t = S_0 * exp((r - 0.5 * sigma * sigma) * dt + sigma * Z );
    return S_t;
}
