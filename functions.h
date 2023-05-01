#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Fiber.h"
#include <cmath>
#include <tuple>
#include <functional>

namespace FBM {

// Identity function
double identity(double x);

int squareLattice22side(int N);

// General Weibull distribution
class Weibull {
    double m_alpha, m_beta;

public:
    Weibull(double alpha, double beta) : m_alpha{alpha}, m_beta{beta} {};
    double operator()(double x) const;
};

// Modified Weibull distribution
class ModifiedWeibull {
    double m_alpha, m_beta;

public:
    ModifiedWeibull(double alpha, double beta) : m_alpha{alpha}, m_beta{beta} {};
    double operator()(double x) const;
};

// General Exponential Distribution of for alpha * x ^ beta * exp(- x ^ gamma) + delta
class ExponentialDistribution {
    double m_alpha, m_beta, m_gamma, m_delta;

public:
    ExponentialDistribution(double alpha, double beta, double gamma, double delta) : m_alpha{alpha}, m_beta{beta}, m_gamma{gamma}, m_delta{delta} {};
    double operator()(double x) const;
    double derivative(double x) const;
};

class EuclideanTransferFunction {
    double m_tp {0.0}, m_rog {0.0};

public:
    EuclideanTransferFunction(double transferParameter, double radiusOfGyration) : m_tp {transferParameter}, m_rog {radiusOfGyration} {}

    double operator()(const Fiber& fb1, const Fiber& fb2) const;
};

class LatticeGenerator {
public:
    virtual std::tuple<int, int> operator()() = 0;
    virtual std::function<int(int)>& sizeDeterminationFunction() = 0;
};

class SquareLatticeGenerator45 : public LatticeGenerator {
    int m_x {0}, m_y {0};
    int m_boxSize {0};
    int m_mode {0};
    size_t m_iterator {0};
    std::function<int(int)> m_sizeDeterminationFunction = squareLattice22side;

public:
    std::tuple<int, int> operator()();
    std::function<int(int)>& sizeDeterminationFunction();
};

}
#endif