#include "model.hpp"


using BlackScholes = MC::BlackScholes;


int main(int argc, char* argv[]){

    BlackScholes model(0.05, 0.2);
    model.print(std::cout);

    // 1000 simulations and return the mean
    double S_0 = 100.0;

    // convert the input to a number of simulations
    int N = argv[1] ? std::stoi(argv[1]) : 1000;

    double mean = 0.0;

    for (int i = 0; i < N; ++i) {
        double S_t = model.simulate(S_0, 1.0);
        mean += S_t;
    }

    mean /= N;

    std::cout << "mean = " << mean << std::endl;
    std::cout << S_0 * exp(0.05) << std::endl;

    return 0;
}
