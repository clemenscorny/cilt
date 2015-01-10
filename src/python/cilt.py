
import numpy as np
import ctypes as ct
from abc import ABCMeta, abstractmethod

_libcilt = np.ctypeslib.load_library('libcilt', '.')

_libcilt.cilt_filt_del.argtypes = [ct.c_void_p]
_libcilt.cilt_filt_del.restype = None

_libcilt.cilt_filt_getNumerator.argtypes = [ct.c_void_p,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filt_getNumerator.restype = None

_libcilt.cilt_filt_setNumerator.argtypes = [ct.c_void_p,
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filt_setNumerator.restype = None


_libcilt.cilt_filtTrv_new.argtypes = None
_libcilt.cilt_filtTrv_new.restype = ct.c_void_p

_libcilt.cilt_filtTrv_tick.argtypes = [ct.c_void_p,
        ct.c_float]
_libcilt.cilt_filtTrv_tick.restype = ct.c_float

class Filter(object):
    __metaclass__ = ABCMeta

    def getOrder(self):
        return self.order

    def getNumerator(self):
        b = np.zeros(self.order, dtype=np.float32)
        _libcilt.cilt_filt_getNumerator(self.filter, b)
        return b

    def setNumerator(self, b):
        if len(b) != self.order:
            self.order = len(b)
        b = np.asarray(b, dtype=np.float32)
        _libcilt.cilt_filt_setNumerator(self.filter, self.order, b)

    @abstractmethod
    def tick(self, data):
        pass

class FilterIIR(Filter):
    def __init__(self, a=None, b=None):
        super(FilterIIR, self).__init__()
        if a != None and b != None:
            if len(a) == len(b):
                self.order = len(a)
                self.setNumerator(b)
                self.setDenumerator(a)

    def getDenumerator(self):
        pass

    def setDenumerator(self, a):
        if len(a) != self.order:
            return
        pass

    def setNumerator(self, b):
        if len(b) != self.order:
            return
        b = np.asarray(b, dtype=np.float32)
        _libcilt.cilt_filt_setNumerator(self.filter, b)

class FilterTransversal(Filter):
    def __init__(self, b=None):
        self.filter = _libcilt.cilt_filtTrv_new()
        if b != None:
            self.order = len(b)
            self.setNumerator(b)
        else:
            self.order = 0

    def __del__(self):
        _libcilt.cilt_filt_del(self.filter)

    def tick(self, data):
        return _libcilt.cilt_filtTrv_tick(self.filter, data)
