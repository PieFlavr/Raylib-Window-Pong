#ifndef RAYLIB_CPP_INCLUDE_RADIANS_DEGREES_HPP_
#define RAYLIB_CPP_INCLUDE_RADIANS_DEGREES_HPP_

#include "./raylib.hpp"
#include "./raylib-cpp-utils.hpp"
#include "./raymath.hpp"

#ifndef RAYLIB_CPP_NO_MATH
#include <cmath>
#endif

namespace raylib {
    class Degree;

    /**
     * Radian type (allows automatic worry free conversion between radians and degrees)
     */
    class Radian {
        float value;
    public:
        Radian(const float radian) : value(radian) {}
        Radian(const Radian& other) : Radian(other.value) {}
        Radian() : Radian(0) {}
        Radian(const Degree d); 

        Radian& operator=(const Radian& other) = default;

        operator float() const { return value; }

        Radian operator+(const Radian other) const { return value + other.value; }
        Radian operator-(const Radian other) const { return value - other.value; }
        Radian operator*(const Radian other) const { return value * other.value; }
        Radian operator/(const Radian other) const { return value / other.value; }

        Radian& operator+=(const Radian other) { *this = *this + other; return *this; }
        Radian& operator-=(const Radian other) { *this = *this - other; return *this; }
        Radian& operator*=(const Radian other) { *this = *this * other; return *this; }
        Radian& operator/=(const Radian other) { *this = *this / other; return *this; }

        float DegreeValue() const;
    };

    /**
     * Degree type (allows automatic worry free conversion between radians and degrees)
     */
    class Degree {
        float value;
    public:
        Degree(float degree) : value(degree) {}
        Degree(const Degree& other) : Degree(other.value) {}
        Degree() : Degree(0) {}
        Degree(const Radian r) : Degree(float(r) * RAD2DEG) {}

        Degree& operator=(const Degree& other) = default;

        operator float() const { return value; }

        Degree operator+(const Degree other) const { return value + other.value; }
        Degree operator-(const Degree other) const { return value - other.value; }
        Degree operator*(const Degree other) const { return value * other.value; }
        Degree operator/(const Degree other) const { return value / other.value; }

        Degree& operator+=(const Degree other) { *this = *this + other; return *this; }
        Degree& operator-=(const Degree other) { *this = *this - other; return *this; }
        Degree& operator*=(const Degree other) { *this = *this * other; return *this; }
        Degree& operator/=(const Degree other) { *this = *this / other; return *this; }

        float RadianValue() const { return Radian(*this); }
    };

    inline Radian::Radian(const Degree d) : Radian(float(d) * DEG2RAD) {}
    inline float Radian::DegreeValue() const { return Degree(*this); }
}

#endif // RAYLIB_CPP_INCLUDE_RADIANS_DEGREES_HPP_