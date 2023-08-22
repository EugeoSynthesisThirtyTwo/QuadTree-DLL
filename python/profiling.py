import time

class benchmark_context:
    def __init__(self, title: str):
        self.title = title

    def __enter__(self):
        self.t1 = time.time()
    
    def __exit__(self, exc_type, exc_value, exc_traceback):
        t2 = time.time()
        print(f"{self.title} : {t2 - self.t1} s")

def benchmark_decorator(function):
    def wrapper(*args, **kw_args):
        t1 = time.time()
        ret_value = function(*args, **kw_args)
        t2 = time.time()
        print(f"{function.__name__} : {t2 - t1} s")
        return ret_value
    
    return wrapper
