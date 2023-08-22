#pragma once

#include "vec2.hpp"
#include "rect.hpp"

/*
    TODO remove empty
    TODO remove representative
    TODO dans ma méthode du closest, chercher récursivement en priorité dans les sous régions les plus proche du point
    TODO mettre des BetterVector
    TODO essayer la disjonction de grégoire
*/
class QuadTree
{
private:
    static const int MAX_CAPACITY;
    static float OBJECT_RADIUS;

    const Rect rect;
    const int capacity;
    bool divided;

    std::vector<Vec2> points;
    std::optional<Vec2> representative;

    bool empty;
    std::vector<QuadTree*> childs;

public:
    QuadTree(Rect rect, int capacity = QuadTree::MAX_CAPACITY);
    
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
        Return true if the given rectangle intersects this quadtree.
    */
    bool intersects(Rect rect) const;

    /*
        Try to merge the children of this quadtree.
    */
    void merge();

    /*
        Subdivide this quadtree into four subnodes.
    */
    void subdivide();

    /*
        Add a point to the quadtree.
    */
    bool insert(Vec2 point);

    /*
        TODO : This method doesn't work !!!
        We often want to supress points that moved outside of their previous square.
    */
    void suppress(Vec2 point);

    /*
        This has a worst case complexity of O(n*log(n)) for one query. It is worse thant the naive method of going through the list of all points.
        However, the worst case complexity assumes that all points are within the same radius, so all points are mutually in collision.
        Thus, in a setting with collisions, it is extremely unlikely that more than 6 points are close enough, and in practice, this
        method is faster than the mathematically optimal method below.
    */
    std::optional<Vec2> naiveClosest(Vec2 point, float radius = QuadTree::OBJECT_RADIUS + 1) const;
    
    /*
        Return the closest point to the given point.
        Algorithm :
            Maintain a list of interesting squares
            sqrs_0 is the root of the quadtree
            sqrs_{i+1} are the squares whose distance to the point is less than the current minimum distance among the children of sqrs_i.
            The current minimum distance is the distance between the point and the best representative yet.
            Returning the best representative gives the closest point.
    */
    std::optional<Vec2> closest(Vec2 point);

private:
    /*
        Return the closest distance to the given point.
        Depth search algorithm.
    */
    float closestDepth(Vec2 point, std::optional<Vec2>& bestPoint, bool notEquals, float bestDist = -1) const;

public:
    /*
        Return the closest point to the given point.
        Depth search algorithm.
    */
    std::optional<Vec2> closestDepth(Vec2 point, bool notEquals = false) const;

    /*
        Append in buffer all points in the given rectangle.
    */
    void queryRect(Rect rect, std::vector<Vec2>& buffer) const;

    /*
        Append in buffer all points in the given circle.
    */
    void queryCircle(Vec2 center, float radius, std::vector<Vec2>& buffer) const;
    
    /*
        Return true if the given circle intersects this quadtree.
        May also return true if the circle does not intersect this quadtree, we in fact check if the square containing the circle intersects the quadtree.
    */
    bool intersectsCircle(Vec2 center, float radius) const;
};
