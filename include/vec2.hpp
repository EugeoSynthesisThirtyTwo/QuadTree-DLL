#pragma once

struct Vec2
{
    float x, y;

    Vec2();

    Vec2(float x, float y);

    Vec2(const Vec2& other);

    void print();
};