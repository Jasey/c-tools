#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MBR_LEN 512

int main(int argc, char const *argv[])
{
    int startSeek = 0;
    if (argv[1] == NULL)
    {
        printf("please enter binary file!\n");
        return 0;
    }

    if (argv[2] == NULL)
    {
        printf("please enter vhd file!\n");
        return 0;
    }

    if (argv[3] == NULL)
    {
        printf("--> not set mbr logic addr, default write to lba 0\n");
    }else
    {
        startSeek = atoi(argv[3]);      
    }

    const char* asmFileName = argv[1];

    const char* diskFileName = argv[2];

    FILE* fd = fopen(asmFileName, "rb");
    if (fd == NULL)
    {
        printf("open file \"%s\" failed!\n", asmFileName);
        exit(1);
    }
    char binArray[MBR_LEN];

    fread(binArray, sizeof(char), MBR_LEN, fd);

    fclose(fd);

    FILE* desFd = fopen(diskFileName, "rb+");
    if (desFd == NULL)
    {
        printf("open file \"%s\" failed!\n", diskFileName);
        exit(1);
    }
    if (fseek(desFd, startSeek*MBR_LEN, SEEK_SET) != 0)
    {
        printf("can't find lba addr : %d\n", startSeek);
        exit(1);
    }

    fwrite(binArray, sizeof(char), MBR_LEN, desFd);

    fclose(desFd);

    printf("write mbr lba addr : %d success!!\n", startSeek);

    return 0;

}