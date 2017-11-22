#ifndef SPHOLES_POINT_H
#define SPHOLES_POINT_H

#include <complex>
#include "Common.h"

namespace spholes {

class Point : public std::complex<real>
{
public:
    Point() {}
    Point(spholes::real x, spholes::real y) : std::complex<spholes::real> (x, y) {}
    Point(const std::complex<spholes::real> &c) : Point(c.real(), c.imag()) {}
    inline spholes::real x() const { return this->real(); }
    inline spholes::real y() const { return this->imag(); }
};

static real crossProduct(const Point &a, const Point &b)
{
    return a.x() * b.y() - a.y() * b.x();
}

static real dotProduct(const Point &a, const Point &b)
{
    return a.x() * b.x() + a.y() * b.y();
}

}

#endif // SPHOLES_POINT_H
