#include "Fiber.h"

namespace FBM {

bool Fiber::operator<(const Fiber& other) const { return strength - load < other.strength - other.load; }

bool Fiber::operator>(const Fiber& other) const { return strength - load > other.strength - other.load; }

std::ostream& operator<<(std::ostream& os, const Fiber& fb) { return os << "[Index: (" << fb.x << ',' << fb.y << ")\t\tStrength: " << fb.strength << "\t\tLoad: " << fb.load << ']'; }

}