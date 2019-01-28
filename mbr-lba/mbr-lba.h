#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MBR_LEN 512

int writeMbr(const char* bin, int mbrAddr, FILE* desFd);