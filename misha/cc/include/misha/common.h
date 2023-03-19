#ifndef MISHA_COMMON_H_
#define MISHA_COMMON_H_

#include <cstdlib> 

#include <iostream>

#include "misha/types.h"

// Headers delimiters.
#define MISHA_HEADER_START 0x25
#define MISHA_HEADER_END 0x0A

#define MISHA_FILE_BUFFER_SIZE 4096

// Thank you http://wolfprojects.altervista.org/articles/dll-in-c-for-python/!
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else 
#define DLLEXPORT
#endif

template <class Info> MishaStatus_t reset_common_info(Info& info) {
  info.common.status = MISHA_OK; 
  info.common.isChunk = false; 
  info.common.timeWindow = 0; 
  info.common.isTimeWindow = false; 
  info.common.startByte = 0; 
  info.common.finished = false; 

  return MISHA_OK; 
}


template <class Info> MishaStatus_t jump_header(
  FILE* fp_in, 
  FILE* fp_out, 
  bool copy, 
  Info& info
  ) {

  std::size_t nbytes = 0; 
  char c; 
  bool headerStart; 
  do {
    headerStart = true; 
    do { 
      if (fread(&c, 1, 1, fp_in)!= 1) {
        std::cerr << "ERROR: Could not read from file." << std::endl; 
        info.common.status = MISHA_FILE_ERROR; 
        return MISHA_FILE_ERROR; 
      }
      nbytes++;

      if (headerStart && c != MISHA_HEADER_START){
        // Coming back one byte.
        if (fseeko(fp_in, -1, SEEK_CUR) != 0) {
          std::cerr << "ERROR: Could not perform fseeko()." << std::endl; 
          info.common.status = MISHA_FSEEK_ERROR; 
          return MISHA_FSEEK_ERROR; 
        }
        info.common.startByte = --nbytes; 
        info.common.status = MISHA_OK; 
        return MISHA_OK; 
      } else {
        headerStart = false; 
      }

      if (copy) {
        if (fwrite(&c, 1, 1, fp_out) != 1) {
          std::cerr << "ERROR: Could not write to file." << std::endl; 
          info.common.status = MISHA_FILE_ERROR; 
          return MISHA_FILE_ERROR; 
        }
      }
    } while (c != MISHA_HEADER_END); 
  } while (1); 

  return MISHA_UNEXPECTED_ERROR; 
}


template <class Info> MishaStatus_t count_events(
  const char* fpath, 
  Info& info, 
  MishaStatus_t (*count_fn)(FILE*, Info&) 
  ){

  MishaStatus_t status; 

	FILE* fp = fopen(fpath, "rb"); 
  if (fp == NULL) {
    std::cerr << "ERROR: The file could not be opened." << std::endl; 
    info.common.status = MISHA_FILE_ERROR; 
    return MISHA_FILE_ERROR; 
  }
	
	// Jumping over the headers.
	if (info.common.startByte == 0){
    status = jump_header<Info>(fp, nullptr, false, info); 
    if (status != MISHA_OK) {
      std::cerr << "ERROR: While processing file header." << std::endl; 
      info.common.status = status; 
      fclose(fp); 
      return status; 
    }
	} else {
		if (fseeko(fp, (long) info.common.startByte, SEEK_CUR) != 0) {
      std::cerr << "ERROR: Could not perform fseeko." << std::endl; 
      info.common.status = MISHA_FILE_ERROR; 
      fclose(fp); 
      return status; 
    }
	}

  status = count_fn(fp, info); 
  
  if (status != MISHA_OK) {
    std::cerr << "ERROR: Could not process the file." << std::endl; 
    info.common.status = status; 
    fclose(fp); 
    return status; 
  }

	fclose(fp); 
	return MISHA_OK;
}

template <class Info> MishaStatus_t read_events(
  const char* fpath, 
  Info& info, 
  MishaEvent* arr, 
  MishaStatus_t (*read_fn)(FILE*, Info&, MishaEvent*)
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
    status = jump_header<Info>(fp, nullptr, false, info); 
    if (status != MISHA_OK) {
      std::cerr << "ERROR: While processing file header." << std::endl; 
      info.common.status = status; 
      fclose(fp); 
      return status; 
    }
	} else {
		if (fseeko(fp, (long) info.common.startByte, SEEK_CUR) != 0) {
      std::cerr << "ERROR: Could not perform fseeko." << std::endl; 
      info.common.status = MISHA_FILE_ERROR; 
      fclose(fp); 
      return status; 
    }
	}

  status = read_fn(fp, info, arr); 
  
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