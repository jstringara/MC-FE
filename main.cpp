#include "model.hpp"
#include "option.hpp"
#include "matrix.hpp"
#include "MC.hpp"

using BlackScholes = BlackScholes;

// define the normal distribution function
double N(double x) {
    return 0.5 * erfc(-x / sqrt(2.0));
}


int main(int argc, char* argv[]){

    // take the number of simulations from the input
    size_t N_sim = argv[1] ? std::stoi(argv[1]) : 1000;

    size_t N_steps = 1;
    double S_0 = 100.0;

    // run a MC simulation for BlackScholes call option
    BlackScholes model = BlackScholes(0.05, 0.2);
    EU_Call option = EU_Call(100.0);

    // compute the discount factors
    vector<double> DF({exp(-0.05 * 1.0)});

    // create the MC object
    MC mc = MC(&model, &option);

    // run the simulation
    map<string, double> results = mc.simulate(N_sim, N_steps, S_0, 1.0, DF);

    std::cout << "Mean: " << results["mean"] << std::endl;
    std::cout << "[" << results["lb"] << ", " << results["ub"] << "]" << std::endl;

    // compare with the Black-Scholes formula
    double d1 = (log(S_0 / 100.0) + (0.05 + 0.2 * 0.2 / 2) * 1.0) / (0.2 * sqrt(1.0));
    double d2 = d1 - 0.2 * sqrt(1.0);
    double BS_call = S_0 * N(d1) - 100.0 * exp(-0.05 * 1.0) * N(d2);
    std::cout << "BS formula: " << BS_call << std::endl;

    return 0;
}
