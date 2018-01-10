#ifndef SPHOLES_COMMON_H
#define SPHOLES_COMMON_H

#include <cmath>

namespace spholes {

typedef double real;

const real EPS = 1e-9;
const real PI = 2 * std::acos(0);

static inline bool approx(real a, real b)
{
    real diff = a > b ? a - b : b - a;
    return diff < EPS;
}

}

#endif // SPHOLES_COMMON_H
