
import numpy as np
import ctypes as ct
from enum import Enum, unique

_libcilt = np.ctypeslib.load_library('libcilt', '.')

_libcilt.cilt_filt_del.argtypes = [ct.c_void_p]
_libcilt.cilt_filt_del.restype = None

_libcilt.cilt_filt_getNumerator.argtypes = [ct.c_void_p,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filt_getNumerator.restype = None

_libcilt.cilt_filt_setNumerator.argtypes = [ct.c_void_p,
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filt_setNumerator.restype = ct.c_int

_libcilt.cilt_filt_tick.argtypes = [ct.c_void_p,
        ct.c_float]
_libcilt.cilt_filt_tick.restype = ct.c_float

_libcilt.cilt_filt_lfilter.argtypes = [ct.c_void_p,
        np.ctypeslib.ndpointer(dtype=np.float32),
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filt_lfilter.restype = None


_libcilt.cilt_filtTrv_new.argtypes = None
_libcilt.cilt_filtTrv_new.restype = ct.c_void_p

_libcilt.cilt_filtTrv_setCoeffs.argtypes = [ct.c_void_p,
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filtTrv_setCoeffs.restype = None


_libcilt.cilt_filtIIR_getDenumerator.argtypes = [ct.c_void_p,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filtIIR_getDenumerator.restype = None

_libcilt.cilt_filtIIR_setDenumerator.argtypes = [ct.c_void_p,
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filtIIR_setDenumerator.restype = ct.c_int

_libcilt.cilt_filtIIR_setCoeffs.argtypes = [ct.c_void_p,
        ct.c_size_t,
        np.ctypeslib.ndpointer(dtype=np.float32),
        np.ctypeslib.ndpointer(dtype=np.float32)]
_libcilt.cilt_filtIIR_setCoeffs.restype = ct.c_int


_libcilt.cilt_filtForm1_new.argtypes = None
_libcilt.cilt_filtForm1_new.restype = ct.c_void_p


_libcilt.cilt_filtForm2_new.argtypes = None
_libcilt.cilt_filtForm2_new.restype = ct.c_void_p


@unique
class Errno(Enum):
    none = 0
    error = 1
    ind_out_of_rng = 2
    wrong_size = 3
    diff_sizes = 4
    a_0 = 5

class CiltError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)

class WrongSizeError(CiltError):
    def __init__(self, value):
        CiltError.__init__(self, value)

class DifferentSizesError(CiltError):
    def __init__(self, value):
        CiltError.__init__(self, value)

class DenumeratorNormalizationError(CiltError):
    def __init__(self, value):
        CiltError.__init__(self, value)

class Filter(object):
    def __del__(self):
        _libcilt.cilt_filt_del(self.filter)

    def getOrder(self):
        return self.order

    def getNumerator(self):
        b = np.zeros(self.order, dtype=np.float32)
        _libcilt.cilt_filt_getNumerator(self.filter, b)
        return b

    def setNumerator(self, b):
        b = np.asarray(b, dtype=np.float32)
        errno = _libcilt.cilt_filt_setNumerator(self.filter, len(b), b)
        if Errno(errno) == Errno.wrong_size:
            raise WrongSizeError('Input array has a wrong size!')

    def tick(self, data):
        return _libcilt.cilt_filt_tick(self.filter, data)

    def lfilter(self, data):
        data = np.asarray(data, dtype=np.float32)
        ret = np.empty_like(data)
        _libcilt.cilt_filt_lfilter(self.filter, ret, len(data), data)
        return ret

class FilterTransversal(Filter):
    def __init__(self, b=None):
        self.filter = _libcilt.cilt_filtTrv_new()
        if b != None:
            self.setCoeffs(b)
        else:
            self.order = 0

    def setCoeffs(self, b):
        b = np.asarray(b, dtype=np.float32)
        self.order = len(b)
        _libcilt.cilt_filtTrv_setCoeffs(self.filter, self.order, b)

class FilterIIR(Filter):
    def setCoeffs(self, a=None, b=None):
        if len(a) != len(b):
            raise DifferentSizesError('Input arrays have different sizes!')
        a = np.asarray(a, dtype=np.float32)
        b = np.asarray(b, dtype=np.float32)
        self.order = len(a)
        errno = _libcilt.cilt_filtIIR_setCoeffs(self.filter, self.order, a, b)
        if Errno(errno) == Errno.diff_sizes:
            raise DifferentSizesError('Input arrays have different sizes!')
        elif Errno(errno) == Errno.a_0:
            raise DenumeratorNormalizationError(
                "First coefficient of denumerator polynom mustn't be 0!")

    def getDenumerator(self):
        a = np.zeros(self.order, dtype=np.float32)
        _libcilt.cilt_filtIIR_getDenumerator(self.filter, a)
        return a

    def setDenumerator(self, a):
        a = np.asarray(a, dtype=np.float32)
        errno = _libcilt.cilt_filtIIR_setDenumerator(self.filter, len(a), a)
        if Errno(errno) == Errno.wrong_size:
            raise WrongSizeError('Input array has a wrong size!')

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
