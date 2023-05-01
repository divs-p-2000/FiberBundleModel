#ifndef LLSBundleOpt_H
#define LLSBundleOpt_H

#include <queue>
#include <vector>
#include <random>
#include <set>
#include <algorithm>
#include "Bundle.h"
#include "functions.h"

namespace FBM {

class LLSBundleOpt : public Bundle {
    LLSBundleOpt(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution, std::function<double(int, int, int, int)>& transferFunction, LatticeGenerator& latgen);
    LLSBundleOpt(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution, std::function<double(int, int, int, int)>&& transferFunction, LatticeGenerator&& latgen);
    Fiber weakestFiber() override;
    Fiber breakFiber() override;
    void calibrateLoad(double load) override;

private:
    LatticeGenerator& m_latGen;
    std::function<double(int, int, int, int)> m_transferFunc;
    std::set<Fiber*, FiberPointerComparator> m_breakingList;
    std::vector<std::vector<Fiber*>> m_lattice;
    std::vector<Fiber> m_fibers;
    Fiber* m_fiberArray;

    double m_loadPerFiber {0.0};
    
    void synthesizeBundle() override;
    Fiber*& getFiber(int x, int y);
    void distributeLoad(int x, int y);
};

}

#endif