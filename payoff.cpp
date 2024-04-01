#include "payoff.hpp"

double EU_Call::operator()(double S) const {
    double K = m_params.at("K");
    return std::max(S - K, 0.0);
}

double EU_Put::operator()(double S) const {
    double K = m_params.at("K");
    return std::max(K - S, 0.0);
}

double ClOption::operator()(vector<double> S) const {

    double L = m_params.at("L");
    double payoff = 0.0;

    for (size_t i = 1; i < S.size(); i++) {
        payoff += std::max(L*(S[i] - S[i-1]), 0.0);
    }

    return payoff;
}
