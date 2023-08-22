#include <cmath>

#include "rect.hpp"

Rect::Rect()
{}

Rect::Rect(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h)
{}

bool Rect::contains(Vec2 point) const
{
    return point.x >= x && point.x <= x + w && point.y >= y && point.y <= y + h;
}

Vec2 Rect::projection(Vec2 point) const
{
    return Vec2{
        // contraint point.x à rester entre x et x + w
        std::min(std::max(point.x, x), x + w),
        // contraint point.y à rester entre y et y + h
        std::min(std::max(point.y, y), y + h)
    };
}

float Rect::distSquared(Vec2 point) const
{
    return point.distSquared(projection(point));
}

float Rect::dist(Vec2 point) const
{
    return point.dist(projection(point));
}

bool Rect::intersects(Rect other) const
{
    return !(x > other.x + other.w || x + w < other.x || y > other.y + h || y + h < other.y);
}

std::ostream& operator<<(std::ostream& os, const Rect& rect)
{
    return os << "Rect(x=" << rect.x << ", y=" << rect.y << ", w=" << rect.w << ", h=" << rect.h << ")";
}
