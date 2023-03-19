#ifndef MISHA_COMMON_H_
#define MISHA_COMMON_H_

#include <stdio.h> 

#include "misha/types.h"

// Headers delimiters.
#define MISHA_HEADER_START 0x25
#define MISHA_HEADER_END 0x0A

// Thank you http://wolfprojects.altervista.org/articles/dll-in-c-for-python/!
#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else 
#define DLLEXPORT
#endif

MishaStatus_t jump_header(
  FILE* fp_in, 
  FILE* fp_out, 
  MishaBool_t copy_file, 
  void* info, 
  MishaEncoding_t encoding
  ); 

DLLEXPORT void count_events(
  const char* fpath, 
  void* info, 
  MishaEncoding_t encoding, 
  MishaStatus_t (*count_fn)(FILE*, void*) 
  ); 

#endif // MISHA_COMMON_H_
