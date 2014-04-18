#ifndef FILE_H_A9582592_6060_4472_A92F_3CD06A7500FC

#define FILE_H_A9582592_6060_4472_A92F_3CD06A7500FC

/*
Function: 文件拷贝（每4字节进行）

Param   : 

Return  : SUCCESS 成功拷贝， FAILED 拷贝失败
*/
int
filecopy(char *filesrc, char *filedst);

/*
Function: 检测文件是否存在， 不存在则试图新建。

Param   : szfile 文件名称 （MAX_INPUT限制）

Return  : SUCCESS 确认存在或者新建成功
          NOT_EXIST 不存在（新建失败）

Remark  : 用于init阶段，检测信息存储文件，添加表文件，删除表文件
*/
int
checkfile(char *szfile);

/*
Function: 向tablefile中写入一条record. （与类型无关）

Param   : void *record 是要写入的record信息
          initflg  1 表示初次写入，会将之前的内容清空。 0 继续添加。

Return  : SUCCESS 成功写入  FAILED 失败
*/
int
storerecord(char *tablefile, void *record, size_t recordsize, int initflg);

/*
Function: 备份添加表，将内存中的添加表写入到添加表文件中

Return  : SUCCESS 成功处理 FAILED 失败
*/
int
saveaddtable(void);

/*
Function: 备份删除表，将内存中的删除表写入到删除表文件中

Return : SUCCESS 成功处理 FAILED 失败
*/
int
savedeltable(void);

/*
Function: 从分配表文件中加载分配表，不存在则试图创建

Param   : sztablefile是相应分配表的路径名称
          tabletype是分配表的类型，ADD 或者DEL

Return  : SUCCESS 加载成功或者新建成功
          NOT_EXIST 相应文件不存在
          FAILED  加载失败

Remark  : 
*/
int
loadtable(char *sztablefile, int tabletype);

/*
Function: 从szfile中offset地址读取size字节到buf中

Return : SUCCESS 成功读取， FAILED 失败
*/
int
readbuf(char *buf, unsigned long offset, size_t size, char *szfile);

/*
Function: 处理因defrag未进行truncate, 
        利用addtable, deltable的最后一项来确定信息存储文件的大小。

Return : SUCCESS 成功  FAILED 失败
*/
int
getfilesize(long *filesize);


#endif