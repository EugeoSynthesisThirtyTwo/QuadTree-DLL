#define DLL_EXPORT __declspec(dllexport)
#include "vec2.hpp"

extern "C" {
    DLL_EXPORT void* createVec2(float x, float y) {
        Vec2* vec2 = new Vec2(x, y);
        return vec2;
    }

    DLL_EXPORT void destroyVec2(void* vec2) {
        delete static_cast<Vec2*>(vec2);
    }

    DLL_EXPORT void printVec2(void* vec2) {
        static_cast<Vec2*>(vec2)->print();
    }
}
