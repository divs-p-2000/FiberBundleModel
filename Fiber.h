#ifndef FIBER_H
#define FIBER_H

#include <iostream>

namespace FBM {

struct Fiber {
    int x {0};
    int y {0};
    double strength {0.0};
    double load {0.0};

    bool operator<(const Fiber& other) const;
    bool operator>(const Fiber& other) const;
};

class FiberComparator {
public:
    bool operator()(const Fiber& a, const Fiber& b) const {
        return (a.strength - a.load) < (b.strength - b.load);
    }
};

class FiberPointerComparator {
public:
    bool operator()(const Fiber* a, const Fiber* b) const {
        return (a->strength - a->load) < (b->strength - b->load);
    }
};

std::ostream& operator<<(std::ostream& os, const Fiber& fb);

}

#endif