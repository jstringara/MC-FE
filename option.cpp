#include "option.hpp"

double EU_Call::payoff(vector<double> S, vector<double> DF) const {
    double K = m_params.at("K");
    // get only the last value
    double S_T = S.back();
    double DF_T = DF.back();
    return DF_T * std::max(S_T - K, 0.0);
}

double EU_Put::payoff(vector<double> S, vector<double> DF) const {
    double K = m_params.at("K");
    double S_T = S.back();
    double DF_T = DF.back();
    return DF_T * std::max(K - S_T, 0.0);
}

double ClOption::payoff(vector<double> S, vector<double> DF) const {

    double L = m_params.at("L");
    double payoff = 0.0;

    for (size_t i = 1; i < S.size(); i++) {
        payoff += DF[i] * std::max(L*(S[i] - S[i-1]), 0.0);
    }

    return payoff;
}
