#include "option.hpp"

Vec<double> EU_Call::payoff(Matrix<double> S, Vec<double> DF) const {
    double K = m_params.at("K");
    // get only the last column of S
    Vec<double> S_T = S[S.columns()-1];
    double DF_T = DF[DF.size()-1];
    // apply the payoff
    S_T = DF_T * (S_T - K);
    // find the maximum between S_T and 0
    return S_T ^ 0.0;
}

Vec<double> EU_Put::payoff(Matrix<double> S, Vec<double> DF) const {
    double K = m_params.at("K");
    Vec<double> S_T = S[S.columns()-1];
    double DF_T = DF[DF.size()-1];
    S_T = DF_T * (K - S_T);
    return S_T ^ 0.0;
}

Vec<double> ClOption::payoff(Matrix<double> S, Vec<double> DF) const {

    double L = m_params.at("L");
    Vec<double> payoff(S.columns(), 0.0);

    for (size_t i = 1; i < S.columns(); i++) {
        payoff += DF[i] * (L*(S[i] - S[i-1]) ^ 0.0);
    }

    return payoff;
}
