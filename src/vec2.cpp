#include <iostream>
#include "vec2.hpp"

Vec2::Vec2()
{}

Vec2::Vec2(float x, float y)
    : x(x), y(y)
{}

Vec2::Vec2(const Vec2& other)
{}

void Vec2::print()
{
    std::cout << "Vec2(" << x << ", " << y << ")\n";
}