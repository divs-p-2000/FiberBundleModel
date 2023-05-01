#ifndef LLSBUNDLE_H
#define LLSBUNDLE_H

#include <queue>
#include <vector>
#include <random>
#include <set>
#include <algorithm>
#include "Bundle.h"
#include "functions.h"

namespace FBM {

class LLSBundle : public Bundle {
public:
    LLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution, std::function<double(const Fiber&, const Fiber&)>& transferFunction, LatticeGenerator& latgen);
    LLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution, std::function<double(const Fiber&, const Fiber&)>&& transferFunction, LatticeGenerator&& latgen);
    Fiber weakestFiber() override;
    Fiber breakFiber() override;
    void calibrateLoad(double load) override;
    int breakingListsize() const;

private:
    LatticeGenerator& m_latGen;
    std::function<double(const Fiber&, const Fiber&)> m_transferFunc;
    std::multiset<Fiber*, FiberPointerComparator> m_breakingList;
    std::vector<Fiber> m_fibers;

    double m_loadPerFiber {0.0};
    double m_loadPerFiberPrev {0.0};
    double m_deficit {0.0};
    bool m_breakFlag {false};
    
    void synthesizeBundle() override;
    void distributeLoad(const Fiber* brokenFiber);
};

}

#endif