#include "functions.h"

namespace FBM {

double identity(double x) { return x; }

int squareLattice22side(int N) { return (int) 2 * ceil(0.5 * sqrt(2*N - 1) - 1) + 1; }

double Weibull::operator()(double x) const { return m_alpha * pow(x, m_beta - 1) * exp(-1 * pow(x, m_beta)); }

double ModifiedWeibull::operator()(double x) const { return m_alpha * pow(x, m_beta) * exp(-1 * pow(x, m_beta - 1)); }

double ExponentialDistribution::operator()(double x) const { return m_alpha * pow(x, m_beta) * exp(-1 * pow(x, m_gamma)) + m_delta; }

double ExponentialDistribution::derivative(double x) const { return m_alpha * -1 * pow(x, m_beta - 1) * exp(-1 * pow(x, m_gamma)) * (m_gamma * pow(x, m_gamma) - m_beta); }

double EuclideanTransferFunction::operator()(const Fiber& fb1, const Fiber& fb2) const {
    return (m_tp * m_rog * fb2.load) / sqrt(pow(fb1.x - fb2.x, 2) + pow(fb1.y - fb2.y, 2));
}

std::tuple <int, int> SquareLatticeGenerator45::operator()() {
    if(m_boxSize == 0) {
        m_boxSize++;
        m_x = -m_boxSize;
        m_y = -m_boxSize;
        return {0, 0};
    }

    int x = m_x, y = m_y;

    switch (m_mode) {
        case 0:
        m_y += 2;
        break;

        case 1:
        m_x += 2;
        break;

        case 2:
        m_y -= 2;
        break;

        case 3:
        m_x -= 2;
        break;
    }

    if(m_x == -m_boxSize && m_y == m_boxSize) m_mode = 1;
    else if(m_x == m_boxSize && m_y == m_boxSize) m_mode = 2;
    else if(m_x == m_boxSize && m_y == -m_boxSize) m_mode = 3;
    else if(m_x == -m_boxSize && m_y == -m_boxSize) {
        m_boxSize++;
        m_x = -m_boxSize;
        m_y = -m_boxSize;
        m_mode = 0;
    }

    return {x, y};
}

std::function<int(int)>& SquareLatticeGenerator45::sizeDeterminationFunction() { return m_sizeDeterminationFunction; }

}