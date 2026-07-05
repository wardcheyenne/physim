#pragma once
#include <cmath>
#include <stdexcept>
#include <ostream>

namespace physim {

struct Vector3 {
    double x, y, z;

    /**
    This is a function to construct a Vector3 from optional x, y, z components.
    Input(s):
    - x: the x component (default 0)
    - y: the y component (default 0)
    - z: the z component (default 0)
    Output(s):
    - None: initializes the vector's x, y, z members
    **/
    constexpr Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    /**
    This is a function to add two vectors component-wise.
    Input(s):
    - o: the vector to add to this one
    Output(s):
    - Vector3: a new vector equal to the component-wise sum
    **/
    Vector3 operator+(const Vector3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    /**
    This is a function to subtract another vector from this one component-wise.
    Input(s):
    - o: the vector to subtract from this one
    Output(s):
    - Vector3: a new vector equal to the component-wise difference
    **/
    Vector3 operator-(const Vector3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    /**
    This is a function to scale this vector by a scalar.
    Input(s):
    - s: the scalar to multiply each component by
    Output(s):
    - Vector3: a new vector with each component multiplied by s
    **/
    Vector3 operator*(double s)         const { return {x*s,   y*s,   z*s};   }
    /**
    This is a function to divide this vector by a scalar.
    Input(s):
    - s: the scalar to divide each component by
    Output(s):
    - Vector3: a new vector with each component divided by s
    **/
    Vector3 operator/(double s)         const { return {x/s,   y/s,   z/s};   }
    /**
    This is a function to add another vector into this one in place.
    Input(s):
    - o: the vector to add
    Output(s):
    - Vector3&: reference to this vector after the addition
    **/
    Vector3& operator+=(const Vector3& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    /**
    This is a function to subtract another vector from this one in place.
    Input(s):
    - o: the vector to subtract
    Output(s):
    - Vector3&: reference to this vector after the subtraction
    **/
    Vector3& operator-=(const Vector3& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
    /**
    This is a function to scale this vector by a scalar in place.
    Input(s):
    - s: the scalar to multiply each component by
    Output(s):
    - Vector3&: reference to this vector after the scaling
    **/
    Vector3& operator*=(double s)         { x*=s;   y*=s;   z*=s;   return *this; }

    /**
    This is a function to compute the dot product of this vector with another.
    Input(s):
    - o: the other vector
    Output(s):
    - double: the dot product x*o.x + y*o.y + z*o.z
    **/
    double dot(const Vector3& o)    const { return x*o.x + y*o.y + z*o.z; }
    /**
    This is a function to compute the cross product of this vector with another.
    Input(s):
    - o: the other vector
    Output(s):
    - Vector3: the vector perpendicular to both, following the right-hand rule
    **/
    Vector3 cross(const Vector3& o) const {
        return {y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x};
    }
    /**
    This is a function to compute the squared Euclidean norm of the vector.
    Input(s):
    - None
    Output(s):
    - double: x*x + y*y + z*z
    **/
    double norm2() const { return x*x + y*y + z*z; }
    /**
    This is a function to compute the Euclidean norm (magnitude) of the vector.
    Input(s):
    - None
    Output(s):
    - double: the square root of norm2()
    **/
    double norm()  const { return std::sqrt(norm2()); }

    /**
    This is a function to compute a unit-length vector pointing in the same direction as this one.
    Input(s):
    - None
    Output(s):
    - Vector3: this vector divided by its norm; throws std::domain_error if the vector's length is effectively zero
    **/
    Vector3 normalized() const {
        double n = norm();
        if (n < 1e-300)
            throw std::domain_error("Cannot normalize a zero-length vector");
        return {x/n, y/n, z/n};
    }

    /**
    This is a function to check whether the vector's magnitude is within a small tolerance of zero.
    Input(s):
    - eps: the tolerance below which the squared norm is considered zero (default 1e-15)
    Output(s):
    - bool: true if norm2() is less than eps squared, false otherwise
    **/
    bool is_zero(double eps = 1e-15) const { return norm2() < eps * eps; }

    /**
    This is a function to construct the zero vector.
    Input(s):
    - None
    Output(s):
    - Vector3: the vector (0, 0, 0)
    **/
    static Vector3 zero() { return {0,0,0}; }

    /**
    This is a function to write a vector's components to an output stream in "(x, y, z)" format.
    Input(s):
    - os: the output stream to write to
    - v: the vector to format
    Output(s):
    - std::ostream&: reference to the stream after writing
    **/
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    }
};

/**
This is a function to scale a vector by a scalar with the scalar on the left-hand side.
Input(s):
- s: the scalar multiplier
- v: the vector to scale
Output(s):
- Vector3: the result of v * s
**/
inline Vector3 operator*(double s, const Vector3& v) { return v * s; }

} //namespace physim
