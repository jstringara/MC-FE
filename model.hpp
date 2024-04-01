#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <map>
#include <random>
#include <string>

using std::vector;
using std::string;
using std::map;
using std::ostream;
using std::normal_distribution;

namespace MC {
    
    // model class used to model the underlying stock dynamics
    class Model {

        protected:
            // random number generator (static member variable)
            static std::mt19937 m_rng;
            // name of the model
            string m_name;
            // parameters of the model {name, value}
            map<string, double> m_params;

        public:
            // constructor
            Model(string name, map<string, double> params) :
                m_name(name), m_params(params) {}
            // print the model
            void print(ostream& os) const;
            // pure virtual function to simulate the model from a single value
            virtual vector<double> simulate(double S, double dt, int N_sim) const = 0;
            // pure virtual function to simulate from multiple values
            virtual vector<double> simulate(const vector<double>& S, double dt) const = 0;
    };

    // Black-Scholes model
    class BlackScholes : public Model {
        
        public:
            // constructor
            BlackScholes(double r, double sigma, double d=0.0) :
                Model("Black-Scholes", {{"r", r}, {"sigma", sigma}, {"d", d}}) { 
                // check if the parameters are valid
                if (sigma < 0.0)
                    throw std::invalid_argument("sigma must be non-negative");
            }

            // simulate the model with Black Scholes dynamics
            vector<double> simulate(double S, double dt, int N_sim) const override;
            // simulate the model with Black Scholes dynamics (vectorized)
            vector<double> simulate(const vector<double>& S, double dt) const override;

        private:
            // generate a random number from a normal distribution
            double randn() const {
                normal_distribution<double> dist(0.0, 1.0);
                return dist(m_rng);
            }

    };

}

#endif // !#ifndef MODEL_HPP
