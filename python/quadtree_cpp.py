import ctypes

class Vec2(ctypes.Structure):
    _fields_ = [
        ('x', ctypes.c_float),
        ('y', ctypes.c_float)
    ]

    def __str__(self):
        return f"Vec2({self.x}, {self.y})"

class Rect(ctypes.Structure):
    _fields_ = [
        ('x', ctypes.c_float),
        ('y', ctypes.c_float),
        ('w', ctypes.c_float),
        ('h', ctypes.c_float),
    ]

    def __str__(self):
        return f"Rect({self.x}, {self.y}, {self.w}, {self.h})"

class CArrayVec2(ctypes.Structure):
    _fields_ = [
        ('array', ctypes.POINTER(Vec2)),
        ('size', ctypes.c_size_t)
    ]

    def __str__(self):
        return f"CArrayVec2({self.array}, {self.size})"

class OptionalVec2(ctypes.Structure):
    _fields_ = [
        ('has_value', ctypes.c_bool),
        ('value', Vec2)
    ]

    def __str__(self):
        if not self.has_value:
            return "OptionalVec2({})"
        
        return f"OptionalVec2({self.value})"

class QuadTreeCpp:
    def __init__(self, dll: ctypes.CDLL, rect: Rect, capacity: int = 4):
        self._dll = dll
        self._init_dll()
        self._quadTree = self._dll.QTcreate(rect, capacity)

    def _init_dll(self) -> None:
        dll = self._dll

        dll.vectorVec2ToArray.argtypes = [ctypes.c_void_p]
        dll.vectorVec2ToArray.restype = CArrayVec2

        dll.freeVectorVec2.argtypes = [ctypes.c_void_p]
        dll.freeVectorVec2.restype = None
    
        dll.QTcreate.argtypes = [Rect, ctypes.c_int]
        dll.QTcreate.restype = ctypes.c_void_p

        dll.QTfree.argtypes = [ctypes.c_void_p]
        dll.QTfree.restype = None

        dll.QTdepth.argtypes = [ctypes.c_void_p]
        dll.QTdepth.restype = ctypes.c_int

        dll.QTtotalPoints.argtypes = [ctypes.c_void_p]
        dll.QTtotalPoints.restype = ctypes.c_int

        dll.QTcontains.argtypes = [ctypes.c_void_p, Vec2]
        dll.QTcontains.restype = ctypes.c_bool
    
        dll.QTintersects.argtypes = [ctypes.c_void_p, Rect]
        dll.QTintersects.restype = ctypes.c_bool
    
        dll.QTmerge.argtypes = [ctypes.c_void_p]
        dll.QTmerge.restype = None
    
        dll.QTsubdivide.argtypes = [ctypes.c_void_p]
        dll.QTsubdivide.restype = None

        dll.QTinsert.argtypes = [ctypes.c_void_p, Vec2]
        dll.QTinsert.restype = ctypes.c_bool
    
        dll.QTsuppress.argtypes = [ctypes.c_void_p, Vec2]
        dll.QTsuppress.restype = None
    
        dll.QTnaiveClosest.argtypes = [ctypes.c_void_p, Vec2, ctypes.c_float]
        dll.QTnaiveClosest.restype = OptionalVec2
    
        dll.QTclosest.argtypes = [ctypes.c_void_p, Vec2]
        dll.QTclosest.restype = OptionalVec2
    
        dll.QTclosestDepth.argtypes = [ctypes.c_void_p, Vec2, ctypes.c_bool]
        dll.QTclosestDepth.restype = OptionalVec2

        dll.QTqueryRect.argtypes = [ctypes.c_void_p, Rect]
        dll.QTqueryRect.restype = ctypes.c_void_p
    
        dll.QTqueryCircle.argtypes = [ctypes.c_void_p, Vec2, ctypes.c_float]
        dll.QTqueryCircle.restype = ctypes.c_void_p

        dll.QTintersectsCircle.argtypes = [ctypes.c_void_p, Vec2, ctypes.c_float]
        dll.QTintersectsCircle.restype = ctypes.c_bool
    
    def __del__(self) -> None:
        self._dll.QTfree(self._quadTree)
    
    def depth(self) -> int:
        return self._dll.QTdepth(self._quadTree)
    
    def total_points(self) -> int:
        return self._dll.QTtotalPoints(self._quadTree)
    
    def contains(self, point: Vec2) -> bool:
        return self._dll.QTcontains(self._quadTree, point)
    
    def intersects(self, rect: Rect) -> bool:
        return self._dll.QTintersects(self._quadTree, rect)
    
    def merge(self) -> None:
        self._dll.QTmerge(self._quadTree)
    
    def subdivide(self) -> None:
        self._dll.QTsubdivide(self._quadTree)
    
    def insert(self, point: Vec2) -> bool:
        return self._dll.QTinsert(self._quadTree, point)
    
    def suppress(self, point: Vec2) -> None:
        self._dll.QTsuppress(self._quadTree, point)
    
    def naive_closest(self, point: Vec2, radius: float) -> Vec2 | None:
        optional = self._dll.QTnaiveClosest(self._quadTree, point, radius)

        if optional.has_value:
            return optional.value
        
        return None
    
    def closest(self, point: Vec2) -> Vec2 | None:
        optional = self._dll.QTclosest(self._quadTree, point)

        if optional.has_value:
            return optional.value
        
        return None
    
    def closest_depth(self, point: Vec2, exclude_point: bool = True) -> Vec2 | None:
        optional = self._dll.QTclosestDepth(self._quadTree, point, exclude_point)

        if optional.has_value:
            return optional.value
        
        return None
    
    def query_rect(self, rect: Rect) -> list[Vec2]:
        vector = self._dll.QTqueryRect(self._quadTree, rect)
        c_array = self._dll.vectorVec2ToArray(self, vector)
        python_list = [c_array.array[i] for i in range(c_array.size)]
        self._dll.freeVectorVec2(vector)
        return python_list
    
    def query_circle(self, center: Vec2, radius: float) -> list[Vec2]:
        vector = self._dll.QTqueryCircle(self._quadTree, center, radius)
        c_array = self._dll.vectorVec2ToArray(self, vector)
        python_list = [c_array[i] for i in range(c_array.size)]
        self._dll.freeVectorVec2(vector)
        return python_list
    
    def intersects_circle(self, center: Vec2, radius: float) -> bool:
        return self._dll.QTintersectsCircle(self._quadTree, center, radius)