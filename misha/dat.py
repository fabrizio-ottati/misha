from misha.clib_dat import (
    read_events_dat, 
    count_events_dat, 
    reset_common_info_dat, 
    MishaDATInfo,
    MishaCommonInfo,
    MishaEvent,
    )

from numpy import empty

class DAT: 
    def __init__(self):
        self._info = MishaDATInfo(MishaCommonInfo())

    def read(self, fpath):
        _fpath = bytes(str(fpath), "utf-8")
        status = count_events_dat(_fpath, self._info)
        arr = empty((self._info.common.dim,), dtype=MishaEvent)
        read_events_dat(_fpath, self._info, arr)
        return arr

    def reset(self):
        reset_common_info_dat(self._info)
        return
