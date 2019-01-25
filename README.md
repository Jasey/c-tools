### sharding
> 分表计算工具，可以输入指定的分表基础名和分表下标范围，根据分表主键计算分表的位置

### mbr-lba
> 磁盘主扇区填充程序，只填充主扇区512字节数据，需要指定需要填充的二进制文件和被填充的磁盘文件

```shell
    #使用方法
    $ gcc mbr-lba.c -o mbr-lba
    $ ./mbr-lba ${binFilePath} ${vhdFormatDiskFilePath}

    # ${binFilePath} 编译好的二进制文件路径
    # ${vhdFormatDiskFilePath} vhd格式的磁盘文件路径
```