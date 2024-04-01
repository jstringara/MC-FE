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

    // simulate the model
    vector<double> S_t = model.simulate(S_0, 1.0, N_sim);

    // use the payoffs
    EU_Call call(100.0);
    EU_Put put(100.0);
    ClOption cl(100.0);

    double mean_call = 0.0;
    double mean_put = 0.0;

    for (auto& S : S_t) {
        mean_call += call(S);
        mean_put += put(S);
    }

    mean_call /= N_sim;
    mean_put /= N_sim;

    // display the mean payoff
    mean_call = mean_call * exp(-0.05);
    mean_put = mean_put * exp(-0.05);

    // compute the closed form solution
    double d1 = (log(S_0 / 100.0) + (0.05 + 0.2 * 0.2 / 2.0)) / (0.2 * sqrt(1.0));
    double d2 = d1 - 0.2 * sqrt(1.0);

    double call_price = S_0 * N(d1) - 100.0 * exp(-0.05) * N(d2);
    double put_price = 100.0 * exp(-0.05) * N(-d2) - S_0 * N(-d1);

    std::cout << "mean call payoff = " << mean_call << std::endl;
    std::cout << "mean put payoff = " << mean_put << std::endl;
    std::cout << "call price = " << call_price << std::endl;
    std::cout << "put price = " << put_price << std::endl;

    // check the put-call parity
    // C - P = S - K * exp(-r * T)
    double diff = mean_call - mean_put;
    double diff2 = S_0 - 100.0 * exp(-0.05);
    std::cout << "C - P = " << diff << std::endl;
    std::cout << "S - K * exp(-r * T) = " << diff2 << std::endl;

    return 0;
}
