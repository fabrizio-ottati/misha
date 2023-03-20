import ctypes
from numpy.ctypeslib import ndpointer
from pathlib import Path
import re
import os

MishaAddr_t = ctypes.c_int16
MishaPol_t = ctypes.c_uint8
MishaTime_t = ctypes.c_int64

class MishaEvent(ctypes.Structure):
    _fields_ = [
        ("x", MishaAddr_t), 
        ("y", MishaAddr_t), 
        ("p", MishaPol_t), 
        ("t", MishaTime_t), 
    ]

MishaStatus_t = ctypes.c_int

class MishaCommonInfo(ctypes.Structure):
    _fields_ = [
        ("status", MishaStatus_t), 
        ("dim", ctypes.c_size_t), 
        ("isChunk", ctypes.c_bool), 
        ("timeWindow", ctypes.c_size_t), 
        ("isTimeWindow", ctypes.c_bool), 
        ("startByte", ctypes.c_size_t), 
        ("finished", ctypes.c_bool),
    ]

class MishaDATInfo(ctypes.Structure):
    _fields_ = [
       ("common", MishaCommonInfo), 
       ("last_t", ctypes.c_int64), 
       ("timeOvfs", ctypes.c_int64), 
   ]

MishaStatusDecode = (
    "MISHA_OK"
    "MISHA_FILE_ERROR"
    "MISHA_FSEEK_ERROR"
    "MISHA_UNKNOWN_EVENT"
    "MISHA_ALLOCATION_ERROR"
    "MISHA_UNKNOWN_ENCODING"
    "MISHA_UNEXPECTED_ERROR"
)

ROOT_PATH = Path(__file__).resolve().parent.parent
lib_re = r"^clib-misha\..*\.(so|pyd)$"
for root, _, files in os.walk(ROOT_PATH):
    for f in files:
        if re.match(lib_re, f):
            lib_path = Path(root, f); 
            break

clib = ctypes.CDLL(str(lib_path))

count_events_dat = clib.count_events_dat
count_events_dat.restype = MishaStatus_t
count_events_dat.argtypes = [
    ctypes.c_char_p, # fpath
    ctypes.POINTER(MishaDATInfo), # info
]

read_events_dat = clib.read_events_dat
read_events_dat.restype = MishaStatus_t
read_events_dat.argtypes = [
    ctypes.c_char_p, # fpath
    ctypes.POINTER(MishaDATInfo), # info
    ndpointer(ndim=1), # arr
]

reset_common_info_dat = clib.reset_common_info_dat
reset_common_info_dat.res = MishaStatus_t
read_events_dat.argtype = [
    ctypes.POINTER(MishaDATInfo),
]
