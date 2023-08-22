from quadtree_cpp import QuadTreeCpp, Rect, Vec2
from quadtree import QuadTree
import ctypes
import random
from profiling import benchmark_context, benchmark_decorator

dll = ctypes.cdll.LoadLibrary("build/quadtree.dll")

rect = Rect(x=0, y=0, w=1, h=1)

quadtree = QuadTreeCpp(dll, rect, 4)
#quadtree = QuadTree((rect.x, rect.y), rect.h, rect.w, 4)

def rand_vec2(rect: Rect):
    return Vec2(
        x=random.random() * rect.w + rect.x,
        y=random.random() * rect.h + rect.y
    )

n_points = 1

with benchmark_context(f"Generation de {n_points} points en python"):
    points = [rand_vec2(rect) for i in range(n_points)]

with benchmark_context(f"Ajout des {n_points} points au quadtree"):
    for point in points:
        if not quadtree.insert(point):
            print(f"fail to insert {point}")

with benchmark_context(f"Calcul de la profondeur"):
    print(f"depth = {quadtree.depth()}")
