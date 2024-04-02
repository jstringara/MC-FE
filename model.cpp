#include "model.hpp"

using std::vector;

// initialize the random number generator
// (seed = 42, the answer to the ultimate question of life, the universe, and everything)
std::mt19937 Model::m_rng(42);

void Model::print(std::ostream& os) const {
    os << "Model: " << m_name << std::endl;
    for (auto const& x : m_params) {
        os << x.first << ": " << x.second << std::endl;
    }
}

vector<double> BlackScholes::simulate(const vector<double>& S_0, double dt) const {

    // retrieve the parameters
    double r = m_params.at("r");
    double sigma = m_params.at("sigma");
    double d = m_params.at("d");

    // generate the normal random variable (rescaled)
    double Z = randn() * sqrt(dt);
    // simulate the model
    vector<double> S_t(S_0.size());
    for (size_t i = 0; i < S_0.size(); i++) {
        S_t[i] = S_0[i] * exp((r - d - 0.5 * sigma * sigma) * dt + sigma * Z);
    }
    return S_t;
}
