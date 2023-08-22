#pragma once

#include <vector>
#include <optional>

#include "vec2.hpp"
#include "rect.hpp"

class QuadTree
{
private:
    static const int MAX_CAPACITY;

    Rect rect;
    int capacity;
    bool divided;

    std::vector<Vec2> points;
    std::optional<Vec2> representative;

    bool empty;
    std::vector<QuadTree*> childs;

public:
    QuadTree(Rect rect);

    QuadTree(Rect rect, int capacity);
    
    ~QuadTree();
    
    /*
        Return the depth of this quadtree.
    */
    int depth() const;
    
    /*
        Return the total number of points in this quadtree.
    */
    int totalPoints() const;
    
    /*
        Return true if this quadtree region contains the given point.
    */
    bool contains(Vec2 point) const;

    /*
        Subdivide this quadtree into four subnodes.
    */
    void subdivide();

    /*
        Add a point to the quadtree.
    */
    bool insert(Vec2 point);
};