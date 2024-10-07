#pragma once
#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>

#define PI 3.14159265359
#define DEGREE 0.01745329251
#define RADIAN 57.2957795131

struct vec2
{
    float x = 0.0f;
    float y = 0.0f;

    // Constructors
    vec2();
    vec2(float xin, float yin);
    vec2(float angle);
    vec2(const vec2& other);

    // Constructors to support SFML types
    vec2(const sf::Vector2f& sfvec);
    vec2(const sf::Vector2i& sfvec);
    vec2(const sf::Vector2u& sfvec);

    // Conversion Operators to SFML types
    operator sf::Vector2f() const;
    operator sf::Vector2i() const;
    operator sf::Vector2u() const;

    // Operator Overloads
    bool operator == (const vec2& rhs) const;
    bool operator != (const vec2& rhs) const;

    vec2 operator + (const vec2& rhs) const;
    vec2 operator - (const vec2& rhs) const;
    vec2 operator / (const float val) const;
    vec2 operator * (const float val) const;
    vec2 operator - () const;

    void operator += (const vec2& rhs);
    void operator -= (const vec2& rhs);
    void operator *= (const float val);
    void operator /= (const float val);

    // Utility Functions
    float dot(const vec2& rhs) const;
    float cross2d(const vec2& rhs) const;
    float magnitudesq() const;
    float magnitude() const;
    vec2 normalize() const;
    float dist(const vec2& rhs) const;
    float angle() const;
    float angle(const vec2& rhs) const;
    vec2 perpendicular() const;  // Returns a vector perpendicular to this one
    vec2 lerp(const vec2& rhs, float t) const;  // Linear interpolation
    vec2 rotate(float angle) const;  // Rotate vector by angle in degrees

    // Stream output for debugging
    friend std::ostream& operator<<(std::ostream& os, const vec2& vec);
};