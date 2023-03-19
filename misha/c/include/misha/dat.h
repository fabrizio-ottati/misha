#ifndef MISHA_DAT_H_
#define MISHA_DAT_H_

// Library to handle DAT encoded binary files.

#include <stdlib.h>

#include "misha/common.h"
#include "misha/types.h"

// DAT format constants.
#define MISHA_DAT_EVENT_2D 0x00
#define MISHA_DAT_EVENT_CD 0x0C
#define MISHA_DAT_EVENT_EXT_TRIGGER 0x0E

/** Wrap structure for the cargo information about the events tuned for the 
 *  DAT encoding format.
 *      
 *  @field  events_info Information about the event stream. See "events.h".
 *  @field  last_t      Last timestamp read.
 *  @field  time_ovfs   Number of overflows in the timestamps occured in the
 *                      event stream, for recordings with t >= 2^32.
 */
typedef struct {
	MishaCommonInfo_t common; 
	MishaTs_t last_t; 
	MishaTs_t time_ovfs; 
} MishaDATInfo_t; 

typedef uint64_t MishaDATPkt_t; 

void count_events_dat(FILE* fp, void* info); 
DLLEXPORT void count_events_dat_wrap(const char* fpath, MishaDATInfo_t* info); 

#endif // MISHA_DAT_H_
