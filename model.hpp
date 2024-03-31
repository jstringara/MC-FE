#ifndef MODEL_HPP
#define MODEL_HPP

#include <iostream>
#include <map>
#include <random>
#include <string>
#include <cmath>

namespace MC {
    
    // model class used to model the underlying stock dynamics
    class Model {

        protected:
            // random number generator (static member variable)
            static std::mt19937 m_rng;
            // name of the model
            std::string m_name;
            // parameters of the model {name, value}
            std::map<std::string, double> m_params;

        public:
            // constructor
            Model(std::string name, std::map<std::string, double> params) :
                m_name(name), m_params(params) {}
            // print the model
            void print(std::ostream& os) const;
            // pure virtual function to simulate the model
            virtual double simulate(double S, double dt) const = 0;

    };

    // Black-Scholes model
    class BlackScholes : public Model {
        
        public:
            // constructor
            BlackScholes(double r, double sigma) :
                Model("Black-Scholes", {{"r", r}, {"sigma", sigma}}) {
                // check if the parameters are valid
                if (sigma < 0.0)
                    throw std::invalid_argument("sigma must be non-negative");
            }

            // simulate the model with Black Scholes dynamics
            double simulate(double S, double dt) const override;
                    

        private:
            // generate a random number from a normal distribution
            double randn() const {
                std::normal_distribution<double> dist(0.0, 1.0);
                return dist(m_rng);
            }

    };

}

#endif // !#ifndef MODEL_HPP
