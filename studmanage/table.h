#ifndef  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889
#define  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889


/*
Function: 与类型无关的插入表操作

Param   : IN OUT unsigned long *nrecords,         //传入传出的table中record的总数
          void *records,                          //table中record的起始地址
          size_t recordsize,                      //record的大小 
          IN OUT int *index,                      //接收插入的位置
          int (*compare)(void *, void *)          //比较记录大小的回调函数

Remark  : insertintoaddtable, insertintodeltable的实现通过此函数

*/
void
insertintotable(unsigned long *nrecords,         
                void *records,                         
                void *newrecord,                       
                size_t recordsize,
                int *index,
                int (*compare)(void *, void *)     
                );

/*
Function: 将addrecord代表的(start, size, namesize)记录有序的插入到**addtable中
           start作为keywords
　　  
Param	:  addtable, addrecord
　　        
　　        
Return  :  None.

Effect  : 注意，因为我们的g_add_table是根据需要增长的，所以，要realloc进行调整。
          注意参数的传入
*/
void
insertintoaddtable(tag_addtable **addtable, struct tag_addrecord *addrecord);


/*
Function: 将delrecord代表的(start, size)记录有序的插入到**deltable中, 
            并对g_del_table指示连续空闲空间进行整合
　　  
Param	:  deltable, delrecord
　　        
Return  :  None.

Effect   : 更新g_del_table, 对g_del_table指示的连续空闲空间进行整合，进行再分配
            注意，因为我们的g_del_table是按需增长的，所以需要realloc, 
            注意参数的传入
*/
void
insertintodeltable(tag_deltable **deltable, struct tag_delrecord *delrecord);


/*
Function: 与类型无关的表数据删除操作

Param   : IN OUT unsigned long *nrecords,     //传入传出的table中record的总数
          void *records,                      //table中records的起始地址
          size_t recordsize,                  //record的大小
          int index                           //要删除的record在table中的下标
*/
void
removefromtable(unsigned long *nrecords,     //传入传出的table中record的总数
                void *records,                      //table中records的起始地址
                size_t recordsize,                  //record的大小
                int index                           //要删除的record在table中的下标
                );

/*
Function: 将index 所标识的记录从addtable中删除, 并保证addtable的有序性。
　　  
Param	: addtable, index
　　        
Return  :  None.

Effect  : 更新addtable;
           注意，因为realloc对表进行裁剪，所以需要注意传入的参数。
*/
void
removefromaddtable(tag_addtable **addtable, int index);

/*
Function: 将index 所标识的记录从deltable中删除, 并保证deltable的有序性。
　　  
Param	: deltable, index
　　        
Return  :  None.

Effect  : 更新deltable;
           注意，因为realloc对表进行裁剪，所以需要注意传入的参数。
*/
void
removefromdeltable(tag_deltable **deltable, int index);

/*
Function: 与类型无关的表中记录更新操作（整体记录更新）

Param   :  void *records,          //表中records起始地址
            size_t recordsize,      //表中record的大小
            int index,              //要更新的record
            void *newrecord         //新内容

*/
void
updatetable(void *records,          
            size_t recordsize,      
            int index,              
            void *newrecord         
            );

/*
Function: 将addtable中相应index所标识的记录更新为新传入的addrecord。
　　  
Param	: 
　　        
Return  :  None.

Effect  :  更新addtable
*/
void
updateaddtable(tag_addtable *addtable, int index, tag_addrecord *addrecord);


/*
Function: 将deltable中相应index所标识的记录更新为新传入的delrecord。
　　  
Param	: 
　　        
Return  :  None.

Effect  :  更新deltable
*/
void
updatedeltable(tag_deltable *deltable, int index, tag_delrecord *delrecord);


/*
Function: get the table that index belongs to, and set the max value of this range.
           用于space_stat 中设置使用标志U, F

Param   : buf is the storage buf.
          index_add, index_del 为space_stat所传进的参数，
          代表目前对g_add_del, g_del_table所遍历到的下标。
          NO_USE_MALLOC, USE_MALLOC_V1中恒为(0,0)

*/
int
gettable(unsigned long index, int *max, 
         unsigned long index_add, unsigned long index_del);


#endif