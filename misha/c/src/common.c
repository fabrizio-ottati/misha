#include <stdio.h>
#include <stdlib.h>

#include "misha/common.h"
#include "misha/types.h"

MishaStatus_t jump_header(
  FILE* fp_in, 
  FILE* fp_out, 
  MishaBool_t copy_file, 
  void* info, 
  MishaEncoding_t encoding
  ) {

  if (encoding == DAT) {
    info = (DATInfo_t*) info; 
  } else if (encoding == EVT2) {
    info = (EVT2Info*) info; 
  } else if (encoding == EVT3) {
    info = (EVT3Info*) info; 
  } else {
    fprintf(stderr, "ERROR: Encoding type not recognised.\n"); 
    return MISHA_UNKOWN_ENCODING; 
  }

  size_t nbytes = 0; 
  char c; 
  MishaBool_t header_begins; 
  do {
    header_begins = MISHA_TRUE; 
    do { 
      if (fread(&c, 1, 1, fp_in)!= 1) {
        fprintf(stderr, "ERROR: Could not read from file."); 
        return MISHA_FILE_ERROR; 
      }
      nbytes++;

      if (header_begins && c != MISHA_HEADER_START){
        // Coming back one byte.
        if (fseeko(fp_in, -1, SEEK_CUR) != 0) {
          fprintf(stderr, "ERROR: Could not perform fseeko.\n"); 
          return MISHA_FSEEK_ERROR; 
        }
        info->start_byte = --bytes_read; 
        return MISHA_OK; 
      } else {
        header_begins = MISHA_FALSE; 
      }

      if (copy_file == MISHA_TRUE) {
        if (fwrite(&c, 1, 1, fp_out) != 1) {
          fprintf(stderr, "ERROR: Could not write to file.\n"); 
          return MISHA_FILE_ERROR; 
        }
      }
    } while (c != MISHA_HEADER_END); 
  } while (1); 

  return MISHA_UNEXPECTED_ERROR; 
}

DLLEXPORT void count_events(
  const char* fpath, 
  void* info, 
  MishaEncoding_t encoding, 
  MishaStatus_t (*count_fn)(FILE*, void*) 
  ){

  MishaStatus_t status; 

	FILE* fp = fopen(fpath, "rb"); 
  if (fp == NULL) {
    fprintf(stderr, "ERROR: The file could not be opened.\n"); 
    info->status = MISHA_FILE_ERROR; 
    return; 
  }
	
	// Jumping over the headers.
	if (info->common.start_byte == 0){
    status = jump_header(fp, null, MISHA_FALSE, (void*)info, encoding); 
    if (status != MISHA_OK) {
      fprintf(stderr, "ERROR: While processing file header."); 
      info->status = status; 
    }
		// Jumping two bytes.
		if (fseeko(fp, 2, SEEK_CUR) != 0) {
      fprintf(stderr, "ERROR: Could not perform fseeko.\n"); 
      info->status = MISHA_FILE_ERROR; 
      fclose(fp); 
      return; 
    }
		info->events_info.start_byte += 2; 
	} else {
		if (fseeko(fp, (long) info->common.start_byte, SEEK_CUR) != 0) {
      fprintf(stderr, "ERROR: Could not perform fseeko.\n"); 
      info->status = MISHA_FILE_ERROR; 
      fclose(fp); 
      return; 
    }
	}

  status = count_fn(fp, (void*)info); 
  
  if (status != MISHA_OK) {
    fprintf(stderr, "ERROR: Could not process the file.\n"); 
    info->status = status; 
    fclose(fp); 
    return; 
  }

	fclose(fp); 
	return;
}

