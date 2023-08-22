#include <cmath>

#include "vec2.hpp"

Vec2::Vec2()
{}

Vec2::Vec2(float x, float y)
    : x(x), y(y)
{}

Vec2::Vec2(const Vec2& other)
{}

float Vec2::normSquared() const
{
    return x * x + y * y;
}

float Vec2::norm() const
{
    return std::sqrt(x * x + y * y);
}

float Vec2::distSquared(Vec2 other) const
{
    return (*this - other).normSquared();
}

float Vec2::dist(Vec2 other) const
{
    return (*this - other).norm();
}

bool Vec2::operator==(const Vec2& other) const
{
    return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2& other) const
{
    return x != other.x || y != other.y;
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return {x + other.x, y + other.y};
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return {x - other.x, y - other.y};
}

float Vec2::operator*(const Vec2& other) const
{
    return x * other.x + y * other.y;
}

Vec2 Vec2::operator*(float coeff) const
{
    return {x * coeff, y * coeff};
}

Vec2 Vec2::operator/(float coeff) const
{
    return {x / coeff, y / coeff};
}

std::optional<Vec2> Vec2::closest(const std::vector<Vec2>& points) const
{
    std::optional<Vec2> closest;
    float closestDistSquared;
    
    for (Vec2 candidate : points)
    {
        float candidateDistSquared = distSquared(candidate);

        if (!closest.has_value() || candidateDistSquared < closestDistSquared)
        {
            closest = candidate;
            closestDistSquared = candidateDistSquared;
        }
    }

    return closest;
}

Vec2 operator*(float coeff, Vec2 vec)
{
    return {coeff * vec.x, coeff * vec.y};
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
    return os << "Vec2(" << vec.x << ", " << vec.y << ")";
}
