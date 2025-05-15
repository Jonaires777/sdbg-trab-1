#ifndef WRITE_FILE_H 
#define WRITE_FILE_H

#include "bptree/bptree.h"
#include <stdio.h>

Record read_record_buffer(char* buffer);
void create_records_file();
Record get_record_from_file(FILE* file, int index);

#endif