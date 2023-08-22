#include "quadtree.hpp"

const int QuadTree::MAX_CAPACITY = 4;

QuadTree::QuadTree(Rect rect)
    : QuadTree(rect, QuadTree::MAX_CAPACITY)
{}

QuadTree::QuadTree(Rect rect, int capacity)
    : rect(rect), capacity(capacity), divided(false), representative({}), empty(true)
{}

QuadTree::~QuadTree()
{
    for (QuadTree* child : childs)
        delete child;
}

int QuadTree::depth() const
{
    if (!divided)
        return 1;

    int max_depth = 0;

    for (const QuadTree* child : childs)
    {
        int depth = child->depth();

        if (depth > max_depth)
            max_depth = depth;
    }

    return 1 + max_depth;
}

int QuadTree::totalPoints() const
{
    if (!divided)
        return points.size();
    
    int total = 0;

    for (const QuadTree* child : childs)
        total += child->totalPoints();

    return total; 
}

bool QuadTree::contains(Vec2 point) const
{
    return rect.contains(point);
}

void QuadTree::subdivide()
{
    if (divided)
        return;
    
    float w = rect.w / 2;
    float h = rect.h / 2;

    childs.reserve(4);

    childs.push_back(new QuadTree(Rect{rect.x, rect.y, h, w}, capacity));
    childs.push_back(new QuadTree(Rect{rect.x + w, rect.y, h, w}, capacity));
    childs.push_back(new QuadTree(Rect{rect.x, rect.y + h, h, w}, capacity));
    childs.push_back(new QuadTree(Rect{rect.x + w, rect.y + h, h, w}, capacity));

    for (Vec2 point : points)
    {
        for (QuadTree* child : childs)
        {
            if (child->insert(point))
                break;
        }
    }

    divided = true;
}

bool QuadTree::insert(Vec2 point)
{
    if (!rect.contains(point))
        return false;
    
    representative = point;

    if (divided)
    {
        for (QuadTree* child : childs)
        {
            if (child->insert(point))
                break;
        }
    }
    else
    {
        points.emplace_back(point);

        if (points.size() > capacity)
            subdivide();
    }
    
    empty = false;
}