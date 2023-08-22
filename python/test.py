import ctypes

class Vec2(ctypes.Structure):
    _fields_ = [
        ('x', ctypes.c_float),
        ('y', ctypes.c_float)
    ]

    def __str__(self):
        return f"Vec2({self.x}, {self.y})"

dll = ctypes.cdll.LoadLibrary("build/quadtree.dll")

vec = Vec2(0.4, 0.6)
dll.test.argtypes = [Vec2]
dll.test.restype = None
dll.test(vec)