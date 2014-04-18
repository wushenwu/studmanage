#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "studmanage.h"
#include "ui.h"

void
usage(void)
{
    printf("e.g mycopy src.txt  dst.txt\r\n");
}

/*
Function: 对pfn如fread, fwrite的封装，主要是错误检查。

Param   : szerrorinfo 用于接收错误提示

Return  : 1 for success, 0 for error.
*/
int 
operatefile(char *buf, size_t size, size_t count, FILE *fp, 
           size_t (*pfn)(char *, size_t, size_t, FILE *),
           char *szerrorinfo
           )
{
    int result = 1; //success

    if (count != pfn(buf, size, count, fp))
    {
        result = 0;

        //know more with error check
        if (feof(fp))
        {
            strcpy(szerrorinfo, "Come to end of file\r\n");
        }
        else
        {
            strcpy(szerrorinfo, "Something wrong\r\n");
        }
    }

    return result;
}

/*
Function: 拷贝文件，每次拷贝4字节

Remark  : 这里的错误处理较为丑陋。
*/
int
mycopy(char *filesrc, char *filedst)
{
    int result       = 1;
    FILE *fpsrc      = NULL;
    FILE *fpdst      = NULL;
    long lnfsize    =  0;
    unsigned ncount  = 0;
    unsigned nremain = 0;
    char buf[8]      = {0};
    unsigned i       = 0;
    char szerrorinfo[32] = {0};

    fpsrc = fopen(filesrc, "rb");
    if (NULL == fpsrc)
    {
        result = 0;
        goto ERROR_PROCESS;
    }

    fpdst = fopen(filedst, "wb");
    if (NULL == fpdst)
    {
        result = 0;
        goto ERROR_PROCESS;
    }

    //get file size, 
    fseek(fpsrc, 0, SEEK_END);
    lnfsize = ftell(fpsrc);
    if (-1L == lnfsize)
    {
        result = 0;
        goto ERROR_PROCESS;
    }

    //restore to the beginning
    fseek(fpsrc, 0, SEEK_SET);

    //read 4 bytes every time.
    ncount  = lnfsize >> 2; //ncount  = lnfsize / sizeof(int);
    nremain = lnfsize & 3;              //nremain = lnfsize % sizeof(int);
    
    //read and write now every 4 bytes
    for (i = 0; i < ncount; i++)
    {
        result = operatefile(buf, sizeof(int), 1, fpsrc, fread, szerrorinfo);
        if (0 == result)
        {
            printf(szerrorinfo);
            goto ERROR_PROCESS;
        }

        result = operatefile(buf, sizeof(int), 1, fpdst, fwrite, szerrorinfo);
        if (0 == result)
        {
            printf(szerrorinfo);
            goto ERROR_PROCESS;
        }
    }

    //deal with the remaining
    if (0 == nremain)
    {
        goto ERROR_PROCESS;
    }

    result = operatefile(buf, nremain, 1, fpsrc, fread, szerrorinfo);
    if (0 == result)
    {
        printf(szerrorinfo);
        goto ERROR_PROCESS;
    }

    result = operatefile(buf, nremain, 1, fpdst, fwrite, szerrorinfo);
    if (0 == result)
    {
        printf(szerrorinfo);
        goto ERROR_PROCESS;
    }

ERROR_PROCESS:
    if (fpsrc)
    {
        fclose(fpsrc);
        fpsrc = NULL;
    }

    if (fpdst)
    {
        fclose(fpdst);
        fpdst = NULL;
    }

    return result;
}

/*
Function: 拷贝文件，每4byte进行

Remark  : 利用ferror, perror 对错误处理进行了改进
*/
int
mycopy_v2(char *filesrc, char *filedst)
{
    int result       = SUCCESS;
    FILE *fpsrc      = NULL;
    FILE *fpdst      = NULL;
    long lnfsize    =  0;
    unsigned ncount  = 0;
    unsigned nremain = 0;
    char buf[8]      = {0};
    unsigned i       = 0;
    
    fpsrc = fopen(filesrc, "rb");
    if (NULL == fpsrc)
    {
        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
    fpdst = fopen(filedst, "wb");
    if (NULL == fpdst)
    {
        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
    //get file size, 
    fseek(fpsrc, 0, SEEK_END);
    if (ferror(fpsrc))
    {
        perror("mycopy_v2 fseek error");
        
        result = FAILED;
        goto V2_ERROR_PROCESS;
    }

    lnfsize = ftell(fpsrc);
    if (-1L == lnfsize)
    {
        result = 0;
        goto V2_ERROR_PROCESS;
    }
    
    //restore to the beginning
    fseek(fpsrc, 0, SEEK_SET);
    
    //read 4 bytes every time.
    ncount  = lnfsize >> 2;     //ncount  = lnfsize / sizeof(int);
    nremain = lnfsize & 3;      //nremain = lnfsize % sizeof(int);
    
    //read and write now every 4 bytes
    for (i = 0; i < ncount; i++)
    {
        fread(buf, sizeof(int), 1, fpsrc);
        if (ferror(fpsrc))
        {
            perror("Read Error When Copy 4 bytes");

            result = FAILED;
            goto V2_ERROR_PROCESS;
        }
        
        fwrite(buf, sizeof(int), 1, fpdst);
        if (ferror(fpdst))
        {
            perror("Write Error When Copy 4 bytes");

            result = FAILED;
            goto V2_ERROR_PROCESS;
        }
    }//end for copy 4bytes everytime
    
    //deal with the remaining
    if (0 == nremain)
    {
        goto V2_ERROR_PROCESS;
    }
    
    fread(buf, nremain, 1, fpsrc);
    if (ferror(fpsrc))
    {
        perror("Read Error When Copy remained bytes");

        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
    fwrite(buf, nremain, 1, fpdst);
    if (ferror(fpdst))
    {
        perror("Write Error When Write remained bytes");

        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
V2_ERROR_PROCESS:
    if (fpsrc)
    {
        fclose(fpsrc);
        fpsrc = NULL;
    }
    
    if (fpdst)
    {
        fclose(fpdst);
        fpdst = NULL;
    }
    
    return result;
}

int
filecopy(char *filesrc, char *filedst)
{
    int result = SUCCESS;

    result = mycopy_v2(filesrc, filedst);
    
    return result;
}

int
checkfile(char *szfile)
{
    FILE *fp   = NULL;
    int result = SUCCESS;
    
    fp = fopen(szfile, "rb+");
    if (NULL == fp)
    {
        //try to create
        fp = fopen(szfile, "wb+");
        if (NULL == fp)
        {
            result = NOT_EXIST;
        }
    }
    
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
}

int
storerecord(char *tablefile, void *record, size_t recordsize, int initflg)
{
    FILE *fp = NULL;
    int result = SUCCESS;
    
    if (1 == initflg)
    {
        fp = fopen(tablefile, "wb");  //reset the file
        goto STORERECORD_ERROR;
    }
    else
    {
        fp = fopen(tablefile, "rb+");
    }
    
    if (NULL == fp)
    {
        showinfo("storerecord fopen error");
        result = FAILED;
        return result;
    }
    
    result = fseek(fp, 0, SEEK_END);
    if (0 != result)
    {
        showinfo("storerecord fseek error");
        result = FAILED;
        goto STORERECORD_ERROR;
    }
    
    result = fwrite(record, recordsize, 1, fp);
    if (ferror(fp))
    {
        perror("storerecord fwrite error");
        result = FAILED;
        goto STORERECORD_ERROR;
    }
    
STORERECORD_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
}

int
saveaddtable(void)
{
    char szaddfile_bak[MAX_INPUT] = {0};
    unsigned long nrecords = 0;
    
    int initflg = 0;            //是否初次写入
    int result = SUCCESS;
    unsigned long i = 0;
    
    sprintf(szaddfile_bak, "%s_bak", g_szaddtablefile);
    
    //backup addfile
    result = filecopy(g_szaddtablefile, szaddfile_bak);
    if (FAILED == result)
    {
        showinfo("backup addtablefile error");
        return result;
    }
    
    //save addtable
    nrecords = g_add_table.nrecords;

    //clear the file
    initflg = 1;
    result = storerecord(g_szaddtablefile, g_add_table.records, sizeof(tag_addrecord), 
                        initflg); //初次写入
    if (FAILED == result)
    {
        showinfo("savetable storerecord add init error");
        return result;
    }
    initflg = 0;    
    
    for (i = 0; i < nrecords; i++)
    {
        result = storerecord(g_szaddtablefile, g_add_table.records + i, sizeof(tag_addrecord), 
                    initflg);   //追加
        if (FAILED == result)
        {
            showinfo("savetable storerecord add error");
            return result;
        }
    }
    
    return result;
}

int
savedeltable(void)
{
    char szdelfile_bak[MAX_INPUT] = {0};
    
    unsigned long nrecords = 0;
    
    int initflg = 0;            //是否初次写入
    int result = SUCCESS;
    unsigned long i = 0;
    
    sprintf(szdelfile_bak, "%s_bak", g_szdeltablefile);
    
    //backup delfile
    result = filecopy(g_szdeltablefile, szdelfile_bak);
    if (FAILED == result)
    {
        showinfo("backup deltablefile error");
        return result;
    }
    
    //save deltable
    nrecords = g_del_table.nrecords;
    
    initflg = 1;
    result = storerecord(g_szdeltablefile, g_del_table.records, sizeof(tag_delrecord), 
        initflg); //初次写入
    if (FAILED == result)
    {
        showinfo("savetable storerecord del init error");
        return result;
    }
    initflg = 0;    
    
    for (i = 0; i < nrecords; i++)
    {
        result = storerecord(g_szdeltablefile,g_del_table.records + i, sizeof(tag_delrecord), 
                            initflg); //追加
        if (FAILED == result)
        {
            showinfo("savetable storerecord del error");
            return result;
        }
    }
    
    return result;
}

int
loadtable(char *sztablefile, int tabletype)
{
    FILE *fp   = NULL;
    long size  = 0;     //文件大小
    long count = 0;     //文件中记录的数目
    int result = SUCCESS;
    void *precord = NULL;
    size_t recordsize = 0;

    if (ADD == tabletype)
    {
        recordsize = sizeof(tag_addrecord);
    }
    else if (DEL == tabletype)
    {
        recordsize = sizeof(tag_delrecord);
    }

    //make sure exist
    result = checkfile(sztablefile);
    if (NOT_EXIST == result)
    {
        return result;
    }

    //now load into memory
    fp = fopen(sztablefile, "rb");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }

    fseek(fp, 0, SEEK_END);
    if (ferror(fp))
    {
        perror("loadtable fseek error");

        result = FAILED;
        goto LOAD_ERROR;
    }

    size = ftell(fp);
    if (ferror(fp))
    {
        perror("loadtable ftell error");

        result = FAILED;
        goto LOAD_ERROR;
    }

    if (size % recordsize)
    {
        result = FAILED;

        //all right to do like this? with ui in logic ?????
        showinfo("loadtable: data is corrupted\r\n");
        goto LOAD_ERROR;
    }

    count = size / recordsize;
    precord = malloc(size);    //count * recordsize
    if (NULL == precord)
    {
        result = FAILED;

        showinfo("loadtable: malloc failed\r\n");
        goto LOAD_ERROR;
    }

    fseek(fp, 0, SEEK_SET);
    if (ferror(fp))
    {
        perror("loadtable fseek to begin error");
        result = FAILED;
        goto LOAD_ERROR;
    }

    if (ADD == tabletype)
    {
        g_add_table.nrecords = count;
        g_add_table.records   = (tag_addrecord *)precord;

        fread(g_add_table.records, size, 1, fp);
    }
    else if (DEL == tabletype)
    {
        g_del_table.nrecords = count;
        g_del_table.records   = (tag_delrecord *)precord;

        fread(g_del_table.records, size, 1, fp);
    }   

    if (ferror(fp))
    {
        perror("loadtable fread error");

        result = FAILED;
        goto LOAD_ERROR;
    }

LOAD_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    } 

    return result;
}

int
readbuf(char *buf, unsigned long offset, size_t size, char *szfile)
{
    FILE *fp = NULL;
    int result = SUCCESS;

    fp = fopen(szfile, "rb");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }
    
    fseek(fp, offset, SEEK_CUR);
    if (ferror(fp))
    {
        perror("readbuf fseek error");

        result = FAILED;
        goto READBUF_ERROR;
    }

    fread(buf, size, 1, fp);
    if (ferror(fp))
    {
        perror("readbuf fread error");

        result = FAILED;
        goto READBUF_ERROR;
    }

READBUF_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
}

int
getfilesize(long *filesize)
{
    FILE *fp    = NULL;
    unsigned long naddrecords = 0;
    unsigned long ndelrecords = 0;
    unsigned long addoffset   = 0;
    unsigned long deloffset   = 0;
    long size   = 0;
    long offset = 0;
    int result  = SUCCESS;
    
    fp = fopen(g_szinfofile, "rb");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }
    
    /*
    //This is not right, for we don't truncate the file when defrag
    fseek(fp, 0, SEEK_END);    
    */
    //因defrag，未进行truncate, 所以利用addtable和deltable最后一项纪录确认文件结束
    naddrecords = g_add_table.nrecords;
    ndelrecords = g_del_table.nrecords;
    if (0 != naddrecords)
    {
        addoffset = g_add_table.records[naddrecords- 1].ulstart
                    + g_add_table.records[naddrecords - 1].uchsize;

    }

    if (0 != ndelrecords)
    {
        deloffset = g_del_table.records[ndelrecords - 1].ulstart
            + g_del_table.records[ndelrecords - 1].uchsize;    
    }
    //取大者
    offset = (addoffset > deloffset ? addoffset : deloffset);       
    

    fseek(fp, offset, SEEK_CUR);
    if (ferror(fp))
    {
        perror("Getfilesize fseek error");
        
        result = FAILED;
        goto GETFILESIZE_ERROR;
    }

    size = ftell(fp);
    if (ferror(fp))
    {
        perror("Getfilesize ftell error");

        result = FAILED;
        goto GETFILESIZE_ERROR;
    }

    *filesize = size;

GETFILESIZE_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }

    return result;
}


#if 0
int
main(int argc, char *argv[])
{
    int result = 0;

    if (argc != 3)
    {
        usage();
        return -1;
    }

    //result = mycopy(argv[1], argv[2]); 
    result = mycopy_v2(argv[1], argv[2]); //对错误处理进行了改进
    if (1 == result)
    {
        printf("已成功复制文件\r\n");
    }
    else
    {
        printf("发生错误\r\n");
    }
    
    return 0;
}
#endif