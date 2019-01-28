### sharding
> 分表计算工具，可以输入指定的分表基础名和分表下标范围，根据分表主键计算分表的位置

### mbr-lba
> 磁盘扇区填充程序，按照512字节数据填充扇区，不足512字节的部分会填充0，需要指定需要填充的二进制文件和被填充的磁盘文件

```shell
    #使用方法
    $ gcc mbr-lba.c -o mbr-lba
    $ ./mbr-lba ${binFilePath} ${vhdFormatDiskFilePath} [${lbaAddr}]

    # ${binFilePath} 编译好的二进制文件路径
    # ${vhdFormatDiskFilePath} vhd格式的磁盘文件路径
    # ${lbaAddr} 为可选的，如果不输入，则默认写入主引导扇区（lba addr = 0）
```