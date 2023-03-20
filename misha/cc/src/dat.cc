#include <cstdio>
#include <cstdlib>

#include <filesystem>
#include <iostream>

#include "misha/common.h"
#include "misha/dat.h"

extern "C" {

  DLLEXPORT MishaStatus_t count_events_dat(
    const char* fpath, MishaDATInfo& info
    ) {
    return count_events<MishaDATInfo>(fpath, info, count_fn_dat); 
  }

  DLLEXPORT MishaStatus_t read_events_dat(
    const char* fpath, MishaDATInfo& info, MishaEvent* arr
    ) {
    return read_events<MishaDATInfo>(fpath, info, arr, read_fn_dat); 
  }

  DLLEXPORT MishaStatus_t reset_common_info_dat(MishaDATInfo& info) {
    return reset_common_info<MishaDATInfo>(info); 
  }

} // extern "C"

/**
 *  Count events function.
 */
MishaStatus_t count_fn_dat(FILE* fp, MishaDATInfo& info) {
  // Reaching end of file to count number of bytes.
  if (fseeko(fp, 0, SEEK_END) != 0) {
    std::cerr << "ERROR: Could not reach end of file through fseeko()." << 
      std::endl; 
    info.common.status = MISHA_FILE_ERROR; 
    return MISHA_FILE_ERROR; 
  }
  
  // Getting number of events as (file_size - header_size)/bytes_per_event.
  info.common.dim = 
    (ftello(fp) - info.common.startByte - 2) / sizeof(MishaDATPkt_t); 
  return MISHA_OK; 
}

/**
 *  Read function.
 */
MishaStatus_t read_fn_dat(FILE* fp, MishaDATInfo&info, MishaEvent* arr) {
  // Jumping two bytes.
  if (fseeko(fp, 2, SEEK_CUR) != 0) {
    std::cerr << "ERROR: Could not perform fseeko on file." << std::endl; 
    info.common.status = MISHA_FILE_ERROR; 
    return MISHA_FILE_ERROR;
  }

  MishaDATPkt_t buff[MISHA_FILE_BUFFER_SIZE]; 

  std::size_t i=0, n=0, numRead=0; 
  MishaDATPkt_t upper; 
  while (
    n < info.common.dim && 
      (numRead = fread(buff, sizeof(*buff), MISHA_FILE_BUFFER_SIZE, fp)) > 0
    ) {
    for (i=0; n < info.common.dim && i < numRead; i++) {
      arr[n].t = buff[i] & 0xFFFFFFFF; 
      upper = buff[i] >> 32; 
      arr[n].x = (upper & 0x3FFF); 
      arr[n].y = ((upper >> 14) & 0x3FFF); 
      arr[n++].p = (upper >> 28);
    }
    info.common.startByte += i*sizeof(*buff); 
  }

  info.common.dim = n; 
  if (numRead == 0) {
    info.common.finished = true; 
  }
  info.common.status = MISHA_OK;

  return MISHA_OK;
}

