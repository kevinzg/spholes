#ifndef SPHOLES_POINT_H
#define SPHOLES_POINT_H

#include <complex>
#include "Common.h"

namespace spholes {

class Point : public std::complex<real>
{
public:
    Point(spholes::real x, spholes::real y) : std::complex<spholes::real> (x, y) {}
    inline spholes::real x() { return this->real(); }
    inline spholes::real y() { return this->imag(); }
};

}

#endif // SPHOLES_POINT_H
