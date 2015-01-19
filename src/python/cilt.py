
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

_libcilt.cilt_filt_tick.argtypes = [ct.c_void_p,
        ct.c_float]
_libcilt.cilt_filt_tick.restype = ct.c_float


_libcilt.cilt_filtTrv_new.argtypes = None
_libcilt.cilt_filtTrv_new.restype = ct.c_void_p

_libcilt.cilt_filtTrv_setCoeffs.argtypes = [ct.c_void_p,
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filtTrv_setCoeffs.restype = None


_libcilt.cilt_filtIIR_setCoeffs.argtypes = [ct.c_void_p,
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32),
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filtIIR_setCoeffs.restype = None


_libcilt.cilt_filtForm1_new.argtypes = None
_libcilt.cilt_filtForm1_new.restype = ct.c_void_p


_libcilt.cilt_filtForm2_new.argtypes = None
_libcilt.cilt_filtForm2_new.restype = ct.c_void_p

class Filter(object):
    __metaclass__ = ABCMeta

    def __del__(self):
        _libcilt.cilt_filt_del(self.filter)

    def getOrder(self):
        return self.order

    def getNumerator(self):
        b = np.zeros(self.order, dtype=np.float32)
        _libcilt.cilt_filt_getNumerator(self.filter, b)
        return b

    def setNumerator(self, b):
        if len(b) != self.order:
            return
        b = np.asarray(b, dtype=np.float32)
        _libcilt.cilt_filt_setNumerator(self.filter, self.order, b)

    def tick(self, data):
        return _libcilt.cilt_filt_tick(self.filter, data)

class FilterTransversal(Filter):
    def __init__(self, b=None):
        self.filter = _libcilt.cilt_filtTrv_new()
        if b != None:
            self.order = len(b)
            self.setCoeffs(b)
        else:
            self.order = 0

    def setCoeffs(self, b):
        b = np.asarray(b, dtype=np.float32)
        self.order = len(b)
        _libcilt.cilt_filtTrv_setCoeffs(self.filter, self.order, b)

class FilterIIR(Filter):
    def setCoeffs(self, a=None, b=None):
        if len(a) == len(b):
            a = np.asarray(a, dtype=np.float32)
            b = np.asarray(b, dtype=np.float32)
            self.order = len(a)
            _libcilt.cilt_filtIIR_setCoeffs(self.filter, self.order, a, b)

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

class FilterForm1(FilterIIR):
    def __init__(self, a=None, b=None):
        self.filter = _libcilt.cilt_filtForm1_new()
        if a != None and b != None:
            self.setCoeffs(a, b)

class FilterForm2(FilterIIR):
    def __init__(self, a=None, b=None):
        self.filter = _libcilt.cilt_filtForm2_new()
        if a != None and b != None:
            self.setCoeffs(a, b)
