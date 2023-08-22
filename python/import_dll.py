from quadtree_cpp import QuadTreeCpp, Rect, Vec2
from quadtree import QuadTree
import ctypes
import random
from profiling import benchmark_context, benchmark_decorator

dll = ctypes.cdll.LoadLibrary("build/quadtree.dll")

rect = Rect(x=0, y=0, w=1024, h=1024)

quadtree = QuadTreeCpp(dll, rect, 4)
#quadtree = QuadTree((rect.x, rect.y), rect.h, rect.w, 4)

def rand_vec2(rect: Rect):
    return Vec2(
        x=random.random() * rect.w + rect.x,
        y=random.random() * rect.h + rect.y
    )

n_points = 1000
n_iter = 1000

with benchmark_context(f"Generation de {n_points} points en python"):
    points = [rand_vec2(rect) for i in range(n_points)]

with benchmark_context(f"Ajout des {n_points} points au quadtree"):
    for point in points:
        quadtree.insert(point)

with benchmark_context(f"{n_iter} Calculs de la profondeur"):
    for i in range(n_iter):
        quadtree.depth()

with benchmark_context(f"{n_iter} Calcul du nombre total de points"):
    for i in range(n_iter):
        quadtree.depth()

with benchmark_context(f"{n_points} Calculs du point le plus proche avec la méthode naive"):
    for point in points:
        quadtree.naive_closest(point, radius=6)

with benchmark_context(f"{n_points} Calculs du point le plus proche avec la méthode en largeur"):
    for point in points:
        breadth_closest = quadtree.closest(point)

with benchmark_context(f"{n_points} Calculs du point le plus proche avec la méthode en profondeur"):
    for point in points:
        depth_closest = quadtree.closest_depth(point)