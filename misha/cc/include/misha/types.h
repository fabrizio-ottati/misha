#ifndef MISHA_TYPES_H_
#define MISHA_TYPES_H_

/** Library for events streams.
 *  Data structures used to hold information about an event stream. 
 */

#include <stdlib.h>
#include <stdint.h>

// Data types used for the event data structure fields.
typedef int16_t MishaAddr_t; 
typedef uint8_t MishaPol_t; 
typedef int64_t MishaTime_t;  

typedef enum {
  DAT,
  EVT2, 
  EVT3
} MishaEncoding_t; 

typedef enum {
  MISHA_OK=0, 
  MISHA_FILE_ERROR=1, 
  MISHA_FSEEK_ERROR=2, 
  MISHA_UNKNOWN_EVENT=3,
  MISHA_ALLOCATION_ERROR=4, 
  MISHA_UNKNOWN_ENCODING=5, 
  MISHA_UNEXPECTED_ERROR=6
} MishaStatus_t; 

/** Structure of an event in a recording.
 *
 *  @field  t Timestamp.
 *  @field  x X address of the pixel.
 *  @field  y Y address of the pixel.
 *  @field  p Polarity of the event.
 */
struct MishaEvent {
	MishaAddr_t x; 
	MishaAddr_t y; 
	MishaPol_t p; 
	MishaTime_t t; 
}; 

/** Structure that holds additional information about the event stream.
 *
 *  @field  dim             The number of events in the recording.
 *  @field  is_chunk        A flag to indicate that the file is being read in
 *                          chunks and not all at once.
 *  @field  time_window     The time window duration expressed in milliseconds
 *                          when the file is read in recordings of fixed 
 *                          duration.
 *  @field  is_time_window  Flag to indicate that the file is being read in 
 *                          time windows and not all at once to an array.
 *  @field  start_byte      Number of bytes with respect to file beginning to
 *                          be used with fseek() to reopen the file from the 
 *                          point where it was left off.
 *  @field  finished        Flag to indicate that the entire file has been read.
 */
struct MishaCommonInfo {
  MishaStatus_t status; 
	size_t dim;
	bool isChunk; 
	size_t timeWindow; 
	bool isTimeWindow; 
	size_t startByte;
	bool finished; 
}; 

#endif // MISHA_TYPES_H_
