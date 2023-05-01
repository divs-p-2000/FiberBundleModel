#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <utility>
#include <map>
#include "Bundle.h"

namespace FBM {

class Simulation {

public:
    Simulation(Bundle& testBundle, double loadPreset, double loadIncrement);
    Simulation(Bundle&& testBundle, double loadPreset, double loadIncrement);

    void advance();
    void run();

    size_t timeElapsed() const;
    double appliedLoad() const;
    size_t totalAvalanches() const;
    const std::vector<int>& avalancheList() const;
    const std::vector<Fiber>& breakingOrder() const;
    const std::vector<std::pair<Fiber, size_t>>& breakingTimes() const;

private:
    size_t m_timestep {0};
    double m_load {0.0};
    double m_loadIncrement {0.0};
    size_t m_avalanches {0};

    Bundle& m_testBundle;
    std::vector<int> m_avalancheList;
    std::vector<Fiber> m_breakOffList;
    std::vector<std::pair<Fiber, size_t>> m_breakOffTimes;
};

}

#endif