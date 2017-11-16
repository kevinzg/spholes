#ifndef SPHOLES_POINT_H
#define SPHOLES_POINT_H

#include <complex>
#include "Common.h"

namespace spholes {

class Point : public std::complex<real>
{
public:
    Point(spholes::real x, spholes::real y) : std::complex<spholes::real> (x, y) {}
    Point(const std::complex<spholes::real> &c) : Point(c.real(), c.imag()) {}
    inline spholes::real x() const { return this->real(); }
    inline spholes::real y() const { return this->imag(); }
};

}

#endif // SPHOLES_POINT_H
