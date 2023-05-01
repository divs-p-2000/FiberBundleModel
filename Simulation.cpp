#include "Simulation.h"

namespace FBM {

Simulation::Simulation(Bundle& testBundle, double loadPreset, double loadIncrement)
: m_testBundle(testBundle), m_loadIncrement {loadIncrement}  {
    m_load = (m_testBundle.size() - 1) * loadPreset;
    m_breakOffList.reserve(m_testBundle.size());
}

Simulation::Simulation(Bundle&& testBundle, double loadPreset, double loadIncrement)
: m_testBundle(testBundle), m_loadIncrement {loadIncrement}  {
    m_load = (m_testBundle.size() - 1) * loadPreset;
    m_breakOffList.reserve(m_testBundle.size());
}

size_t Simulation::timeElapsed() const {
    return m_timestep;
}

double Simulation::appliedLoad() const {
    return m_load;
}

size_t Simulation::totalAvalanches() const {
    return m_avalanches;
}

const std::vector<int>& Simulation::avalancheList() const {
    return m_avalancheList;
}
const std::vector<Fiber>& Simulation::breakingOrder() const {
    return m_breakOffList;
}

const std::vector<std::pair<Fiber, size_t>>& Simulation::breakingTimes() const {
    return m_breakOffTimes;
}

void Simulation::advance() {
    m_timestep++;
    m_testBundle.calibrateLoad(m_load);

    Fiber weakestOne = m_testBundle.weakestFiber();
    int avalancheSize = 0;

    while(weakestOne.load > weakestOne.strength) {
        avalancheSize++;
        Fiber brokenOne = m_testBundle.breakFiber();
        m_breakOffList.push_back(brokenOne);
        m_breakOffTimes.push_back({brokenOne, m_timestep});

        if(m_testBundle.size() == 0) break;

        m_testBundle.calibrateLoad(m_load);
        weakestOne = m_testBundle.weakestFiber();
    }

    if (avalancheSize > 0) {
        m_avalanches++;
    }
    m_avalancheList.push_back(avalancheSize);
    if(m_testBundle.size() > 0) m_load += m_loadIncrement;
}

void Simulation::run() {
    while(m_testBundle.size() > 0) advance();
}

}