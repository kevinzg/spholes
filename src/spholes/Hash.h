#ifndef SPHOLES_HASH_H
#define SPHOLES_HASH_H

#include <functional>
#include "Point.h"

namespace std {

template<>
class hash<spholes::Point>
{
public:
    size_t operator()(const spholes::Point& point) const
    {
        size_t seed = 0;
        seed ^= hash<spholes::real>()(point.x()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash<spholes::real>()(point.y()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

}
#endif // SPHOLES_HASH_H
