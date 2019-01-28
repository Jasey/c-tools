### sharding
> 分表计算工具，可以输入指定的分表基础名和分表下标范围，根据分表主键计算分表的位置

```shell
    #使用方法
    #1. 授权
    chmod 777 sharding-alternately.out
    #2. 运行
    ./sharding-alternately.out [${algorithm}] [${fileName} | ${baseTableName}] [${from}] [${to}]
    # [] 中是可选的，其中${algorithm}是算法类型，使用default使用内置算法(播单使用内置算法),使用file则从文件中读取节点
    # 如果选择的是default, 则需要提供分表基本表明，如果选择file，则需要提供文件名(文件必须位于应用当前目录下)
    # 选择defualt还可以提供分表范围 ${from}, ${to}
    #3. 运行后可以切换算法，具体的可以使用命令 --help 查看
    > --help
    "--info" : show your config
    "--edit" : redit your config
    "--help" : more help
    "quit" : exit
    
    #文件格式必须是csv格式，以逗号隔开，每个数据使用双引号包裹,如下
    "1","2313109423","tb_album0","17/7/2012 18:13:15","18/11/2013 16:08:57","Album0"
    "2","2989584511","tb_album0","17/7/2012 18:13:15","18/11/2013 16:08:57","Album0"
    #虚拟节点一定在第二栏，分表数据在第三栏，后面的信息可以不需要提供

    #其中播单相关的基表名和分表范围如下
    播单 uid分表
    tb_playlist 0 199
    播单索引 播单id分表
    tb_playlist_index 0 199
    收藏 uid分表
    tb_collect_playlist 0 199
    收藏索引 收藏表id分表
    tb_collect_playlist_index 0 199
    播单声音 播单id分表
    tb_playlist_track_index 0 199
    播单相关统计表 专辑id分表
    tb_playlist_offline_statistics 0 199

    #4. 可以重新配置算法和文件，使用命令
    > --redit
```

### mbr-lba
> 磁盘扇区填充程序，按照512字节数据填充扇区，不足512字节的部分会填充0，需要指定需要填充的二进制文件和被填充的磁盘文件

```shell
    #使用方法
    $ gcc mbr-lba.c -o mbr-lba
    $ ./mbr-lba ${binFilePath} ${vhdFormatDiskFilePath} [${lbaAddr}]

    # ${binFilePath} 编译好的二进制文件路径
    # ${vhdFormatDiskFilePath} vhd格式的磁盘文件路径
    # ${lbaAddr} 为可选的，如果不输入，则默认写入主引导扇区(lba addr = 0)
```