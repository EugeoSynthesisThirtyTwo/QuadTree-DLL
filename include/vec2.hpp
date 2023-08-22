#pragma once

#include <iostream>
#include <vector>
#include <optional>

struct Vec2
{
    float x;
    float y;

    Vec2();

    Vec2(float x, float y);

    float normSquared() const;

    float norm() const;

    float distSquared(Vec2 other) const;

    float dist(Vec2 other) const;

    bool operator==(const Vec2& other) const;

    bool operator!=(const Vec2& other) const;

    Vec2 operator+(const Vec2& other) const;

    Vec2 operator-(const Vec2& other) const;

    float operator*(const Vec2& other) const;

    Vec2 operator*(float coeff) const;

    Vec2 operator/(float coeff) const;

    std::optional<Vec2> closest(const std::vector<Vec2>& points) const;
};

Vec2 operator*(float coeff, Vec2 vec);

std::ostream& operator<<(std::ostream& os, const Vec2& vec);