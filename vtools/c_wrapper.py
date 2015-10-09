#==== imports =================================================================
#
import ctypes
import numpy

import config
#
#______________________________________________________________________________

#==== config ==================================================================
#
sflat   = (config.height * config.width * 3)
sdef    = (config.height, config.width, 3)
#
#______________________________________________________________________________

#==== wrapper =================================================================
#
# load module
module = ctypes.cdll.LoadLibrary(config.module_name)

# load function
impl = module.rotate

# result and args
impl.restype = numpy.ctypeslib.ndpointer(ctypes.c_uint32, shape=sflat)
impl.argtypes = [ctypes.c_float, ctypes.c_float, ctypes.c_float,
    numpy.ctypeslib.ndpointer(ctypes.c_uint32, flags="C_CONTIGUOUS"),
    numpy.ctypeslib.ndpointer(ctypes.c_uint32, flags="C_CONTIGUOUS")]
#
#______________________________________________________________________________

#==== main ====================================================================
#
def rotate(roll, pitch, yaw, in_img):
    in_img  = numpy.ascontiguousarray(numpy.ravel(in_img),              dtype=numpy.uint32)
    out_img = numpy.ascontiguousarray(numpy.empty(sflat, order='C'),    dtype=numpy.uint32)
    out_img = impl(roll, pitch, yaw, in_img, out_img)
    return numpy.reshape(out_img, sdef)
#
#______________________________________________________________________________
