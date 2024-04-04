#include "MC.hpp"

// simulation
Matrix<double> MC::simulate(size_t N_sim, size_t N_steps, double S_0, double T) {

    // compute the time step
    double dt = T/N_steps;
    // get the model for the simulation
    const Model &model = *m_model;

    // create the matrix to hold the paths (N_sim x N_steps+1)
    Matrix<double> S(N_sim, N_steps+1);

    // set the first column to S_0
    S.insert_col(0, vector<double>(N_sim, S_0));

    // simulate the paths for each column
    for (size_t i = 0; i<N_steps; ++i){
        // get the i-th column
        vector<double> S_i = S.col(i);
        vector<double> S_iplus1 = model.simulate(S_i, dt);
        // save in the matrix
        S.insert_col(i+1, S_iplus1);
    }

    return S;
}

map<string, double> MC::compute_IC_and_mean(vector<double> DF) const {

    // check that we have enough discount factors
    if (DF.size() < m_result.columns()-1){
        throw std::invalid_argument("Not enough discount factors");
    }

    // save the number of simulations and steps
    Matrix<double>::size_type N_sim = m_result.rows();
    Matrix<double>::size_type N_steps = m_result.columns() - 1;

    // compute the payoff for each path
    vector<double> payoffs;
    for (Matrix<double>::size_type i = 0; i<N_sim; ++i){
        // get the row i
        vector<double> path = m_result.row(i);
        // compute the payoff
        double payoff = m_option->payoff(path, DF);
        // add to the mean
        payoffs.push_back(payoff);
    }

    // compute the mean of the vector
    double mean = std::accumulate(payoffs.begin(), payoffs.end(), 0.0) / N_sim;

    // compute the variance for the IC
    double sum_squared_diff = std::accumulate(payoffs.begin(), payoffs.end(), 0.0,
        [mean](double accumulator, double value) {
            return accumulator + (value - mean) * (value - mean);
        });
    double var = sum_squared_diff / (N_sim - 1);

    // lower and upper bounds of the IC at 95%
    double lb = mean - 1.96 * sqrt(var / N_sim);
    double ub = mean + 1.96 * sqrt(var / N_sim);

    return {
        {"mean", mean},
        {"lb", lb},
        {"ub", ub},
        {"var", var}
    };
}

map<string, double> MC::price(vector<double> DF, double S_0, double T, size_t N_sim,
    size_t N_steps) {
    // simulate the paths if necessary
    if (m_result.rows() != N_sim || m_result.columns() != N_steps + 1){
        m_result = simulate(N_sim, N_steps, S_0, T);
    }

    // compute the IC and mean
    return compute_IC_and_mean(DF);
}

