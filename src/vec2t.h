#pragma once
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#define PI 3.14159265359
#define DEGREE 0.01745329251
#define RADIAN 57.2957795131

template <typename T>
struct vec2t
{
    T x = 0;
    T y = 0;

    // Constructors
    vec2t();
    vec2t(T xin, T yin);
    vec2t(float angle);
    vec2t(const vec2t& other);

    // Constructors to support SFML types
    vec2t(const sf::Vector2f& sfvec);
    vec2t(const sf::Vector2i& sfvec);
    vec2t(const sf::Vector2u& sfvec);

    // Conversion Operators to SFML types
    operator sf::Vector2f() const;
    operator sf::Vector2i() const;
    operator sf::Vector2u() const;

    // Operator Overloads
    bool operator == (const vec2t& rhs) const;
    bool operator != (const vec2t& rhs) const;

    vec2t operator + (const vec2t& rhs) const;
    vec2t operator - (const vec2t& rhs) const;
    vec2t operator / (const float val) const;
    vec2t operator * (const float val) const;
    vec2t operator - () const;

    void operator += (const vec2t& rhs);
    void operator -= (const vec2t& rhs);
    void operator *= (const float val);
    void operator /= (const float val);

    // Utility Functions
    float dot(const vec2t& rhs) const;
    float cross2d(const vec2t& rhs) const;
    float magnitudesq() const;
    float magnitude() const;
    vec2t normalize() const;
    float dist(const vec2t& rhs) const;
    float angle() const;
    float angle(const vec2t& rhs) const;
    vec2t perpendicular() const;  // Returns a vector perpendicular to this one
    vec2t lerp(const vec2t& rhs, float t) const;  // Linear interpolation
    vec2t rotate(float angle) const;  // Rotate vector by angle in degrees

    // Stream output for debugging
    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const vec2t<U>& vec);
};


// Constructors
template <typename T>
vec2t<T>::vec2t()
{
}


template <typename T>
vec2t<T>::vec2t(T xin, T yin)
    : x(xin), y(yin)
{
}


template <typename T>
vec2t<T>::vec2t(float angle)
{
    float rad = angle * DEGREE;
    x = (T)cosf(rad);
    y = (T)sinf(rad);
}


template <typename T>
vec2t<T>::vec2t(const vec2t& other)
    : x(other.x), y(other.y)
{
}


// Constructors for SFML types
template <typename T>
vec2t<T>::vec2t(const sf::Vector2f& sfvec)
    : x(sfvec.x), y(sfvec.y)
{
}


template <typename T>
vec2t<T>::vec2t(const sf::Vector2i& sfvec)
    : x((float)(sfvec.x)), y((float)(sfvec.y))
{
}


template <typename T>
vec2t<T>::vec2t(const sf::Vector2u& sfvec)
    : x((float)(sfvec.x)), y((float)(sfvec.y))
{
}


// Conversion Operators to SFML types
template <typename T>
vec2t<T>::operator sf::Vector2f() const
{
    return sf::Vector2f(x, y);
}


template <typename T>
vec2t<T>::operator sf::Vector2i() const
{
    return sf::Vector2i((int)(x), (int)(y));
}


template <typename T>
vec2t<T>::operator sf::Vector2u() const
{
    return sf::Vector2u((unsigned int)(x), (unsigned int)(y));
}


// Operator Overloads
template <typename T>
bool vec2t<T>::operator == (const vec2t& rhs) const
{
    return x == rhs.x && y == rhs.y;
}


template <typename T>
bool vec2t<T>::operator != (const vec2t& rhs) const
{
    return !(*this == rhs);
}


template <typename T>
vec2t<T> vec2t<T>::operator + (const vec2t& rhs) const
{
    return vec2t(x + rhs.x, y + rhs.y);
}


template <typename T>
vec2t<T> vec2t<T>::operator - (const vec2t& rhs) const
{
    return vec2t(x - rhs.x, y - rhs.y);
}


template <typename T>
vec2t<T> vec2t<T>::operator / (const float val) const
{
    return vec2t(x / val, y / val);
}


template <typename T>
vec2t<T> vec2t<T>::operator * (const float val) const
{
    return vec2t(x * val, y * val);
}


template <typename T>
vec2t<T> vec2t<T>::operator - () const
{
    return vec2t(-x, -y);
}


template <typename T>
void vec2t<T>::operator += (const vec2t& rhs)
{
    x += rhs.x;
    y += rhs.y;
}


template <typename T>
void vec2t<T>::operator -= (const vec2t& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}


template <typename T>
void vec2t<T>::operator *= (const float val)
{
    x *= val;
    y *= val;
}


template <typename T>
void vec2t<T>::operator /= (const float val)
{
    x /= val;
    y /= val;
}


// Utility Functions
template <typename T>
float vec2t<T>::dot(const vec2t& rhs) const
{
    return x * rhs.x + y * rhs.y;
}


template <typename T>
float vec2t<T>::cross2d(const vec2t& rhs) const
{
    return x * rhs.y - rhs.x * y;
}


template <typename T>
float vec2t<T>::magnitudesq() const
{
    return x * x + y * y;
}


template <typename T>
float vec2t<T>::magnitude() const
{
    return sqrtf(magnitudesq());
}


template <typename T>
vec2t<T> vec2t<T>::normalize() const
{
    float mag = magnitude();
    if (mag < 1e-6)  // Small magnitude check
        return vec2t(0, 0); // Or return a unit vector in any direction, depending on your needs
    return *this / mag;
}


template <typename T>
float vec2t<T>::dist(const vec2t& rhs) const
{
    return (*this - rhs).magnitude();
}


template <typename T>
float vec2t<T>::angle() const
{
    return atan2f(y, x) * RADIAN;
}


template <typename T>
float vec2t<T>::angle(const vec2t& rhs) const
{
    float dotprod = dot(rhs);
    float mags = magnitude() * rhs.magnitude();
    return acosf(dotprod / mags) * RADIAN;
}


template <typename T>
vec2t<T> vec2t<T>::perpendicular() const
{
    return vec2t(-y, x);
}


template <typename T>
vec2t<T> vec2t<T>::lerp(const vec2t& rhs, float t) const
{
    return *this + (rhs - *this) * t;
}


template <typename T>
vec2t<T> vec2t<T>::rotate(float angle) const
{
    float rad = angle * DEGREE;
    float cosine = cosf(rad);
    float sine = sinf(rad);
    return vec2t(x * cosine - y * sine, x * sine + y * cosine);
}

// Stream output for debugging
template <typename U>
std::ostream& operator<<(std::ostream& os, const vec2t<U>& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

typedef vec2t<float> vec2;
typedef vec2t<int32_t> vec2i;
typedef vec2t<uint32_t> vec2ui;