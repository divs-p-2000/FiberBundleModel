#include "Bundle.h"

namespace FBM {

Bundle::Bundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>& distribution)
: m_fiberCount {count},
m_minIntervalLimit {minIntervalLimit},
m_maxIntervalLimit {maxIntervalLimit},
m_distribution(distribution)
{}

Bundle::Bundle(size_t count, double minIntervalLimit, double maxIntervalLimit, std::function<double(double)>&& distribution)
: m_fiberCount {count},
m_minIntervalLimit {minIntervalLimit},
m_maxIntervalLimit {maxIntervalLimit},
m_distribution(distribution)
{}

size_t Bundle::size() const { return m_fiberCount; }
double Bundle::strength() const { return m_strength; }
std::function<double(double)>& Bundle::distribution() { return m_distribution; }
const std::function<double(double)>& Bundle::distribution() const { return m_distribution; }

}