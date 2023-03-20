#ifndef MISHA_DAT_H_
#define MISHA_DAT_H_

// Library to handle DAT encoded binary files.

#include <cstdint>
#include <cstdlib>

#include "misha/common.h"
#include "misha/types.h"

// DAT format constants.
#define MISHA_DAT_EVENT_2D 0x00
#define MISHA_DAT_EVENT_CD 0x0C
#define MISHA_DAT_EVENT_EXT_TRIGGER 0x0E

struct MishaDATInfo {
  MishaCommonInfo common; 
  MishaTime_t last_t; 
  MishaTime_t timeOvfs; 
}; 

typedef std::uint64_t MishaDATPkt_t; 

extern "C" {

DLLEXPORT MishaStatus_t count_events_dat(
  const char* fpath, MishaDATInfo& info
  ); 

DLLEXPORT MishaStatus_t read_events_dat(
  const char* fpath, MishaDATInfo& info, MishaEvent* arr
  ); 

DLLEXPORT MishaStatus_t reset_common_info_dat(MishaDATInfo& info); 

} // extern "C"

MishaStatus_t count_fn_dat(FILE* fp, MishaDATInfo& info); 
MishaStatus_t read_fn_dat(FILE* fp, MishaDATInfo&info, MishaEvent* arr); 

#endif // MISHA_DAT_H_
