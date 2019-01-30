#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MBR_LEN 512
#define MAX_LBR_ADDR_LEN 29

int writeMbr(const char* bin, int mbrAddr, FILE* desFd);