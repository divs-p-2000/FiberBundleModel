#include "LLSBundleOpt.h"

namespace FBM {

LLSBundleOpt::LLSBundleOpt(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution, std::function<double(int, int, int, int)>& transferFunction, LatticeGenerator& latgen)
: Bundle(count, minIntervalLimit, maxIntervalLimit, distribution), m_transferFunc(transferFunction), m_latGen(latgen)
{
    synthesizeBundle();
}

LLSBundleOpt::LLSBundleOpt(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution, std::function<double(int, int, int, int)>&& transferFunction, LatticeGenerator&& latgen)
: Bundle(count, minIntervalLimit, maxIntervalLimit, distribution), m_transferFunc(transferFunction), m_latGen(latgen)
{
    synthesizeBundle();
}

void LLSBundleOpt::synthesizeBundle() {

    // Interval broken into corresponding number of fibers in bundle
    double delta = (m_maxIntervalLimit - m_minIntervalLimit) / m_fiberCount;

    double randomVariable = m_minIntervalLimit + delta;

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

    // The edge length of the bounding box encompassing the lattice
    int array2Dsize = m_latGen.sizeDeterminationFunction()(m_fiberCount);
    
    // Resizing the m_lattice vector and initializing its elements with null pointers
    m_lattice.resize(array2Dsize);
    for(auto& row : m_lattice) {
        row.resize(array2Dsize, nullptr);
    }

    // Setup the array where the fibers are actually stored
    m_fibers.reserve(m_fiberCount);
    m_fiberArray = m_fibers.data();

    // generate central fiber that is the strongest one
    auto [x, y] = m_latGen();
    m_fibers.push_back(Fiber {x, y, max, 0});
    
    // Store pointers to central fiber in the lattice pointer matrix and set which determines breaking order
    getFiber(x, y) = m_fiberArray;
    m_breakingList.insert(m_fiberArray);

    // Generate all other fibers and store accordingly
    for(int i = 0; i < stresses.size(); ++i) {
        if(stresses[i] == max) continue;

        auto [x, y] = m_latGen();
        m_fibers.push_back(Fiber {x, y, stresses[i], 0});
        getFiber(x, y) = m_fiberArray + i + 1;
        m_breakingList.insert(m_fiberArray + i + 1);
    }

}

Fiber*& LLSBundleOpt::getFiber(int x, int y) {
    return m_lattice[y + (int)m_lattice.size()/2][x + (int)m_lattice.size()/2];
}

Fiber LLSBundleOpt::weakestFiber() {
    return **m_breakingList.begin();
}

void LLSBundleOpt::calibrateLoad(double load) {
    m_loadPerFiber = load / m_fiberCount;
    (*m_breakingList.begin())->load = m_loadPerFiber;
}

Fiber LLSBundleOpt::breakFiber() {
    Fiber brokenOne = **m_breakingList.begin();
    m_breakingList.erase(m_breakingList.begin());
    getFiber(brokenOne.x, brokenOne.y) = nullptr;

    m_strength -= brokenOne.strength;
    m_fiberCount--;

    distributeLoad(brokenOne.x, brokenOne.y);

    return brokenOne;
}

void LLSBundleOpt::distributeLoad(int x, int y) {}

}