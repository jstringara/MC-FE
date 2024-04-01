#include "payoff.hpp"

double EU_Call::operator()(double S) const {
    double K = m_params.at("K");
    return std::max(S - K, 0.0);
}
