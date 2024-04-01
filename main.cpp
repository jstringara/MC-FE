#include "model.hpp"
#include "payoff.hpp"

using BlackScholes = MC::BlackScholes;

// define the normal distribution function
double N(double x) {
    return 0.5 * erfc(-x / sqrt(2.0));
}


int main(int argc, char* argv[]){

    // create and print the model (B&S, r=5%, sigma=20%)
    BlackScholes model(0.05, 0.2);
    model.print(std::cout);

    // initial value
    double S_0 = 100.0;

    // convert the input to a number of simulations
    int N_sim = argv[1] ? std::stoi(argv[1]) : 1000;

    vector<double> S_t = model.simulate(S_0, 1.0, N_sim);

    // use the payoff
    EU_Call call(100.0);

    double mean = 0.0;

    for (auto& S : S_t)
        mean += call(S);

    mean /= N_sim;

    // display the mean payoff
    mean *= exp(-0.05);

    // compute the closed form solution
    double d1 = (log(S_0 / 100.0) + (0.05 + 0.2 * 0.2 / 2.0)) / (0.2 * sqrt(1.0));
    double d2 = d1 - 0.2 * sqrt(1.0);

    double call_price = S_0 * N(d1) - 100.0 * exp(-0.05) * N(d2);

    std::cout << "mean = " << mean << std::endl;
    std::cout << "call price = " << call_price << std::endl;

    return 0;
}
