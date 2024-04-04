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

BlackScholes::BlackScholes(double r, double sigma, double d)
    : Model("Black-Scholes", {{"r", r}, {"sigma", sigma}, {"d", d}}) { 
    // check if the parameters are valid
    if (sigma < 0.0)
        throw std::invalid_argument("sigma must be non-negative");
}

BlackScholes::BlackScholes(std::istream& is) :
    Model("Black-Scholes", {}){

    // read the input
    double r, sigma, d;
    is >> r >> sigma >> d;

    // check the parameters
    if (sigma < 0.0)
        throw std::invalid_argument("sigma must be non-negative");
    // add the parameters
    m_params["r"] = r;
    m_params["sigma"] = sigma;
    m_params["d"] = d;

}

vector<double> BlackScholes::simulate(const vector<double>& S_0, double dt) const {

    // retrieve the parameters
    double r = m_params.at("r");
    double sigma = m_params.at("sigma");
    double d = m_params.at("d");

    // simulate the model
    vector<double> S_t(S_0.size());
    for (size_t i = 0; i < S_0.size(); i++) {
        // generate the normal random variable (rescaled)
        double Z = randn() * sqrt(dt);
        S_t[i] = S_0[i] * exp((r - d - 0.5 * sigma * sigma) * dt + sigma * Z);
    }
    return S_t;
}
