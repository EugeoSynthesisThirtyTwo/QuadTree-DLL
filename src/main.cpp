#define DLL_EXPORT __declspec(dllexport)
#include "quadtree.hpp"

struct CArrayVec2
{
    Vec2* array;
    size_t size;
};

struct OptionalVec2
{
    bool has_value;
    Vec2 value;
};

extern "C"
{
    DLL_EXPORT CArrayVec2 vectorVec2ToArray(void* vector)
    {
        BetterVector<Vec2>* vec = static_cast<BetterVector<Vec2>*>(vector);
        return CArrayVec2
        {
            vec->data(),
            vec->size()
        };
    }

    DLL_EXPORT void freeVectorVec2(void* vector)
    {
        BetterVector<Vec2>* vec = static_cast<BetterVector<Vec2>*>(vector);
        delete vec;
    }
    
    DLL_EXPORT void* QTcreate(Rect& rect, int capacity)
    {
        return new QuadTree(rect, capacity);
    }

    DLL_EXPORT void QTfree(void* quadTree)
    {
        QuadTree* qt = static_cast<QuadTree*>(quadTree);
        delete qt;
    }

    DLL_EXPORT int QTdepth(const void* quadTree)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        return qt->depth();
    }

    DLL_EXPORT int QTtotalPoints(const void* quadTree)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        return qt->totalPoints();
    }

    DLL_EXPORT bool QTcontains(const void* quadTree, Vec2 point)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        return qt->contains(point);
    }

    DLL_EXPORT bool QTintersects(const void* quadTree, Rect rect)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        return qt->intersects(rect);
    }
    
    DLL_EXPORT void QTmerge(void* quadTree)
    {
        QuadTree* qt = static_cast<QuadTree*>(quadTree);
        qt->merge();
    }

    DLL_EXPORT void QTsubdivide(void* quadTree)
    {
        QuadTree* qt = static_cast<QuadTree*>(quadTree);
        qt->subdivide();
    }

    DLL_EXPORT bool QTinsert(void* quadTree, Vec2 point)
    {
        QuadTree* qt = static_cast<QuadTree*>(quadTree);
        return qt->insert(point);
    }

    DLL_EXPORT void QTsuppress(void* quadTree, Vec2 point)
    {
        QuadTree* qt = static_cast<QuadTree*>(quadTree);
        qt->suppress(point);
    }
    
    DLL_EXPORT OptionalVec2 QTnaiveClosest(const void* quadTree, Vec2 point, float radius)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        std::optional<Vec2> closest = qt->naiveClosest(point, radius);
        OptionalVec2 result = {closest.has_value(), Vec2()};

        if (result.has_value)
            result.value = closest.value();
        
        return result;
    }
    
    DLL_EXPORT OptionalVec2 QTclosest(void* quadTree, Vec2 point)
    {
        QuadTree* qt = static_cast<QuadTree*>(quadTree);
        std::optional<Vec2> closest = qt->closest(point);
        OptionalVec2 result = {closest.has_value(), Vec2()};

        if (result.has_value)
            result.value = closest.value();
        
        return result;
    }

    DLL_EXPORT OptionalVec2 QTclosestDepth(const void* quadTree, Vec2 point, bool excludePoint)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        std::optional<Vec2> closest = qt->closestDepth(point, excludePoint);
        OptionalVec2 result = {closest.has_value(), Vec2()};

        if (result.has_value)
            result.value = closest.value();
        
        return result;
    }

    DLL_EXPORT void* QTqueryRect(void* quadTree, Rect rect)
    {
        QuadTree* qt = static_cast<QuadTree*>(quadTree);
        BetterVector<Vec2>* buffer = new BetterVector<Vec2>();
        qt->queryRect(rect, *buffer);
        return buffer;
    }

    DLL_EXPORT void* QTqueryCircle(const void* quadTree, Vec2 center, float radius)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        BetterVector<Vec2>* buffer = new BetterVector<Vec2>();
        qt->queryCircle(center, radius, *buffer);
        return buffer;
    }
    
    DLL_EXPORT bool QTintersectsCircle(const void* quadTree, Vec2 center, float radius)
    {
        const QuadTree* qt = static_cast<const QuadTree*>(quadTree);
        return qt->intersectsCircle(center, radius);
    }
}
