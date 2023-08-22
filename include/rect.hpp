#include "vec2.hpp"

struct Rect
{
    float x;
    float y;
    float w;
    float h;

    Rect();

    Rect(float x, float y, float w, float h);

    bool contains(Vec2 point) const;
    
    Vec2 projection(Vec2 point) const;
    
    float distSquared(Vec2 point) const;

    float dist(Vec2 point) const;

    bool intersects(Rect other) const;
};

std::ostream& operator<<(std::ostream& os, const Rect& rect);