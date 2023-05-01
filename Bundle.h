#ifndef BUNDLE_H
#define BUNDLE_H

#include <functional>
#include "Fiber.h"

namespace FBM {

class Bundle {

public:
    size_t size() const;                                        // The number of fibers intact in the bundle
    double strength() const;                                    // The total strength of fibers in the bundle
    std::function<double(double)>& distribution();              // Get reference to the distribution function
    const std::function<double(double)>& distribution() const;
    virtual Fiber weakestFiber() = 0;                           // Get weakest Fiber in the bundle
    virtual Fiber breakFiber() = 0;                             // Break the weakest fiber in the bundle
    virtual void calibrateLoad(double load) = 0;                // Redistribute loads across all fibers in the bundle

protected:
    size_t m_fiberCount {0};
    double m_strength {0.0};
    double m_minIntervalLimit {0.0};
    double m_maxIntervalLimit {0.0};
    std::function<double(double)> m_distribution;

    Bundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution);
    Bundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution);
    virtual void synthesizeBundle() {};
};

}

#endif