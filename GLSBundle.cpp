#include "GLSBundle.h"

namespace FBM {

GLSBundle::GLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution)
: Bundle(count, minIntervalLimit, maxIntervalLimit, distribution)
{
    synthesizeBundle();
}

GLSBundle::GLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution)
: Bundle(count, minIntervalLimit, maxIntervalLimit, distribution)
{
    synthesizeBundle();
}

void GLSBundle::synthesizeBundle() {

    // Interval broken into corresponding number of fibers in bundle
    double delta = (m_maxIntervalLimit - m_minIntervalLimit) / (m_fiberCount - 1);

    double randomVariable = m_minIntervalLimit + delta;

    // Vector for indexing purposes
    std::vector<double> stresses;

    // Strength of each fiber is now determined by the distribution
    while(stresses.size() < m_fiberCount) {
        stresses.push_back(m_distribution(randomVariable));
        randomVariable += delta;
    }

    // shuffling the vector to scramble indices
    std::shuffle(std::begin(stresses), std::end(stresses), std::default_random_engine(std::random_device()()));

    // Storing all fibers in min heap for fast access to lowest strength fiber
    for(int i = 0; i < stresses.size(); ++i) {
        m_heap.push(Fiber{i, 0, stresses[i], 0});
        m_strength += stresses[i];
    }
}

Fiber GLSBundle::weakestFiber() {
    return m_heap.top();
}

Fiber GLSBundle::breakFiber() {
    Fiber brokenOne = m_heap.top();
    m_heap.pop();

    m_strength -= brokenOne.strength;
    m_fiberCount--;

    return brokenOne;
}

void GLSBundle::calibrateLoad(double load) {
    Fiber weakestOne = m_heap.top();
    m_heap.pop();
    weakestOne.load = load / m_fiberCount;
    m_heap.push(weakestOne);
}

}