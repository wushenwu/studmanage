#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "studmanage.h"
#include "fit.h"
#include "table.h"
#include "ui.h"
#include "file.h"
#include "tool.h"

/*文件相关*/
char g_szinfofile[MAX_INPUT]      = "info.txt";
char g_szaddtablefile[MAX_INPUT]  = "addtable.txt";
char g_szdeltablefile[MAX_INPUT]  = "deltable.txt";

/*分配表相关*/
tag_addtable g_add_table = {0};
tag_deltable g_del_table = {0};

int
init(void)
{
    int result = SUCCESS;

    DisableForceClose();

    if (SUCCESS != checkfile(g_szinfofile))
    {
        goto INIT_ERROR;
    }

    result = loadtable(g_szaddtablefile, ADD);
    if (SUCCESS != result)
    {
        //can do more error check for NOT_EXIST, FAILED
        goto INIT_ERROR;        
    }

    result = loadtable(g_szdeltablefile, DEL);
    if (SUCCESS != result)
    {
        goto INIT_ERROR;
    }

INIT_ERROR:
    if (result != SUCCESS)
    {
        result = FAILED;
    }

    return result;
}

int
finish(void)
{
    int result = 0;

    result = saveaddtable();
    if (FAILED == result)
    {
        showinfo("finish saveaddtable error");
        return result;
    }

    result = savedeltable();
    if (FAILED == result)
    {
        showinfo("finish savedeltable error");
        return result;
    }

    if (g_add_table.records)
    {
        free(g_add_table.records);
        g_add_table.records = NULL;
    }

    if (g_del_table.records)
    {
        free(g_del_table.records);
        g_del_table.records = NULL;
    }

    return result;
}

int 
info_query_by_id(unsigned long infoid)
{
    unsigned long nrecords = 0;

    nrecords = g_add_table.nrecords;
    if (infoid >= 0 && infoid < nrecords)
    {
        return SUCCESS;
    }
    else
    {
        return NOT_EXIST;
    }   
}

int
fetch1info_whole(void *infobuf,             //接收读取的学生整体信息
                 unsigned long ulstart,     //
                 unsigned char uchsize
                 )
{
    FILE *fp   = NULL;
    int result = SUCCESS;    
    
    fp = fopen(g_szinfofile, "rb");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }
    
    fseek(fp, ulstart, SEEK_SET);
    if (ferror(fp))
    {
        perror("Fetch1info_whole fseek error");
        
        result = FAILED;
        goto FETCH_WHOLE_ERROR;
    }
    
    fread(infobuf, uchsize, 1, fp);
    if (ferror(fp))
    {
        perror("Fetch1info_whole fread error");
        
        result = FAILED;
        goto FETCH_WHOLE_ERROR;
    }

FETCH_WHOLE_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return result;
}

int
fetch1info(tag_student *student, char *szname, char *szno,  //接收读取的学生各信息
           tag_addrecord *addrecord                         //要读取的学生信息
           )
{
    FILE *fp   = NULL;
    int result = SUCCESS;

    unsigned char uchsize    = 0;
    unsigned char uchnamesize = 0;
    unsigned char uchnosize   = 0;
    unsigned long ulstart   = 0;

    ulstart     = addrecord->ulstart; 
    uchsize     = addrecord->uchsize;
    uchnamesize = addrecord->uchnamesize;
    uchnosize   = uchsize - uchnamesize - sizeof(tag_student);    

    fp = fopen(g_szinfofile, "rb");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }

    fseek(fp, ulstart, SEEK_SET);
    if (ferror(fp))
    {
        perror("Fetch1info fseek error");
        
        result = FAILED;
        goto FETCH_ERROR;
    }

    fread(student, sizeof(tag_student), 1, fp);
    if (ferror(fp))
    {
        perror("Fetch1info fread tag_student error");

        result = FAILED;
        goto FETCH_ERROR;
    }

    fread(szname, uchnamesize, 1, fp);
    if (ferror(fp))
    {
        perror("Fetch1info fread szname error");

        result = FAILED;
        goto FETCH_ERROR;
    }

    fread(szno, uchnosize, 1, fp);
    if (ferror(fp))
    {
        perror("Fetch1info fread szno error");

        result = FAILED;
        goto FETCH_ERROR;
    }

FETCH_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return result;
}

int
info_query_about(tag_student *student, char *szname, char *szno,    //要查询的学生信息
                 unsigned long infoid,                                            //要查询的编号
                 int *indexary,                                     //接收查询到的条目编号（下标）
                 int *num,                                           //查询匹配到的总条目
                 tag_newinfo *flag
                 )
{
    unsigned char uchnamesize = 0;
    unsigned char uchnosize   = 0;
    unsigned long nrecords    = 0;

    tag_addrecord *paddrecord = NULL;
    tag_student tmpstudent    = {0};
    char sztmpname[MAX_INPUT] = {0};
    char sztmpno[MAX_INPUT]   = {0};

    FILE *fp   = NULL;
    int result = NOT_EXIST;
    unsigned long i = 0;
      
    if (1 == flag->name)
    {
        uchnamesize = strlen(szname) + 1;
    }

    if (1 == flag->no)
    {
        uchnosize = strlen(szno) + 1;
    }
    
    *num = 0;   //not a good idea to use the param
    nrecords = g_add_table.nrecords;
    for (i = 0; i < nrecords; i++)
    {
        //对多字段进行联合查询

        if (uchnamesize > g_add_table.records[i].uchnamesize
            || uchnosize > (g_add_table.records[i].uchsize 
                            - g_add_table.records[i].uchnamesize
                            - sizeof(tag_student))
           )
        {
            continue;
        }

        paddrecord = &g_add_table.records[i];
        result = fetch1info(&tmpstudent, sztmpname, sztmpno,
                            paddrecord
                            );
        if (FAILED == result)
        {
            return FAILED;
        }

        if (uchnamesize > 1
            && (NULL == strstr(sztmpname, szname))
            )
        {
            continue;
        }

        if (uchnosize > 1
            && (NULL == strstr(sztmpno, szno))
            )
        {
            continue;
        }

        if (1 == flag->gender
            && student->chgender != tmpstudent.chgender)
        {
            continue;
        }

        if (1 == flag->score
            && tmpstudent.fscore < student->fscore)
        {
            continue;
        }

        if (1 == flag->birth
            && tmpstudent.ushbirth < student->ushbirth)
        {
            continue;
        }

        if (1 == flag->infoid
            && infoid != i)
        {
            continue;
        }

        *(indexary + *num) = i;
        *num += 1;       
    }

    if (0 != *num)
    {
        result = SUCCESS;
    }
    
    return result;
}

int
store_by_malloc(char *szinput, char **ppszstart)
{
    char *pszstart = NULL;
    size_t len     = 0;
    int result     = SUCCESS;

    len = strlen(szinput);
    
    pszstart = (char *)malloc(len + 1);
    if (NULL == pszstart)
    {
        result = NO_SPACE;
        return result;
    }
    else
    {
        strcpy(pszstart, szinput);
        *(pszstart + len) = '\0';

        *ppszstart = pszstart;

        return result;
    }
}

#if defined(NO_USE_MALLOC) || defined(USE_MALLOC_V1)
int
store(char *chBuf, int start, char *szinput, int requestsize)
{
    int result = SUCCESS;

    //make sure g_indexMAX_buf updated
    if (start == g_indexMAX_buf)
    {
        if (MAX_SPACE == g_indexMAX_buf)
        {
            result = NO_SPACE;
            return result;            
        }
        g_indexMAX_buf += requestsize;
    }

    strcpy(&chBuf[start], szinput);
    chBuf[start + requestsize - 1] = '\0'; 

    return result;
} 
#endif

int
storeinfo_whole(void *infobuf,           //学生整体信息
                unsigned long start,     //要存到的文件地址 
                unsigned char size       //学生信息大小   
                )
{
    FILE *fp    = NULL;
    int result  = SUCCESS;
    
    fp = fopen(g_szinfofile, "rb+");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }
    
    fseek(fp, start, SEEK_SET);    
    if (ferror(fp))
    {
        perror("StoreInfo_whole fseek error");
        
        result = FAILED;
        goto STOREINFO_WHOLE_ERROR;
    }
        
    fwrite(infobuf, size, 1, fp);
    if (ferror(fp))
    {
        perror("StoreInfo_whole fwrite error");
        
        result = FAILED;
        goto STOREINFO_WHOLE_ERROR;
    }
   
STOREINFO_WHOLE_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return result;
}

int
storeinfo(tag_student *student, char *szname, char *szno,   //学生信息
      unsigned long *start,                                  //文件地址
      unsigned char namesize,                              //学生姓名占用空间大小，
      unsigned char nosize                                  // 
      )
{
    FILE *fp    = NULL;
    int result  = SUCCESS;

    unsigned long nrecords = 0;
    unsigned long offset = 0;

    fp = fopen(g_szinfofile, "rb+");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }

    if (-1 == *start)
    {
        /*
        //追加至文件末尾
        fseek(fp, 0, SEEK_END);
        */
        //因defrag，未进行truncate, 所以利用addtable,deltable最后一项纪录确认文件结束
        result = getfilesize(&offset);
        if (FAILED == result)
        {
            goto STOREINFO_ERROR;
        }

        fseek(fp, offset, SEEK_CUR);
    }
    else
    {
        //填充至指定位置
        fseek(fp, *start, SEEK_SET);
    }

    if (ferror(fp))
    {
        perror("StoreInfo fseek error");
        
        result = FAILED;
        goto STOREINFO_ERROR;
    }

    //为了追加至末尾的情况，获取添加到的位置
    *start = ftell(fp);
    if (ferror(fp))
    {
        perror("StoreInfo ftell error");

        result = FAILED;
        goto STOREINFO_ERROR;
    }

    fwrite(student, sizeof(tag_student), 1, fp);
    if (ferror(fp))
    {
        perror("StoreInfo fwrite error");

        result = FAILED;
        goto STOREINFO_ERROR;
    }

    fwrite(szname, namesize, 1, fp);
    if (ferror(fp))
    {
        perror("StoreInfo fwrite szname error");

        result = FAILED;
        goto STOREINFO_ERROR;
    }

    fwrite(szno, nosize, 1, fp);
    if (ferror(fp))
    {
        perror("StoreInfo fwrite szno error");

        result = FAILED;
        goto STOREINFO_ERROR;
    }

STOREINFO_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    return result;
}

int
info_add(tag_student *student, char *szname, char *szno)
{
    unsigned long ulstart     = 0;      //学生信息在文件中的偏移
    unsigned char uchsize     = 0;      //学生信息占用的总空间
    unsigned char uchnamesize = 0;      //学生姓名占用的空间
    unsigned char uchnosize   = 0;      //学生学号占用的空间
    unsigned char uchtmpsize  = 0;      //从g_del_table中查询到的符号条件空间的大小

    tag_addrecord addrecord = {0};
    tag_delrecord delrecord = {0};

    tag_addtable *paddtable = NULL;
    tag_deltable *pdeltable = NULL;

    int index  = 0;
    int result = SUCCESS;

    //also store the '\0'
    uchnamesize = strlen(szname) + 1;
    uchnosize   = strlen(szno) +1;
    uchsize     = sizeof(tag_student) + uchnamesize + uchnosize;

    //使用g_del_table, 对已删除空间再利用，所以优先查询g_del_table
    result = getsize(uchsize, &index);  
    if (NOT_EXIST == result)
    {
        ulstart = -1;   //追加至文件尾
        result = storeinfo(student, szname, szno,
                            &ulstart,             
                            uchnamesize,
                            uchnosize
                            );

        if (FAILED == result)
        {
            return result;
        }

        addrecord.ulstart     = ulstart;
        addrecord.uchsize     = uchsize;
        addrecord.uchnamesize = uchnamesize;
        paddtable = &g_add_table;
        insertintoaddtable(&paddtable,   //注意参数
                           &addrecord
                           );                            

    }//end for if(NOT_EXIST == result)
    else
    {
        uchtmpsize  = g_del_table.records[index].uchsize;
        ulstart     = g_del_table.records[index].ulstart;

        result = storeinfo(student, szname, szno,
                            &ulstart,
                            uchnamesize,
                            uchnosize);

        if (FAILED == result)
        {
            return result;
        }

        if (uchtmpsize == uchsize)
        {
            addrecord.uchsize  = uchsize;
            addrecord.ulstart  = ulstart;
            pdeltable = &g_del_table;
            removefromdeltable(&pdeltable, index);   //注意参数
            paddtable = &g_add_table;
            insertintoaddtable(&paddtable, &addrecord); //注意参数
        }
        else
        {
            delrecord.uchsize  = uchtmpsize - uchsize;
            delrecord.ulstart  =  ulstart + uchsize;
            updatedeltable(&g_del_table, index, &delrecord);

            addrecord.uchsize  = uchsize;
            addrecord.ulstart  = ulstart;
            paddtable = &g_add_table;
            insertintoaddtable(&paddtable, &addrecord); //注意参数
        }
    }//end for NOT_EXIST != result

    //This is not a good idea using a var to represent more than one meaning.
    result = addrecord.ulstart;
    return result;
}

int
info_del(int infoid)
{
    tag_delrecord delrecord = {0};
    unsigned char uchsize = 0;
    unsigned long ulstart = 0;

    tag_student student    = {0};
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT]   = {0};

    tag_deltable *pdeltable = NULL;
    tag_addtable *paddtable = NULL;

    int result = SUCCESS;
       
    uchsize  = g_add_table.records[infoid].uchsize;
    ulstart = g_add_table.records[infoid].ulstart;
    
    delrecord.uchsize = uchsize;
    delrecord.ulstart = ulstart;
    pdeltable = &g_del_table;
    insertintodeltable(&pdeltable, &delrecord);  //注意参数
    paddtable = &g_add_table;
    removefromaddtable(&paddtable, infoid);   //注意参数    
    
    return result;
}

int
info_modify(tag_student *student, char *szname, char *szno, int infoid, tag_newinfo *flag)
{
    tag_delrecord delrecord  = {0};
    tag_addrecord addrecord  = {0};
    unsigned char uchnewsize = 0;
    unsigned char ucholdsize = 0;
    unsigned char uchsize    = 0;
    unsigned char uchnamesize = 0;
    unsigned char uchnosize   = 0;
    unsigned long ulstart   = 0;

    tag_deltable *pdeltable = NULL;
    
    tag_addrecord *paddrecord = NULL;
    tag_student tmpstudent    = {0};
    char sztmpname[MAX_INPUT] = {0};
    char sztmpno[MAX_INPUT]   = {0};
    
    int result = SUCCESS; 
    
    ulstart     = g_add_table.records[infoid].ulstart; 
    
    //get the old info first
    paddrecord = &g_add_table.records[infoid];
    result = fetch1info(&tmpstudent, sztmpname, sztmpno, 
                        paddrecord
                        );

    if (FAILED == result)
    {
        showinfo("info_modify fetch1info error\r\n");
        return result;
    }

    ucholdsize = g_add_table.records[infoid].uchsize;
    uchnewsize = sizeof(tag_student);  //to be updated

    uchnamesize = g_add_table.records[infoid].uchnamesize;  //to be updated, maybe
    uchnosize   = ucholdsize - uchnamesize - sizeof(tag_student);

    //new info for gender
    if (1 == flag->gender)
    {
        tmpstudent.chgender = student->chgender; 
    }

    if (1 == flag->birth)
    {
        tmpstudent.ushbirth = student->ushbirth;
    }

    if (1 == flag->score)
    {
        tmpstudent.fscore = student->fscore;
    }
      
    //we will write sztmpname
    if (1 == flag->name)
    {
        strcpy(sztmpname, szname);
        uchnamesize = strlen(sztmpname) + 1;
    }

    uchnewsize += uchnamesize;

    if (1 == flag->no)
    {
        strcpy(sztmpno, szno);
        uchnosize = strlen(sztmpno) + 1;
    }

    uchnewsize += uchnosize;
    
    // this will generate unused space, we need to update g_add_table and g_del_table;
    if (uchnewsize < ucholdsize)
    {
        result = storeinfo(&tmpstudent, sztmpname, sztmpno,
                            &ulstart,
                            uchnamesize,
                            uchnosize
                            );
                                   
        if (FAILED == result)
        {
            return result;
        }
        
        addrecord.uchsize  = uchnewsize;
        addrecord.ulstart = ulstart;
        addrecord.uchnamesize = uchnamesize;
        updateaddtable(&g_add_table, infoid, &addrecord);
        
        delrecord.uchsize  = ucholdsize - uchnewsize;
        delrecord.ulstart = ulstart + uchnewsize;
        pdeltable = &g_del_table;
        insertintodeltable(&pdeltable, &delrecord);  //注意参数
    }//end for if (uchnewsize < ucholdsize)
    else if (uchnewsize == ucholdsize)
    {
        result = storeinfo(&tmpstudent, sztmpname, sztmpno,
            &ulstart,
            uchnamesize,
            uchnosize
            );
        
        if (FAILED == result)
        {
            return result;
        }
    }//end for if (uchnewsize == ucholdsize)
    else
    {
        info_del(infoid);
        result = info_add(&tmpstudent, sztmpname, sztmpno);
    }//end for if (uchnewsize > ucholdsize)    
    
    return result;
}

void
display(void)
{
    char szhitinfo[MAX_INPUT] = {0};
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_student student       = {0};
    tag_addrecord *paddrecord = NULL;

    unsigned long ulstart = 0;
    int nrecords = 0;
    int result   = SUCCESS;
    int i        = 0;

    nrecords = g_add_table.nrecords;    
    
    showinfo("id   name   no   gender   birthday   score\r\n");
    for (i = 0; i < nrecords; i++)
    {
        ulstart = g_add_table.records[i].ulstart;

        paddrecord = &g_add_table.records[i];
        result = fetch1info(&student, szname, szno, 
                            paddrecord);
        if (FAILED == result)
        {
            showinfo("Display fetch1info error\r\n");
        }

        sprintf(szhitinfo, "    start at: %d\r\n", ulstart);
        showinfo(szhitinfo);
        show1info(&student, szname, szno, i);
    }
}

/*
Function: 显示buf[index, index+1, .. index+len-1]间的字符
*/
void
showbuf(char *buf, int index, int len)
{
    char szhitinfo[8] = {0};
    int i;

    //ugly
    if (index < 0)
    {
        index = 0;
    }

    for (i = index; i < index + len; i++)
    {
        if ('\0' == buf[i])
        {
            showinfo("\\0 ");
        }
        else
        {
            sprintf(szhitinfo, "%02X ", (unsigned char)buf[i]);  
            showinfo(szhitinfo);
        }
    }
}

#if 0
/*
 g_add_table, g_del_table 已经各自有序
 对于其中相邻的(size1, start1), (size2, start2)
 如果start2 == start1 + size1, 说明它们是相邻的。
 这在USE_MALLOC_V2中是对删除空间再利用的情况，所以不能对start2进行free.
*/
void
free_space(void)
{
    int nrecords_add = 0;
    int index_add = 0;
    int size_add = 0;
    int start_add = 0;
    
    int nrecords_del = 0;
    int index_del = 0;
    int size_del = 0;
    int start_del = 0;

    int start_next = 0;    //用于判断那些为malloc所得空间的一部分的空间，不能再释放。
    
    nrecords_add = g_add_table.nrecords;
    nrecords_del = g_del_table.nrecords; 
    
    for ( ; index_add < nrecords_add && index_del < nrecords_del; )
    {
        size_add  = g_add_table.records[index_add].uchsize;
        start_add = g_add_table.records[index_add].ulstart;
        if (start_add == start_next)
        {
            //该start_add所指示的空间，为malloc所得空间的一部分，已经在之前释放。
            index_add++;
            continue;
        }
        
        size_del  = g_del_table.records[index_del].size;
        start_del = g_del_table.records[index_del].start;
        if (start_del == start_next)
        {
            index_del++;
            continue;
        }
        
        //先释放较小地址空间，避免释放的错误。
        if (start_add < start_del)
        {
            free((void *)start_add);
            start_next = start_add + size_add;  //
            index_add++;      
        }
        else
        {
            free((void *)start_del);
            start_next = start_del + size_del;  //
            index_del++;
        }
    }
    
    //perhaps some add records remains
    for ( ; index_add < nrecords_add; index_add++ )
    {
        size_add  = g_add_table.records[index_add].size;
        start_add = g_add_table.records[index_add].start;
        if (start_add == start_next)
        {
            start_next = start_add + size_add;
            continue;
        }
        else
        {
            free((void *)start_add);
        }
    }
    
    //perhaps some del records remains
    for ( ; index_del < nrecords_del; index_del++)
    {
        size_del  = g_del_table.records[index_del].size;
        start_del = g_del_table.records[index_del].start;        
        if (start_del == start_next)
        {
            start_next = start_del + size_del;
            continue;
        }
        else
        {
            free((void *)start_del);
        }           
    }
}
#endif

#if 0
int
space_stat_for_file(void)
{
    int nrecords_add = 0;
    int index_add = 0;
    unsigned char size_add = 0;
    unsigned long start_add = 0;

    int nrecords_del = 0;
    int index_del = 0;
    unsigned char size_del = 0;
    unsigned long start_del = 0;  
    
    tag_addrecord *paddrecord = NULL;

    char strinfobuf[MAX_INPUT * 2] = {0};
    int result = SUCCESS;

    nrecords_add = g_add_table.nrecords;
    nrecords_del = g_del_table.nrecords; 
    
    for ( ; index_add < nrecords_add && index_del < nrecords_del; )
    {
        size_add  = g_add_table.records[index_add].uchsize;
        start_add = g_add_table.records[index_add].ulstart;

        size_del  = g_del_table.records[index_del].uchsize;
        start_del = g_del_table.records[index_del].ulstart;

        if (start_add < start_del)
        {
            result = fetch1info_whole(strinfobuf, start_add, size_add);
            if (FAILED == result)
            {
                showinfo("space_stat_for_file fetch1info_whole add error\r\n");
                return result;
            }

            //the smaller add first
            space_stat(strinfobuf, start_add, size_add, index_add, index_del);
            index_add++;      
        }
        else
        {
            result = fetch1info_whole(strinfobuf, start_del, size_del);
            if (FAILED == result)
            {
                showinfo("space_stat_for_file fetch1info_whole del error\r\n");
                return result;
            }

            space_stat(strinfobuf, start_del, size_del, index_add, index_del);
            index_del++;
        }
    }

    //perhaps some add records remains
    for ( ; index_add < nrecords_add; index_add++ )
    {
        size_add  = g_add_table.records[index_add].uchsize;
        start_add = g_add_table.records[index_add].ulstart;

        result = fetch1info_whole(strinfobuf, start_add, size_add);
        if (FAILED == result)
        {
            showinfo("space_stat_for_file fetch1info_whole add remain error\r\n");
            return result;
        }

        space_stat(strinfobuf, start_add, size_add, index_add, index_del);
    }

    //perhaps some del records remains
    for ( ; index_del < nrecords_del; index_del++)
    {
        size_del  = g_del_table.records[index_del].uchsize;
        start_del = g_del_table.records[index_del].ulstart;

        result = fetch1info_whole(strinfobuf, start_del, size_del);
        if (FAILED == result)
        {
            showinfo("space_stat_for_file fetch1info_whole del remain error\r\n");
            return result;
        }

        space_stat(strinfobuf, start_del, size_del, index_add, index_del);     
    }

    return result;
}
#endif 

int
space_stat_for_file(void)
{
    unsigned long startaddr = 0; //
    unsigned long filesize = 0;

    int result = SUCCESS;

    result = getfilesize(&filesize);
    if (FAILED == result)
    {
        return result;
    }

    result = space_stat(startaddr, filesize, RELATIVE, g_szinfofile);
    if (FAILED == result)
    {
        return result;
    }

    return result;
}


int
space_stat(unsigned long startaddr, unsigned long size, //连续地址空间信息
           int relativeaddrflg,     //相对 or 绝对
           char *szfile)            //为文件相对地址时，指定文件  
{
    char szhitinfo[32] = {0};
    char buf[SPLIT * 2] = {0};
    unsigned long index_add = 0;   //添加表下标
    unsigned long index_del = 0;   //删除表下标
    unsigned long index_buf = 0;   //地址空间偏移
    unsigned long readcount = 0;   //每SPLIT字节读取的次数
    unsigned long readremain = 0;  //不足SPLIT字节读取的剩余字节
    unsigned long max       = 0;    

    int col       = 0;    
    int table     = 0;
    int start     = 0;
    int len       = 0;
    int remain    = 0;
    int i         = 0;
    char flag     = '\0'; //F or U
    int result    = SUCCESS;

    table     = ADD;
    max       = 0; 
    
    readcount = size / (SPLIT);
    readremain = size % (SPLIT);

    for (index_buf = startaddr; index_buf < startaddr + size; index_buf++)
    {
        //每行开始读取buf
        if (0 == col % SPLIT)
        {
            col = 0;

            if (0 == readcount)
            {
                result = readbuf(buf, index_buf, readremain, szfile);
            }
            else
            {
                //每SPLIT字节读取
                result = readbuf(buf, index_buf, SPLIT, szfile);
                if (SUCCESS == result)
                {
                    readcount--;
                }
            }

            if (SUCCESS != result)
            {
                return result;
            }           
        }//end for if (0 == col % (SPLIT))        

        //this index belong to which one? g_add_table, g_del_table ?
        if (index_buf >= max)
        {
            table = gettable(index_buf, &max, index_add, index_del);  
            
            if (ADD == table)
            {
                //not a good habit to reuse the param
                index_add++;
            }
            else
            {
                index_del++;
            }
        }//end for if (index_buf >= max)
        else
        {
            //flag keeps the same until max
            //so no need to gettable again.
        }        

        if (ADD == table)       //used
        {
            flag = 'U';
        }
        else if (DEL == table)  //free
        {
            flag = 'F';
        }
        else
        {
            //sth wrong?
        }

        //输出开头的偏移
        if (0 == col % (SPLIT))
        {
            sprintf(szhitinfo, "%p: ", index_buf); 
            showinfo(szhitinfo);
        }
       
        //输出各空间的状态标志
        sprintf(szhitinfo, "%c ", flag);
        showinfo(szhitinfo);
  
        //输出各空间相应的字符
        if ((col != 0)
            &&(0 == (col + 1) % (SPLIT)) 
            )
        {
            showinfo("    ");

            start = 0;
            len   = (SPLIT);
            showbuf(buf, start, len);

            showinfo("\r\n");
        }
        col += 1;
    } //end for ( ; index_buf < size; index_buf++)
    
    //对最后不足一个SPLIT的处理
    remain = (SPLIT) - col % (SPLIT);
    for (i = 0; i < remain; i++)
    {
        showinfo("  ");
    }
    showinfo("  ");
    
    len   = (SPLIT) - remain;
    start = 0;
    
    showbuf(buf, start, len);
    
    return result;
}

void
info_stat(void)
{
    char szhitinfo[MAX_INPUT] = {0};
    unsigned char uchsize  = 0;
    unsigned long nrecords = 0;
    long filesize = 0; 
    int  result   = SUCCESS;
    unsigned long i = 0;

    result = getfilesize(&filesize);
    if (FAILED == result)
    {
        showinfo("info_stat getfilesize error");
        return;
    }

    showinfo("id: 占用大小  占比\r\n");
    nrecords = g_add_table.nrecords;
    for (i = 0; i < nrecords; i++)
    {
        uchsize = g_add_table.records[i].uchsize;
        sprintf(szhitinfo, "%d: %d  %f\r\n", i, uchsize, uchsize * 1.0f/filesize);
        showinfo(szhitinfo);
    } 
}

/*
Function: used for debug, show detail info about g_add_table, g_del_table
*/
void
showtable(void)
{
    char szhitinfo[64] = {0};
    unsigned long nrecords = 0;
    unsigned long i = 0;
    
    showinfo("\r\n[ Debug ]showing g_add_table (start, size) ......\r\n");    
    showinfo("g_add_table:\r\n");

    nrecords = g_add_table.nrecords;
    for (i = 0; i < nrecords; i++)
    {
        //the start is relative to the same base
        sprintf(szhitinfo, "(%d %d) ", g_add_table.records[i].ulstart, 
                                       g_add_table.records[i].uchsize);
        showinfo(szhitinfo);
    }
    showinfo("\r\n\r\n");
    
    showinfo("g_del_table:\r\n");
    for (i = 0; i < g_del_table.nrecords; i++)
    {
        sprintf(szhitinfo, "(%d %d) ", g_del_table.records[i].ulstart, 
                                       g_del_table.records[i].uchsize);
        showinfo(szhitinfo);
    }
    showinfo("\r\n\r\n"); 
}

int
moveforward(int index, unsigned char step)
{
    char strinfobuf[MAX_INPUT * 2] = {0};
    unsigned long  nrecords = 0;
    unsigned long ulstart = 0;
    unsigned char uchsize = 0;

    int result = 0;
    unsigned long i = 0;

    nrecords = g_add_table.nrecords;
    for (i = index; i < nrecords; i++)
    {
        ulstart = g_add_table.records[i].ulstart;
        uchsize = g_add_table.records[i].uchsize;

        result = fetch1info_whole(strinfobuf, ulstart, uchsize);
        if (FAILED == result)
        {
            showinfo("moveforward fetch error\r\n");
            return result;
        }

        //move from ulstart to ulstart - step
        result = storeinfo_whole(strinfobuf, ulstart - step, uchsize);
        if (FAILED == result)
        {
            showinfo("moveforward store error\r\n");
            return result;
        }
    }

    //now need to retail the file size
    //TODO

    return result;
}

void
defrag(void)
{
    unsigned char size_del = 0;
    unsigned long start_del = 0;
    unsigned char size_add = 0;
    unsigned long start_add = 0;    
    int nrecords_add = 0;
    unsigned long i = 0, j = 0, k = 0;
     
    nrecords_add = g_add_table.nrecords;
    i            = g_del_table.nrecords; 
    
    //walk through del_table
    for ( ;i >= 1; i--)
    {
        size_del  = g_del_table.records[i - 1].uchsize;
        start_del = g_del_table.records[i - 1].ulstart;        

        //find the first proper index (size, start) in add_table which just follows the del record
        for (j = nrecords_add ; j >= 1; j--)
        {
            size_add  = g_add_table.records[j - 1].uchsize;
            start_add = g_add_table.records[j - 1].ulstart;
            
            if (start_add == start_del + size_del)
            {
                //we've already gotten the index
                break;
            }
        }

        //now need to move info forward
        moveforward(j, size_del);

        //remove (size_del, start_del) from del_table
        g_del_table.records[i - 1].uchsize  = 0; //size
        g_del_table.records[i - 1].ulstart = 0; //start
        g_del_table.nrecords--;

        //update (size, start) in add_table to (size, start - size_del)
        for (k = nrecords_add; (j >= 1) && (k >= j); k--)
        {
            g_add_table.records[k - 1].ulstart = g_add_table.records[k - 1].ulstart - size_del;            
        }           
    }
}

#if 0
void
char_stat(void)
{
    double percent = 0.0;
    char ch;
    int  num;
    int i;
    char szhitinfo[64] = {0};

    showinfo("Show char info......\r\n");

    sprintf(szhitinfo, "Total: %d\r\n", g_totalchar);
    showinfo(szhitinfo);

    for (i = 0; i < (MAX_CHAR - MIN_CHAR + 1) ; i++ )
    {
        if (0 != g_char_table[i])       
        {
            //0x21 is the first printable(exclude space) within ascii
            ch  = i + 0x21;
            num = g_char_table[i];
            percent = num * 1.0 / g_totalchar;

            sprintf(szhitinfo, "%c %d %f\r\n", ch, num, percent);
            showinfo(szhitinfo);
        }
    }
}
#endif

#if 0
static void
updatechar(char *szText, int action)
{
    int index;
    int contribute;
    size_t i;
    size_t length;

    length = strlen(szText);

    if (ADD == action)
    {
        contribute = 1;
    }
    else if (DEL == action)
    {
        contribute = 0;
    }
    else
    {
        //sth wrong??
    }

    for (i = 0; i < length; i++ )
    {
        index = szText[i] - MIN_CHAR;
        g_char_table[index] += contribute;
        g_totalchar += contribute;
    }
}
#endif

void
process_add(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    char           chgender   = 'F';    //
    unsigned short ushbirth   = 1212;   //mmdd
    float          fscore     = 0.0f;
    tag_student student       = {0};
    int result                = 0;     //ugly

    //Add
    getinput(szname, "Enter Name:   ");
    getinput(szno,   "Enter No  :   ");
    getgender(&chgender, "Enter Gender:   ");
    getbirthday(&ushbirth, "Enter Birthday(mmdd):   ");
    getscore(&fscore, "Enter Score:   ");

    student.chgender = chgender;
    student.ushbirth = ushbirth;
    student.fscore   = fscore;    
    
    result = info_add(&student, szname, szno);
    if (NO_SPACE == result)
    {
        showinfo("Add failed, for No More Space\r\n");
    }
    else
    {
        //not a good habit to reuse result for different meanning.
        sprintf(szhitinfo, "\r\nADD: store into %d\r\n", result);
        showinfo(szhitinfo);
    }
}

void
process_query(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_student student       = {0};
    tag_newinfo flag          = {0};
    tag_addrecord *paddrecord = NULL;
    unsigned long infoid      = 0;
    
    int result                = 0;     //ugly
    int ch = 0;    
    int i  = 0;

    int indexary[MAX_INPUT]   = {0};    //接收查询到的编号
    int num                   = 0;      //查询匹配到的总数目
    int index                 = 0;
    char szqueryed[MAX_INPUT*2] = {0};
    
    //Query
    showinfo("支持对学生基本信息、姓名、学号和编号的模糊查询、联合查询\r\n");
    getnewinfo(&student, szname, szno, &infoid, QUERY, &flag);

    if (0 == *(unsigned char*)(&flag))
    {
        return;
    }
   
    result = info_query_about(&student, szname, szno,
                               infoid,
                               indexary,
                               &num,
                               &flag
                               );
    
    if (NOT_EXIST == result)
    {
        showinfo("Query: not exist\r\n");
        return;
    }
    
    //模糊查询结果输出
    showinfo("Queryed: id  name     no      gender birthday  score \r\n");
    for (i = 0; i < num; i++)
    {
        //查询到的字符串的(size, start)记录在g_add_table中的下标
        index = indexary[i];    
       
        paddrecord = &g_add_table.records[index];
        result = fetch1info(&student, szname, szno, 
                            paddrecord);
        
        if (FAILED == result)
        {
            showinfo("process_query fetch1info error");
        }
        
        show1info(&student, szname, szno, infoid);
    }
}

int
process_del(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    int infoid                = 0;   
    int result                = 0;
    int ch                    = 0;
    
    tag_addrecord *paddrecord = NULL;
    tag_student student    = {0};
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT]   = {0};

    showinfo("仅能删除具体编号的学生信息，请根据模糊查询确认编号\r\n");
    showinfo("请输入编号：");
    getselect(&infoid);

    result = info_query_by_id(infoid);   
    if (NOT_EXIST == result)
    {
        sprintf(szhitinfo, "%d does not exist\r\n", infoid);
        showinfo(szhitinfo);
        return result;
    }

    paddrecord = &g_add_table.records[infoid];
    result = fetch1info(&student, szname, szno, 
                        paddrecord
                        );
    if (FAILED == result)
    {
        showinfo("info_del fetch1info error\r\n");
        return result;
    }
    
    showinfo("queryed:  id  name   no    gender  birthday  score\r\n");
    show1info(&student, szname, szno, infoid);
    
    showinfo("Are you sure to delete this one? (Y/N) >>");
    ch = getchar();
    fflush(stdin);
    if ('N' == ch)
    {
        return result;
    }
    
    result = info_del(infoid); 
    showinfo("Del ok\r\n");
    
    return result;
}

int
process_modify(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_student student       = {0};
    tag_newinfo flag          = {0};
    tag_addrecord *paddrecord = NULL;
    int infoid                = 0;
    int result                = SUCCESS;     //ugly
    int ch = 0;    
    int i  = 0;
    
    int indexary[MAX_INPUT]   = {0};    //接收查询到的编号
    int num                   = 0;      //查询匹配到的总数目
    int index                 = 0;
    char szqueryed[MAX_INPUT*2] = {0};
    
    showinfo("仅能修改指定编号的学生信息，可通过模糊查询确认编号\r\n");
    showinfo("请输入具体编号: ");
    getselect(&infoid);

    result = info_query_by_id(infoid);
    if (NOT_EXIST == result)
    {
        showinfo("NOT EXIST\r\n");
        return result;
    }
   
    paddrecord = &g_add_table.records[infoid];
    result = fetch1info(&student, szname, szno, 
                        paddrecord
                        );
    if (FAILED == result)
    {
        showinfo("Process_modify fetch1info error\r\n");
        return result;
    }
    
    showinfo("queryed: id  name   no  gender birthday score\r\n");
    show1info(&student, szname, szno, infoid);
    
    showinfo("Are you sure to modify? (Y/N) >>");
    ch = getchar();
    fflush(stdin);
    if ('N' == ch)
    {
        return result;
    }      

    //Modify
    showinfo("支持对学生基本信息、姓名、学号修改\r\n");
    getnewinfo(&student, szname, szno, &infoid, MODIFY, &flag);

    if (0 == *(unsigned char *)&flag)
    {
        return SUCCESS;
    }
    
    result = info_modify(&student, szname, szno, infoid, &flag);
    if (NOT_EXIST == result)
    {
        sprintf(szhitinfo, "Modify: %d not exist\r\n", infoid);
        showinfo(szhitinfo);
    }
    else if (NO_SPACE == result)
    {
        showinfo("Modify: no more space to store newinput\r\n");
    }
    else
    {
        showinfo("Modify ok\r\n");
        
        //
        //updatechar(szinput, DEL);
        //updatechar(newinput, ADD);
    }

    return result;
}

void
process_fit(void)
{
    int nselect = 0;

    //set fit
    showinfo("Please choose the fit way\r\n");    
    showfit();    
    getselect(&nselect);
    
    if (nselect < FIT_EQUAL || nselect > FIT_EQUAL_FIRST )  //change accordingly
    {
        showinfo("Please make the right decision ^_^\r\n");
    }
    else
    {
        setfit(nselect);
    }
}

int
getaverscore(float *fscore)
{
    unsigned long nrecords = 0;

    tag_student student = {0};
    tag_addrecord *paddrecord = NULL;
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT]    = {0};

    float totalscore = 0.0;
    float averscore  = 0.0;
    int result = SUCCESS;
    unsigned long i = 0;

    nrecords = g_add_table.nrecords;
    for (i = 0; i < nrecords; i++)
    {
        paddrecord = &g_add_table.records[i];
        result = fetch1info(&student, szname, szno,
                             paddrecord);
        if (FAILED == result)
        {
            showinfo("process_averscore fetch1info error");
            return result;
        }
        
        totalscore += student.fscore;
    }

    if (nrecords)
    {
        averscore = totalscore / nrecords;

        *fscore = averscore;
    }
    
    return result;
}

void
dispatch(int nselect)
{
    char szhitinfo[MAX_INPUT] = {0};
    float fscore = 0.0f;
    int result = 0;

    switch (nselect)
    {
    default:
        {
            //should not come to this point. whenever here, something wrong in the main.
            showinfo("Please make the right decision ^_^\r\n");
            break;
        }

    case 1:
        {
            process_add();
    
            //write addtable, deltable to file
            saveaddtable();
            savedeltable();
            break;
        }
  
    case 2:
        {
            process_query();
            break;
        }

    case 3:
        {
            process_del();

            saveaddtable();
            savedeltable();
            break;
        }

    case 4:
        {
            process_modify();

            saveaddtable();
            savedeltable();
            break;
        }

    case 5:
        {
            //display:
            showinfo("Displaying......\r\n");
            
            display();            
            break;
        }

    case 6:
        {
            //statistics
            showinfo("Statisticsing......U for Used, F for Free\r\n\r\n");

            space_stat_for_file();
            break;
        }

    case 7:
        {
            process_fit();
            break;
        }

    case 8:
        {
            defrag();

            saveaddtable();
            savedeltable();
            break;
        }

    case 9:
        {
            info_stat();
            break;
        }

    case 10:
        {
            result = getaverscore(&fscore);
            if (SUCCESS == result)
            {
                sprintf(szhitinfo, "averscore is %f\r\n", fscore);
                showinfo(szhitinfo);
            }

            break;
        }
    }//end for switch
}

int
main(void)
{
    int nselect = 0;
    int result = SUCCESS;

    result = init();
    if (SUCCESS != result)
    {
        showinfo("init error\r\n");
        return -1;
    }

    while (1)
    {
        showmenu();

        getselect(&nselect);
        
        if (0 == nselect)
        {
            break;
        }
        else if(nselect < 0 || nselect > 10) //change this according to the menu
        {
            showinfo("Please make the right decision ^_^\r\n");
            continue;
        }

        dispatch(nselect);

        //just for debug, can be removed
        showtable();
    }

    result = finish();
    if (FAILED == result)
    {
        showinfo("finish savetable failed");
        return -1;
    }

    return 0;
}