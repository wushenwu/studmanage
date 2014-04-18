#ifndef MEMMANAGE_H_CF771D01_FB96_4a42_BA47_C7B9C76D8B18    //防止头文件被重复引用

#define MEMMANAGE_H_CF771D01_FB96_4a42_BA47_C7B9C76D8B18

//
#define 	MAX_INPUT		        128

//which table: g_add_table, or g_del_table
#define		ADD		                1
#define		DEL			            2
#define     QUERY                   3
#define     MODIFY                  4

//error process
#define     SUCCESS                 0x00
#define     NOT_EXIST               0x01
#define     NO_SPACE                0x02
#define     FAILED                  0x03

//地址类型（相对，绝对）
#define     RELATIVE                0x00
#define     ABSOLUTE                0x01


//for memory space statistics
#define     SPLIT                   8

/*
采用原有memmanage的处理，来实现对学生信息存储的管理
*/
#pragma pack(push)
#pragma pack(1)

//学生信息格式(gender, birthday, score, 变长的姓名， 变长的学号)
typedef struct tag_student
{
    char           chgender;    //
    unsigned short ushbirth;    //mmdd
    float          fscore;  
    /*
    后面紧跟的变长的姓名、学号信息
    */
}tag_student;

//添加表中记录的格式(start, totalsize, namesize)
typedef struct tag_addrecord
{
    unsigned long ulstart;      //学生信息在文件中的偏移
    unsigned char uchsize;      //学生信息占用的总空间
    unsigned char uchnamesize;  //学生姓名的长度
                                //学生学号长度 = size - namesize - sizeof(char)
                                //               - sizeof(short) - sizeof(float)
}tag_addrecord;

//添加表
typedef struct tag_addtable
{
    unsigned long nrecords;             
    tag_addrecord *records;  //变长处理，根据需要实时申请
}tag_addtable;

//删除表中记录的格式(start, totalsize)
typedef struct tag_delrecord
{
    unsigned long ulstart;
    unsigned char uchsize;
}tag_delrecord;
 
//删除表
typedef struct tag_deltable
{
    unsigned long nrecords;
    tag_delrecord *records;   //变长处理，根据需要实时申请
}tag_deltable;

//是否提供字段新信息
typedef struct tag_newinfo
{
    unsigned char gender:1;
    unsigned char birth:1;
    unsigned char score:1;
    unsigned char name:1;
    unsigned char no:1;
    unsigned char infoid:1;
}tag_newinfo;

#pragma pack(pop)


/*不提倡使用全局变量， 尽量不要在头文件中使用extern int value 这类声明*/

/*used for maintain*/
extern struct tag_addtable g_add_table;
extern struct tag_deltable g_del_table;

extern char g_szinfofile[MAX_INPUT];
extern char g_szaddtablefile[MAX_INPUT];
extern char g_szdeltablefile[MAX_INPUT];



/*
Function: 系统初始化，检查学生信息存储文件是否存在，分配表是否存在。
          不存在则进行新建，将分配表加载到内存中
Return  : SUCCESS 成功初始化
          FAILED  初始化失败
*/
int
init(void);

/*
Function: 系统退出前的处理，备份分配表文件，将内存中的分配表写入到文件中
*/
int
finish(void);

/*
Function: 根据信息id进行查询，确认是否存在相关学生，
          主要用于学生信息的修改、删除操作。 
　　
Param	: 编号id, 该id是根据现存学生记录，动态变化的，需要用户根据模糊查询结果进行确认。
           (结果唯一)
           
Return  :  NOT_EXIST不存在， SUCCESS存在,
*/
int 
info_query_by_id(unsigned long infoid);

/*
Function: 对用户输入的学生信息、编号进行模糊查询、联合查询，确认相关学生编号

Param   : tag_student *student, char *szname, char *szno,    //要查询的学生信息
          unsigned long id,                                  //要查询的编号
          int *indexary,                                     //接收查询到的条目编号（下标）
          int *num                                           //查询匹配到的总条目
          tag_newinfo *flag                                  //相应字段是否进行了设置

Return  : SUCCESS存在，并设置indexary.
          NOT_EXIST 不存在， 
          FAILED  查询失败
          
*/
int
info_query_about(tag_student *student, char *szname, char *szno,    //要查询的学生信息
                 unsigned long id,                                  //要查询的编号
                 int *indexary,                                     //接收查询到的条目编号（下标）
                 int *num,                                           //查询匹配到的总条目
                 tag_newinfo *flag
                 );


/*
Function: 将用户输入的学生信息添加进文件中。
　　  
Param	: student中存放着学生定长信息，szname, szno为变长的姓名、学号
　　        
Return  :  NO_SPACE 添加失败， 其他表示添加的位置.

Effect  : 更新g_add_table, g_del_table


  info_add(student, szname, szno):
  　　sizereal = 该学生全部信息长度
  　　如果没有从g_del_table中查询到合适（ 在后面阐述）的(start，size)记录，
        则在文件末尾添加信息，并在g_add_table中追加(start, size, namesize)记录。
  　　
  　　若从g_del_table中查询到一个合适的（在后面阐述) size的记录(start, start)，
        则将学生信息拷贝至start指示的文件偏移处。
        根据sizereal和size的关系，有以下几种情况：
  　　  sizereal = size, 则将(start, start)从g_del_table中删除，
                        同时将(start, sizereal, namesize)有序的插入到g_add_table中 (start作为keyword)
        sizereal < size, 则将g_del_table中的该(start, size)改为(start + sizereal, size - sizereal), 
                        同时，将(start, sizereal, namesize)有序的插入到g_add_table中。

*/
int
info_add(tag_student *student, char *szname, char *szno);

/*
Function: 删除指定编号的学生信息
　　  
Param	: infoid为编号（信息编号，随g_add_table的变化而变化），用户需用模糊查询确认。
　　        
Return  :  NOT_EXIST for failed， SUCCESS 成功.

Effect  : 更新g_add_table, 
           更新g_del_table， 根据情况对空闲空间进行整合，影响g_del_table

info_del(infoid):　
        将该记录从g_add_table中删除，
            同时考虑更新有序表g_del_table. 
        考虑到可能的连续空闲空间的整合，分为以下情况：
　　    如果有合适的前驱(size1, start1) 满足 start1 + size1 = size, 表明该两块空闲空间连续，则应将(size1, start1)更新为(size1+size, start1)
　　    如果(size,start)有合适的后继(size2, start2)满足 start + size = start2, 则(size2, start2)应更新为(size+size2, start)
*/
int
info_del(int infoid);

/*
Function: 修改指定编号的学生信息
　　  
Param	: tag_student *student, char *szname, char *szno,  为新的信息
            int infoid 指定编号
            tag_newinfo *flag  指示各字段是否设置
　　        
Return  :  NOT_EXIST for not exist, NO_SPACE for no space, SUCCESS for success

Effect  : 更新g_add_table, 
           更新g_del_table， 根据情况对空闲空间进行整合，影响g_del_table

info_modify(origin, new):
　　index = info_query(origin)
　　如果index == -1 表明不存在origin, 修改失败。
　　oldsize = len(origin)
　　newsize = len(new)
　　如果index != -1, 它表示origin的(size, start)在g_add_table中的下标，取得start.
　　根据oldsize和newsize的大小，分为以下情况：
　　如果 newsize < oldsize, 则更改g_add_table中的(size, start) 为(newsize, start), 同时在g_del_table中有序的插入(oldsize - newsize, start + newsize)
　　如果 newsize = oldsize, 则不发生变更
　　如果newsize > oldsize, 则此时可以简单调用的info_del(origin)和info_add(string), 完成空间的释放和再整合，以及再分配。
（当然，也可能出现该start前驱或者后继本来已经为空闲的情况，那么此时进行空闲的整合是最好的，但是我们还是将其纳入一般的流程进行处理）。
*/
int
info_modify(tag_student *student, char *szname, char *szno, int infoid, tag_newinfo *flag);


/*
Function: 将chInput[]存储至chBuf[start]
　　  
Param	 : chBuf, start, chInput, requestsize          
　　        
Return  :  NO_SPACE for failed, SUCESS for success

Effect   : 更新chBuf[],根据需要更新g_indexMAX_buf
*/
int
store(char *chBuf, int start, char *chInput, int requestsize);

/*
Function: 将student, szname, szno指定的学生信息存放至文件中

Param   : tag_student *student, char *szname, char *szno,   //学生信息
          unsigned long start,                              //传入传出的文件地址
          unsigned char namesize ,                          //学生姓名占用空间大小
          unsigned char nosize                              //学生学号占用空间大小

Return  : SUCCESS 成功存储
          FAILED  存储失败
*/
int
storeinfo(tag_student *student, char *szname, char *szno,   
          unsigned long *start,                              
          unsigned char namesize ,                          
          unsigned char nosize
      );

/*
Function: for USE_MALLOC_V2, USE_MALLOC_V3, store szinput into space malloced.

Param   : ppszstart 用于接收malloc的地址。
*/
int
store_by_malloc(char *szinput, char **ppszstart);

/*
Function: 遍历g_add_table, 结合g_chBuf, 输出所有存储的字符串输入。
　　  
Param	 : None         
　　        
Return  : None.

Effect  : None
*/
void
display(void);


/*
Function: 统计startaddr所指示的大小为size的连续空间中各地址的使用情况。
　　  
Param	 : unsigned long startaddr 连续地址空间的起始地址。
            可为相对地址（如文件地址），也可以为绝对地址（如malloc所得地址）
           relativeaddrflg 相对地址标志 （RELATIVE 为相对，ABSOLUTE为绝对）
           
            当然，其他可能也需要，比如指定具体文件等szfile, 待改善。
　　        
Return  : SUCCESS 成功  FAILED 失败
Remark  : 由space_stat_for_file进行调用
*/
int
space_stat(unsigned long startaddr, unsigned long size, int relativeaddrflg, char *szfile);

/*
Function: 对文件空间地址使用情况的统计
          会调用space_stat。
*/
int
space_stat_for_file(void);

/*
Function: 进行碎片整理
*/
void
defrag(void);

/*
Function: 统计各字符出现的次数及频率
*/
void
char_stat(void);

/*
Function: 针对szText[]及action, 更新g_char_table中各字符出现的次数

        called whenever info_add, info_del, info_modify success.
*/
static void
updatechar(char *szText, int action);

/*
Function: 对USE_MALLOC_V2，USE_MALLOC_V3中的g_add_table, g_del_table所维护的空间进行释放。
*/
void
free_space(void);


/*
Function: 从添加表记录指定的位置处取得学生信息各字段

Param   : tag_student *student, char *szname, char *szno,  //接收读取的学生信息
          tag_addrecord *addrecord                          //

Return  : SUCCESS 成功获取
          FAILED  获取失败
*/
int
fetch1info(tag_student *student, char *szname, char *szno,  //接收读取的学生信息
           tag_addrecord *addrecord                         // 要读取的学生信息       
           );

/*
Function: 从指定的位置处取得完整的学生信息

Param   : void *infobuf,             //接收读取的学生整体信息
          unsigned long ulstart,     //指定的地址
          unsigned char uchsize      //指定的大小

Return  : SUCCESS 成功获取
          FAILED  获取失败
*/
int
fetch1info_whole(void *infobuf,             //接收读取的学生整体信息
                 unsigned long ulstart,     //
                 unsigned char uchsize
                 );

/*
Function: 统计各学生信息占用空间大小，及对文件总大小占比。
*/
void
info_stat(void);

#endif