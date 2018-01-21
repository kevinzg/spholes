#ifndef SPHOLES_POINT_H
#define SPHOLES_POINT_H

#include <complex>
#include "Common.h"

namespace spholes {

class PolarPoint;

class Point : public std::complex<real>
{
public:
    Point() {}
    Point(spholes::real x, spholes::real y) : std::complex<spholes::real> (x, y) {}
    Point(const std::complex<spholes::real> &c) : Point(c.real(), c.imag()) {}
    inline spholes::real x() const { return this->real(); }
    inline spholes::real y() const { return this->imag(); }
    inline PolarPoint toPolarPoint() const;

    inline bool operator< (const Point &b) const
    {
        if (this->x() != b.x())
            return this->x() < b.x();
        return this->y() < b.y();
    }
};

static inline real crossProduct(const Point &a, const Point &b)
{
    return a.x() * b.y() - a.y() * b.x();
}

static inline real dotProduct(const Point &a, const Point &b)
{
    return a.x() * b.x() + a.y() * b.y();
}

class PolarPoint : public std::complex<real>
{
public:
    PolarPoint() {}
    PolarPoint(spholes::real angle, spholes::real radius) : std::complex<spholes::real> (angle, radius) {}
    inline spholes::real angle() const { return this->real(); }
    inline spholes::real radius() const { return this->imag(); }

    inline Point toPoint() const { return Point(std::cos(angle()) * radius(), std::sin(angle()) * radius()); }
};

PolarPoint Point::toPolarPoint() const
{
    spholes::real angle = std::atan2(this->y(), this->x());
    return PolarPoint(angle < 0.0 ? angle + 2 * PI : angle,
                      std::hypot(this->x(), this->y()));
}

}

#endif // SPHOLES_POINT_H
