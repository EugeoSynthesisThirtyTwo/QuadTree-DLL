#include "vec2.hpp"

struct Rect
{
    float x;
    float y;
    float w;
    float h;

    Rect();

    Rect(float x, float y, float w, float h);

    Rect(const Rect& other);

    bool contains(Vec2 point) const;

    bool intersects(Rect other) const;
};