#include "LLSBundle.h"

namespace FBM {

LLSBundle::LLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution, std::function<double(const Fiber&, const Fiber&)>& transferFunction, LatticeGenerator& latgen)
: Bundle(count, minIntervalLimit, maxIntervalLimit, distribution), m_transferFunc(transferFunction), m_latGen(latgen)
{
    synthesizeBundle();
}

LLSBundle::LLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution, std::function<double(const Fiber&, const Fiber&)>&& transferFunction, LatticeGenerator&& latgen)
: Bundle(count, minIntervalLimit, maxIntervalLimit, distribution), m_transferFunc(transferFunction), m_latGen(latgen)
{
    synthesizeBundle();
}

void LLSBundle::synthesizeBundle() {

    // Interval broken into corresponding number of fibers in bundle
    double delta = (m_maxIntervalLimit - m_minIntervalLimit) / (m_fiberCount - 1);

    double randomVariable = m_minIntervalLimit;

    // Vector for indexing purposes
    std::vector<double> stresses;

    // Strength of each fiber is now determined by the distribution
    double max = 0.0;
    while(stresses.size() < m_fiberCount) {
        double val = m_distribution(randomVariable);

        if (val > max) max = val;
        
        stresses.push_back(val);
        m_strength += val;
        randomVariable += delta;
    }

    std::shuffle(std::begin(stresses), std::end(stresses), std::default_random_engine(std::random_device()()));

    m_fibers.reserve(m_fiberCount);
    Fiber* arrayPtr = m_fibers.data();
    
    // generate central fiber that is the strongest one
    auto [x, y] = m_latGen();
    m_fibers.push_back({x, y, max, 0});
    m_breakingList.insert(arrayPtr);

    // Generate all other fibers and store accordingly
    for(int i = 0; i < stresses.size(); ++i) {
        if(stresses[i] == max) continue;

        auto [x, y] = m_latGen();
        m_fibers.push_back({x, y, stresses[i], 0});
        
        ++arrayPtr;
        m_breakingList.insert(arrayPtr);
    }

}

Fiber LLSBundle::weakestFiber() {
    return **m_breakingList.begin();
}

void LLSBundle::calibrateLoad(double load) {
    m_loadPerFiberPrev = m_loadPerFiber;
    m_loadPerFiber = load / m_fiberCount;

    Fiber* weakest = *m_breakingList.begin();
    m_breakingList.erase(m_breakingList.begin());

    if(!m_breakFlag) {
        weakest->load += (m_loadPerFiber - m_loadPerFiberPrev);
    }
    else {
        weakest->load += m_loadPerFiberPrev + m_deficit;
        m_breakFlag = false;
    }

    // weakest->load += (m_loadPerFiber - m_loadPerFiberPrev + m_deficit);

    if(weakest->load > load) weakest->load = m_loadPerFiber;
    m_breakingList.insert(weakest);
}

Fiber LLSBundle::breakFiber() {
    m_breakFlag = true;
    Fiber* brokenOne = *m_breakingList.begin();
    m_breakingList.erase(m_breakingList.begin());

    m_strength -= brokenOne->strength;
    m_fiberCount--;

    distributeLoad(brokenOne);

    return *brokenOne;
}

void LLSBundle::distributeLoad(const Fiber* brokenOne) {
    if (m_breakingList.size() == 0) return;
    
    double distributedLoadSum {0.0};

    for(auto it = m_breakingList.begin(); it != m_breakingList.end(); ) {
        Fiber* fb = *it;
        it = m_breakingList.erase(it);

        double carryForward = m_transferFunc(*fb, *brokenOne);
        fb->load += carryForward;
        distributedLoadSum += carryForward;

        m_breakingList.insert(fb);
    }

    m_deficit += (brokenOne->load - distributedLoadSum) / m_breakingList.size();

}

int LLSBundle::breakingListsize() const { return m_breakingList.size(); }

}