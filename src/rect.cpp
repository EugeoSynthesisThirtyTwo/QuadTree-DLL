#include "rect.hpp"

Rect::Rect()
{}

Rect::Rect(float x, float y, float w, float h)
    : x(x), y(y), w(w), h(h)
{}

Rect::Rect(const Rect& other)
    : x(other.x), y(other.y), w(other.w), h(other.h)
{}

bool Rect::contains(Vec2 point) const
{
    return point.x >= x && point.x <= x + w && point.y >= y && point.y <= y + h;
}

bool Rect::intersects(Rect other) const
{
    return !(x > other.x + other.w || x + w < other.x || y > other.y + h || y + h < other.y);
}
