import ctypes

dll = ctypes.cdll.LoadLibrary("build/quadtree.dll")
dll.createVec2.argtypes = [ctypes.c_float, ctypes.c_float]
dll.createVec2.restype = ctypes.c_void_p

dll.destroyVec2.argtypes = [ctypes.c_void_p]
dll.destroyVec2.restype = None

dll.printVec2.argtypes = [ctypes.c_void_p]
dll.printVec2.restype = None

vec = dll.createVec2(4, 7)
dll.printVec2(vec)
dll.destroyVec2(vec)