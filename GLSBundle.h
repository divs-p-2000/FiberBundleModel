#ifndef GLSBUNDLE_H
#define GLSBUNDLE_H

#include <queue>
#include <vector>
#include <random>
#include <algorithm>
#include "Bundle.h"

namespace FBM {

class GLSBundle : public Bundle {

public:
    GLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution);
    GLSBundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution);
    Fiber weakestFiber() override;
    Fiber breakFiber() override;
    void calibrateLoad(double load) override;

private:
    std::priority_queue<Fiber, std::vector<Fiber>, std::greater<Fiber>> m_heap;
    void synthesizeBundle() override;    
};

}

#endif