#include "vec2.h"

// Constructors
vec2::vec2()
{}


vec2::vec2(float xin, float yin)
    : x(xin), y(yin)
{}


vec2::vec2(float angle)
{
    float rad = angle * DEGREE;
    x = cosf(rad);
    y = sinf(rad);
}


vec2::vec2(const vec2& other)
    : x(other.x), y(other.y)
{}


// Constructors for SFML types
vec2::vec2(const sf::Vector2f& sfvec)
    : x(sfvec.x), y(sfvec.y)
{}


vec2::vec2(const sf::Vector2i& sfvec)
    : x((float)(sfvec.x)), y((float)(sfvec.y))
{}


vec2::vec2(const sf::Vector2u& sfvec)
    : x((float)(sfvec.x)), y((float)(sfvec.y))
{}


// Conversion Operators to SFML types
vec2::operator sf::Vector2f() const
{
    return sf::Vector2f(x, y);
}


vec2::operator sf::Vector2i() const
{
    return sf::Vector2i((int)(x), (int)(y));
}


vec2::operator sf::Vector2u() const
{
    return sf::Vector2u((unsigned int)(x), (unsigned int)(y));
}


// Operator Overloads
bool vec2::operator == (const vec2& rhs) const
{
    return x == rhs.x && y == rhs.y;
}


bool vec2::operator != (const vec2& rhs) const
{
    return !(*this == rhs);
}


vec2 vec2::operator + (const vec2& rhs) const
{
    return vec2(x + rhs.x, y + rhs.y);
}


vec2 vec2::operator - (const vec2& rhs) const
{
    return vec2(x - rhs.x, y - rhs.y);
}


vec2 vec2::operator / (const float val) const
{
    return vec2(x / val, y / val);
}


vec2 vec2::operator * (const float val) const
{
    return vec2(x * val, y * val);
}


vec2 vec2::operator - () const
{
    return vec2(-x, -y);
}


void vec2::operator += (const vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}


void vec2::operator -= (const vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}


void vec2::operator *= (const float val)
{
    x *= val;
    y *= val;
}


void vec2::operator /= (const float val)
{
    x /= val;
    y /= val;
}


// Utility Functions
float vec2::dot(const vec2& rhs) const
{
    return x * rhs.x + y * rhs.y;
}


float vec2::cross2d(const vec2& rhs) const
{
    return x * rhs.y - rhs.x * y;
}


float vec2::magnitudesq() const
{
    return x * x + y * y;
}


float vec2::magnitude() const
{
    return sqrtf(magnitudesq());
}


vec2 vec2::normalize() const
{
    float mag = magnitude();
    if (mag < 1e-6)  // Small magnitude check
        return vec2(0, 0); // Or return a unit vector in any direction, depending on your needs
    return *this / mag;
}


float vec2::dist(const vec2& rhs) const
{
    return (*this - rhs).magnitude();
}


float vec2::angle() const
{
    return atan2f(y, x) * RADIAN;
}


float vec2::angle(const vec2& rhs) const
{
    float dotprod = dot(rhs);
    float mags = magnitude() * rhs.magnitude();
    return acosf(dotprod / mags) * RADIAN;
}


vec2 vec2::perpendicular() const
{
    return vec2(-y, x);
}


vec2 vec2::lerp(const vec2& rhs, float t) const
{
    return *this + (rhs - *this) * t;
}


vec2 vec2::rotate(float angle) const
{
    float rad = angle * DEGREE;
    float cosine = cosf(rad);
    float sine = sinf(rad);
    return vec2(x * cosine - y * sine, x * sine + y * cosine);
}

// Stream output for debugging
std::ostream& operator<<(std::ostream& os, const vec2& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}
