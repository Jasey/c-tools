#include "mbr-lba.h"

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

    char ans = 'Y';
    char addr[MAX_LBR_ADDR_LEN];
    if (argv[3] == NULL)
    {
        printf("default write to lba 0 ? (Y/N) ");
        scanf("%c", &ans);
        if (!(ans == 'Y' || ans == 'y' || ans == '\n'))
        {
            printf("lba addr > ");
            scanf("%s", addr);
            startSeek = atoi(addr); 
        }
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

    FILE* desFd = fopen(diskFileName, "rb+");

    if (desFd == NULL)
    {
        printf("open file \"%s\" failed!\n", diskFileName);
        exit(1);
    }

    char binArray[MBR_LEN];
    memset(binArray, 0, MBR_LEN);

    int mbrAddr = startSeek;
    int total = 0;

    while (fread(binArray, sizeof(char), MBR_LEN, fd) > 0)
    {
        if (writeMbr(binArray, mbrAddr, desFd) == -1)
        {
            printf("wirte mbr addr %d error\n", mbrAddr);
            exit(1);
        }
        total++;
        mbrAddr++;
        memset(binArray, 0, MBR_LEN);

    }

    fclose(desFd);

    fclose(fd);

     printf("write mbr lba start addr : %d , total mbr num : %d success!!\n", startSeek, total);

    return 0;

}

int writeMbr(const char* bin, int mbrAddr, FILE* desFd)
{
    // seek addr
    if (fseek(desFd, mbrAddr*MBR_LEN, SEEK_SET) != 0)
    {
        printf("can't find lba addr : %d\n", mbrAddr);
        return -1;
    }

    fwrite(bin, sizeof(char), MBR_LEN, desFd);

    printf("write to mbr lba addr : %d\n", mbrAddr);

    return 1;
}