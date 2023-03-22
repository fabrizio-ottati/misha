#ifndef MISHA_COMMON_H_
#define MISHA_COMMON_H_

#include <cstdlib> 

#include <iostream>

#include "misha/types.h"

// Headers delimiters.
#define MISHA_HEADER_START char(0x25)
#define MISHA_HEADER_END char(0x0A)

#define MISHA_FILE_BUFFER_SIZE 4096
#define MISHA_HEADER_BUFF_SZ 256

// Thank you http://wolfprojects.altervista.org/articles/dll-in-c-for-python/!
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else 
#define DLLEXPORT
#endif

/** Resetting the common information field of the info structure.
 *
 *  @param[out] info    The info data structure.
 *
 *  @return     status  Status flag.
 */
template <class Info> MishaStatus_t reset_common_info(Info& info) {
  info.common.status = MISHA_OK; 
  info.common.isChunk = false; 
  info.common.timeWindow = 0; 
  info.common.isTimeWindow = false; 
  info.common.startByte = 0; 
  info.common.finished = false; 

  return MISHA_OK; 
}


/** Skipping the header in Prophesee formats.
 *
 *  @param[in]  fp      Input file pointer.
 *  @param[out] info    The info data structure.
 *
 *  @return     status  The status flag.
 */
template <class Info> MishaStatus_t jump_header(
  FILE* fp, 
  Info& info
  ) {

  std::size_t nbytes = 0; 
  bool checkFirst = false; 
  char c[MISHA_HEADER_BUFF_SZ]; 
  while (1) {
    if (fread(c, 1, MISHA_HEADER_BUFF_SZ, fp) <= 0) {
      std::cerr << "ERROR: Could not read from file." << std::endl; 
      info.common.status = MISHA_FILE_ERROR; 
      return MISHA_FILE_ERROR; 
    }
    for (std::size_t i=0; i < MISHA_HEADER_BUFF_SZ; i++) {
      if (c[i] == MISHA_HEADER_END) {
        if (i+1 < MISHA_HEADER_BUFF_SZ) {
          if (c[i+1] != MISHA_HEADER_START) {
            // Reached end of header.
            if (
              fseeko(fp, -off_t(MISHA_HEADER_BUFF_SZ-(i+1)), SEEK_CUR) != 0
              ) {
              std::cerr << "ERROR: Could not perform fseeko()." << std::endl; 
              info.common.status = MISHA_FSEEK_ERROR; 
              return MISHA_FSEEK_ERROR; 
            }
            info.common.startByte = nbytes + i + 1; 
            info.common.status = MISHA_OK; 
            return MISHA_OK; 
          } 
        } else {
          // Header might not be finished, checking next bufferf of 
          // characters.
          checkFirst = true; 
        }
      } else if (checkFirst) {
        // Checking first character to see if the header has ended.
        if (c[0] != MISHA_HEADER_START) {
          if (
              fseeko(fp, -off_t(MISHA_HEADER_BUFF_SZ), SEEK_CUR) != 0
             ) {
            std::cerr << "ERROR: Could not perform fseeko()." << std::endl; 
            info.common.status = MISHA_FSEEK_ERROR; 
            return MISHA_FSEEK_ERROR; 
          }
          info.common.startByte = nbytes; 
          info.common.status = MISHA_OK; 
          return MISHA_OK; 
        } else {
          checkFirst = false; 
        }
      }
    }
    nbytes += MISHA_HEADER_BUFF_SZ; 
  }
  return MISHA_UNEXPECTED_ERROR; 
}


template <class Info>
using count_fn = MishaStatus_t(*)(FILE*, Info&); 
template <class Info, count_fn<Info> fn> MishaStatus_t count_events(
  const char* fpath, 
  Info& info 
  ){

  MishaStatus_t status; 

	FILE* fp = fopen(fpath, "rb"); 
  if (fp == NULL) {
    std::cerr << "ERROR: The file could not be opened." << std::endl; 
    info.common.status = MISHA_FILE_ERROR; 
    return MISHA_FILE_ERROR; 
  }
	
	// Jumping over the headers.
  status = jump_header<Info>(fp, info); 
  if (status != MISHA_OK) {
    std::cerr << "ERROR: While processing file header." << std::endl; 
    info.common.status = status; 
    fclose(fp); 
    return status; 
  }

  status = fn(fp, info); 
  
  if (status != MISHA_OK) {
    std::cerr << "ERROR: Could not process the file." << std::endl; 
    info.common.status = status; 
    fclose(fp); 
    return status; 
  }

	fclose(fp); 
	return MISHA_OK;
}


template <class Info>
using read_fn = MishaStatus_t(*)(FILE*, Info&, MishaEvent*); 
template <class Info, read_fn<Info> fn> MishaStatus_t read_events(
  const char* fpath, 
  Info& info, 
  MishaEvent* arr
  ) {
  
  MishaStatus_t status; 

	FILE* fp = fopen(fpath, "rb"); 
  if (fp == NULL) {
    std::cerr << "ERROR: The file could not be opened." << std::endl; 
    info.common.status = MISHA_FILE_ERROR; 
    return MISHA_FILE_ERROR; 
  }
	
	// Jumping over the headers.
	if (info.common.startByte == 0){
    status = jump_header<Info>(fp, info); 
    if (status != MISHA_OK) {
      std::cerr << "ERROR: While processing file header." << std::endl; 
      info.common.status = status; 
      fclose(fp); 
      return status; 
    }
	} else {
		if (fseeko(fp, (off_t) info.common.startByte, SEEK_CUR) != 0) {
      std::cerr << "ERROR: Could not perform fseeko." << std::endl; 
      info.common.status = MISHA_FILE_ERROR; 
      fclose(fp); 
      return status; 
    }
	}

  status = fn(fp, info, arr); 
  
  if (status != MISHA_OK) {
    std::cerr << "ERROR: Could not process the file." << std::endl; 
    info.common.status = status; 
    fclose(fp); 
    return status; 
  }

	fclose(fp); 
	return MISHA_OK;
}

#endif // MISHA_COMMON_H_
