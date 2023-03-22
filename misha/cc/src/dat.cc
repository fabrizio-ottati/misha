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
    return count_events<MishaDATInfo, count_fn_dat>(fpath, info); 
  }

  DLLEXPORT MishaStatus_t read_events_dat(
    const char* fpath, MishaDATInfo& info, MishaEvent* arr
    ) {
    return read_events<MishaDATInfo, read_fn_dat>(fpath, info, arr); 
  }

  DLLEXPORT MishaStatus_t reset_common_info_dat(MishaDATInfo& info) {
    return reset_common_info<MishaDATInfo>(info); 
  }

} // extern "C"

/**
 *  Count events function.
 */
MishaStatus_t count_fn_dat(FILE* fp, MishaDATInfo& info) {
  
  MishaDATPkt_t buff[MISHA_FILE_BUFFER_SIZE]; 
  std::size_t n = 0; 
  info.common.dim = 0; 
  // Jumping two bytes.
  if (fseeko(fp, 2, SEEK_CUR) != 0) {
    std::cerr << "ERROR: Could not perform fseeko on file." << std::endl; 
    info.common.status = MISHA_FILE_ERROR; 
    return MISHA_FILE_ERROR;
  }

  while (
    (n = fread(buff, sizeof(MishaDATPkt_t), MISHA_FILE_BUFFER_SIZE, fp)) > 0
    ) {
    info.common.dim += n; 
  }
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

