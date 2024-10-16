#pragma once
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>

#include "int.h"

#define PI 3.14159265359
#define DEGREE 0.01745329251
#define RADIAN 57.2957795131

// the actual template 2d mathematical vector that stores (x, y) pair
template <typename T>
struct vec2t
{
    T x{};
    T y{};

    // default constructor
    vec2t() = default;

    // construct from cartesian coordinates (x, y)
    vec2t(T x, T y)
        : x(x), y(y) {}

    // construct from an angle in degrees w.r.t x-axis
    vec2t(float angle)
        : x(std::cos(angle * DEGREE)), y(std::sin(angle * DEGREE)) {}

    // construct from another type of vec2t
    template <typename U>
    vec2t(const vec2t<U>& vec)
        : x((T)vec.x), y((T)vec.y) {}

    // copy constructor
    vec2t(const vec2t& other) = default;

    // construct from SFML's Vector2
    template <typename U>
    vec2t(const sf::Vector2<U>& sfvec)
        : x((T)sfvec.x), y((T)sfvec.y) {}

    // coversion to sf::Vector2<T>
    operator sf::Vector2<T> () const
    {
        return sf::Vector2<T>(x, y);
    }

    // the dot product with rhs
    T dot(const vec2t& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }

    // by imagining vec2t's as vec3t's, compute the z-coordinate of the cross product with rhs
    T cross(const vec2t& rhs) const
    {
        return x * rhs.y - y * rhs.x;
    }

    // the length or magnitude squared 
    T magnitudesq() const
    {
        return dot(*this);
    }

    // the actual length or magnitude
    T magnitude() const
    {
        return std::sqrt(dot(*this));
    }

    // normalize to make a unit vector
    vec2t normalize() const
    {
        T mag = magnitude();
        return mag == 0 ? vec2t{} : *this / mag;
    }

    // the distance from rhs
    float dist(const vec2t& rhs) const
    {
        return (*this - rhs).magnitude();
    }

    // the angle in degrees made by this vector w.r.t the x-axis
    float angle() const
    {
        return std::atan2(y, x) * RADIAN;
    }

    // the angle in degrees between this and rhs
    float angle(const vec2t& rhs) const
    {
        T dotprod = dot(rhs);
        T mag = magnitude() * rhs.magnitude();
        return std::acos(dotprod / mag) * RADIAN;
    }

    // one of the ways to receive a perpendicular vector from this one
    vec2t perpendicular() const
    {
        return vec2t(-y, x);
    }

    // linearly interpolate from this to rhs
    vec2t lerp(const vec2t& rhs, float t) const
    {
        return *this + t * (rhs - *this);
    }

    // rotate the vector by the specified angle in degrees
    vec2t rotate(float angle) const
    {
        float rad = angle * DEGREE;
        return vec2t(x * std::cos(rad) - y * std::sin(rad), x * std::sin(rad) + y * std::cos(rad));
    }
};

// ..................................................................................................
// equality
template <typename T>
bool operator== (const vec2t<T>& lhs, const vec2t<T>& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

// ..................................................................................................
// inequality
template <typename T>
bool operator!= (const vec2t<T>& lhs, const vec2t<T>& rhs)
{
    return !(lhs == rhs);
}

// ..................................................................................................
// addition
template <typename T>
vec2t<T> operator+ (const vec2t<T>& lhs, const vec2t<T>& rhs)
{
    return vec2t<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

// ..................................................................................................
// subtraction
template <typename T>
vec2t<T> operator- (const vec2t<T>& lhs, const vec2t<T>& rhs)
{
    return vec2t<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

// ..................................................................................................
// scalar multiplication
template <typename T>
vec2t<T> operator* (const vec2t<T>& vec, float scalar)
{
    return vec2t<T>(vec.x * scalar, vec.y * scalar);
}

// ..................................................................................................
// scalar division
template <typename T>
vec2t<T> operator/ (const vec2t<T>& vec, float scalar)
{
    return vec2t<T>(vec.x / scalar, vec.y / scalar);
}

// ..................................................................................................
// unary negation
template <typename T>
vec2t<T> operator- (const vec2t<T>& vec)
{
    return vec2t<T>(-vec.x, -vec.y);
}

// ..................................................................................................
// addition compound assignment
template <typename T>
vec2t<T>& operator+= (vec2t<T>& lhs, const vec2t<T>& rhs)
{
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

// ..................................................................................................
// subtraction compound assignment
template <typename T>
vec2t<T>& operator-= (vec2t<T>& lhs, const vec2t<T>& rhs)
{
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

// ..................................................................................................
// scalar multiplication compound assignment
template <typename T>
vec2t<T>& operator*= (vec2t<T>& vec, float scalar)
{
    vec.x *= scalar;
    vec.y *= scalar;
    return vec;
}

// ..................................................................................................
// scalar division compound assignment
template <typename T>
vec2t<T>& operator/= (vec2t<T>& vec, float scalar)
{
    vec.x /= scalar;
    vec.y /= scalar;
    return vec;
}
// ..................................................................................................
// to print a vec2t
template <typename T>
std::ostream& operator<< (std::ostream& os, const vec2t<T> vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

// ..................................................................................................
// most common vec2t types
typedef vec2t<float> vec2;  // this should be used for more accurate mathematical operations
typedef vec2t<i32> vec2i;
typedef vec2t<ui32> vec2ui;