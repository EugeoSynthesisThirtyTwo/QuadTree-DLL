#include "quadtree.hpp"

const int QuadTree::MAX_CAPACITY = 4;
float QuadTree::OBJECT_RADIUS = 1;

QuadTree::QuadTree(Rect rect, int capacity)
    : rect(rect), capacity(capacity), divided(false), representative({}), empty(true)
{
    points.reserve(capacity);
}

QuadTree::~QuadTree()
{
    for (QuadTree* child : childs)
        delete child;
}

int QuadTree::depth() const
{
    if (!divided)
    {
        std::cout << this << "(" << points.size() << ")" << " -> None" << std::endl;
        return 1;
    }

    int max_depth = 0;

    for (const QuadTree* child : childs)
    {
        std::cout << this << "(" << points.size() << ")" << " -> " << child << std::endl;
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

bool QuadTree::intersects(Rect rect) const
{
    return this->rect.intersects(rect);
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

    points.clear();
    points.reserve(capacity);
    divided = true;
}

void QuadTree::merge()
{
    if (!divided)
        return;
    
    // counting all points every time is very costly, so we count only when all four children are leaves
    
    for (const QuadTree* child : childs)
        if (child->divided)
            return;
    
    int total = totalPoints();

    if (total > capacity)
        return;
    
    // theoretically, there is no need to recursively merge children as they are merged in the supress method called before the current merge
    
    divided = false;
    points.clear();
    points.reserve(total);
    
    for (QuadTree* child : childs)
    {
        for (Vec2 vec2 : child->points)
            points.push_back(vec2);
        
        delete child;
    }
    
    childs.clear();
}

bool QuadTree::insert(Vec2 point)
{
    bool inserted = false;

    if (!rect.contains(point))
    {
        std::cout << rect << ".contains(" << point << ") -> false" << std::endl;
        return inserted;
    }
    
    representative = point;

    if (divided)
    {
        for (QuadTree* child : childs)
        {
            if (child->insert(point))
            {
                inserted = true;
                break;
            }
        }
    }
    else
    {
        points.emplace_back(point);
        inserted = true;

        if (points.size() > capacity)
            subdivide();
    }
    
    empty = false;
    return inserted;
}

void QuadTree::suppress(Vec2 point)
{
    if (!rect.contains(point))
        return;
    
    if (representative.has_value() && point == representative.value())
        representative = {};

    if (divided)
    {
        for (QuadTree* child : childs)
        {
            child->suppress(point);
            if (!representative.has_value() && child->representative.has_value())
            {
                representative = child->representative.value();
            }
        }

        merge();

        if (totalPoints() == 0)
            empty = true;
    }
    else
    {
        std::vector<Vec2>::iterator iter = points.begin();
        std::vector<Vec2>::iterator end = points.end();
        
        while (iter != end)
        {
            if (*iter == point)
            {
                iter = points.erase(iter);
                break;
            }
        }

        if (points.size() > 0)
        {
            representative = points[0];
        }
        else
        {
            empty = true;
        }
    }
}

std::optional<Vec2> QuadTree::naiveClosest(Vec2 point, float radius) const
{
    std::vector<Vec2> candidates;
    queryCircle(point, radius, candidates);
    return point.closest(candidates);
}

/*
    Return the closest point to the given point.
    Algorithm :
        Maintain a list of interesting squares
        sqrs_0 is the root of the quadtree
        sqrs_{i+1} are the squares whose distance to the point is less than the current minimum distance among the children of sqrs_i.
        The current minimum distance is the distance between the point and the best representative yet.
        Returning the best representative gives the closest point.
*/
std::optional<Vec2> QuadTree::closest(Vec2 point)
{
    if (!divided)
    {
        return point.closest(points);
    }
    
    std::vector<QuadTree*> sqrs;
    sqrs.push_back(this);
    std::vector<QuadTree*> newSqrs;
    std::vector<Vec2> representatives;
    std::optional<Vec2> best;

    while (sqrs.size() != 0)
    {
        /*
            /!\ only put non empty squares in sqrs
            change here to include capacity > 1 trees
        */
        representatives.clear();
        newSqrs.clear();
        
        for (const QuadTree* sqr : sqrs)
        {
            if (sqr->representative.has_value())
                representatives.emplace_back(*sqr->representative);
        }

        std::optional<Vec2> closestRepresentative = point.closest(representatives);

        if (closestRepresentative.has_value())
            best = closestRepresentative;
        
        for (QuadTree* sqr : sqrs)
        {
            if (!sqr->divided)
                // In this case, the closest point in sqr is already accounted for in best_rep and we can skip to the next square.
                continue;
            
            for (QuadTree* subSqr : sqr->childs)
            {
                if (!subSqr->empty && (!best.has_value() || subSqr->rect.dist(point) < best.value().dist(point)))
                {
                    newSqrs.push_back(subSqr);

                    if (!subSqr->divided)
                    {
                        // the representative is changed to the closest :
                        subSqr->representative = point.closest(subSqr->points);
                    }
                }
            }
        }

        sqrs = newSqrs;
    }

    return best;
}

/*
    Return the closest distance to the given point.
    Depth search algorithm.
*/
float QuadTree::closestDepth(Vec2 point, std::optional<Vec2>& bestPoint, bool notEquals, float bestDist) const
{
    if (divided)
    {
        for (const QuadTree* child : childs)
        {
            float d = child->rect.distSquared(point);

            if (d < bestDist || bestDist < 0)
                bestDist = child->closestDepth(point, bestPoint, notEquals, bestDist);
        }
    }
    else
    {
        for (Vec2 other : points)
        {
            if (notEquals && other == point)
                continue;
            
            float d = point.distSquared(other);

            if (d < bestDist || bestDist < 0)
            {
                bestDist = d;
                bestPoint = other;
            }
        }
    }

    return bestDist;
}

/*
    Return the closest point to the given point.
    Depth search algorithm.
*/
std::optional<Vec2> QuadTree::closestDepth(Vec2 point, bool notEquals) const
{
    std::optional<Vec2> closest;
    closestDepth(point, closest, notEquals);
    return closest;
}

/*
    Append in buffer all points in the given rectangle.
*/
void QuadTree::queryRect(Rect rect, std::vector<Vec2>& buffer) const
{
    if (!this->rect.intersects(rect))
        return;

    if (!divided)
    {
        for (Vec2 point : points)
        {
            if (rect.contains(point))
                buffer.emplace_back(point);
        }
    }
    else
    {
        for (QuadTree* child : childs)
            child->queryRect(rect, buffer);
    }
}

/*
    Append in buffer all points in the given circle.
*/
void QuadTree::queryCircle(Vec2 center, float radius, std::vector<Vec2>& buffer) const
{
    if (!intersectsCircle(center, radius))
        return;

    if (!divided)
    {
        float radiusSquared = radius * radius;

        for (Vec2 point : points)
            if (point.distSquared(center) <= radiusSquared)
                buffer.emplace_back(point);
    }
    else
    {
        for (QuadTree* child : childs)
            child->queryCircle(center, radius, buffer);
    }
}

/*
    Return true if the given circle intersects this quadtree.
    May also return true if the circle does not intersect this quadtree, we in fact check if the square containing the circle intersects the quadtree.
*/
bool QuadTree::intersectsCircle(Vec2 center, float radius) const
{
    Rect rectCircle = {center.x - radius, center.y - radius, 2 * radius, 2 * radius};
    return rect.intersects(rectCircle);
    
    // this is much slower in practice than the above method
    float dist_squared = rect.distSquared(center);
    return dist_squared <= radius * radius;
}