#include "MC.hpp"
#include "matrix.hpp"

// simulation
map<string, double> MC::simulate(size_t N_sim, size_t N_steps, double S_0, double T,
    vector<double> DF) const{

    // check that we have enough discount factors
    if (DF.size() < N_steps){
        throw std::invalid_argument("Not enough discount factors");
    }

    // compute the time step
    double dt = T/N_steps;
    // get the model for the simulation
    const Model &model = *m_model;

    // create the matrix to hold the paths (N_sim x N_steps+1)
    matrix S(N_sim, N_steps+1);

    // set the first column to S_0
    S({}, vector<size_t>{0}) = matrix(N_sim, 1, vector<double>(N_sim, S_0));

    // simulate the paths for each column
    for (size_t i = 0; i<N_steps; ++i){
        // get the i-th column
        vector<double> S_i = S({}, vector<size_t>{i}).as_vector();
        vector<double> S_iplus1 = model.simulate(S_i, dt);
        // save in the matrix
        S({}, vector<size_t>{i+1}) = matrix(N_sim, 1, S_iplus1);
    }

    // get the payoff for each path
    vector<double> payoffs;

    // compute the payoff for each path
    for (size_t i = 0; i<N_sim; ++i){
        // get the row i
        vector<double> S_i = S(vector<size_t>{i}, {}).as_vector();
        // compute the payoff
        double payoff = m_option->payoff(S_i, DF);
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
    };
}
