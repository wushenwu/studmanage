#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "studmanage.h"
#include "table.h"
#include "ui.h"

#define ADDR_RECORD(records, i, recordsize)   ((void *)((int)(records) + (i) * (recordsize)))

int
compare_addrecord(void *record, void *addrecord)
{
    unsigned long x = 0, y = 0;

    x = ((tag_addrecord *)record)->ulstart;
    y = ((tag_addrecord *)addrecord)->ulstart;

    return x - y;
}

int
compare_delrecord(void *record, void *delrecord)
{
    unsigned long x = 0, y = 0;

    x = ((tag_delrecord *)record)->ulstart;
    y = ((tag_delrecord *)delrecord)->ulstart;

    return x - y;
}

void
insertintotable(unsigned long *nrecords,         //���봫����table��record������
                void *records,                          //table��record����ʼ��ַ
                void *newrecord,                        //Ҫ�����record
                size_t recordsize,                      //record�Ĵ�С 
                int *index,                      //���ղ����λ��
                int (*compare)(void *, void *)          //�Ƚϼ�¼��С�Ļص�����
                )
                
{
    int i = 0;
    int j = 0;

    //find the proper position
    for (i = *nrecords; i >= 1; i--)
    {
        //i will be the insert point.
        if (0 > compare( //(void *)((int)records + (i-1) * recordsize),
                        ADDR_RECORD(records, i-1, recordsize), 
                        newrecord) 
            )                   
        {
            break;
        }
    }
    
    //now we need to move i, i+1, .. nrecords-1 to i+1, i+2,...nrecords
    for (j = *nrecords; j >= i + 1; j--)
    {
        memcpy(//(void *)((int)records + j * recordsize), 
               ADDR_RECORD(records, j, recordsize),
               //(void *)((int)records + (j-1) * recordsize),
               ADDR_RECORD(records, j - 1, recordsize),
               recordsize);      
    }
    
    //now insert this record to i
    memcpy(//(void *)((int)records + i * recordsize), 
           ADDR_RECORD(records, i, recordsize),
            newrecord, recordsize);

    //update data
    *index = i;
    *nrecords = *nrecords + 1;
}

/*
0 1 2 3 ....i-1 i(the insert point) i+1 i+2....
*/
void
insertintoaddtable(tag_addtable **addtable, struct tag_addrecord *addrecord)
{
    size_t newsize = 0;
    int index      = 0;

    newsize = sizeof((**addtable).nrecords)                          //
              + ((*addtable)->nrecords + 1) * sizeof(tag_addrecord); // ����1��records


    (*addtable)->records = (tag_addrecord *)realloc((*addtable)->records, 
                                                     newsize
                                                    );

    if (NULL == *addtable)
    {
        showinfo("Insertintoaddtable realloc error\r\n");
    }

    insertintotable(&(*addtable)->nrecords,
                     (*addtable)->records,
                     addrecord,
                     sizeof(tag_addrecord),
                     &index,
                     compare_addrecord          //�ص�����
                    );

#if 0
    int i = 0;
    int j = 0;

    //find the proper position
    for (i = addtable->nrecords; i >= 1; i--)
    {
        //i will be the insert point.
        if (addtable->records[i - 1].start <= addrecord->start)
        {
            break;
        }
    }

    //now we need to move i, i+1, .. nrecords-1 to i+1, i+2,...nrecords
    for (j = addtable->nrecords; j >= i + 1; j--)
    {
        memcpy(&addtable->records[j], &addtable->records[j - 1], sizeof(tag_addrecord));      
    }

    //now insert this record to i
    memcpy(&addtable->records[i], addrecord, sizeof(tag_addrecord));

    addtable->nrecords++;
#endif
}

/*
Function: ����g_del_table�У�index��¼����ǰ��������Ƿ�Ϊ�����Ŀ��пռ䣬�������ϣ�
           �Ի�ȡ����Ŀ��пռ䣬���ں����ٷ���
Param   : index

Return  : None

Effect  : ���ܵ���removefromtable��

Caller: this function can only be called by insertintodeltable
*/
static void
reorganize(unsigned long index)
{
    unsigned long indexpre = 0;
    unsigned char sizepre  = 0;
    unsigned long startpre = 0;

    unsigned long indexaft = 0; 
    unsigned char  sizeaft  = 0;
    unsigned long  startaft = 0;

    unsigned char size     = 0;
    unsigned long start    = 0;

    tag_delrecord delrecord;
    tag_deltable *pdeltable = NULL;
     
    size  = g_del_table.records[index].uchsize;
    start = g_del_table.records[index].ulstart;   
    
    //aft
    if ((index + 1) != g_del_table.nrecords)
    {
        indexaft = index + 1;
        sizeaft  = g_del_table.records[indexaft].uchsize;
        startaft = g_del_table.records[indexaft].ulstart;

        if (startaft == start + size)
        {
            delrecord.uchsize  = size + sizeaft;
            delrecord.ulstart  = start;
            updatedeltable(&g_del_table, index, &delrecord);
            pdeltable = &g_del_table;
            removefromdeltable(&pdeltable, indexaft);            
            //showinfo("reorganize space aft happens\r\n");
        }
    }//end for aft
    
    //pre
    if (index != 0)
    {
        indexpre = index - 1;  
        sizepre  = g_del_table.records[indexpre].uchsize;
        startpre = g_del_table.records[indexpre].ulstart;
        
        if (start == startpre + sizepre)
        {
            //take care, may size already updated, we need to use g_del_table[index]
            delrecord.uchsize  = sizepre + g_del_table.records[index].uchsize;
            delrecord.ulstart  = startpre;
            updatedeltable(&g_del_table, indexpre, &delrecord);
            pdeltable = &g_del_table;
            removefromdeltable(&pdeltable, index);
            //showinfo("reorganize space pre happens\r\n");
        }   
    }//end for pre 
}

void
insertintodeltable(tag_deltable **deltable, struct tag_delrecord *delrecord)
{
    size_t newsize = 0;
    int index      = 0;

    newsize = sizeof((**deltable).nrecords)
                + ((*deltable)->nrecords + 1) * sizeof(tag_delrecord); //����1��records

    (*deltable)->records = (tag_delrecord *)realloc((*deltable)->records, 
                                            newsize
                                            );
    
    if (NULL == *deltable)
    {
        //sth wrong
        showinfo("Insertintodeltable realloc error\r\n");
    }

    insertintotable(&(*deltable)->nrecords,
                    (*deltable)->records,
                    delrecord,
                    sizeof(tag_delrecord),
                    &index,
                    compare_delrecord
                    );

    reorganize(index);

#if 0
    unsigned long i = 0;
    unsigned long j = 0;
    
    //find the proper position
    for (i = deltable->nrecords; i >= 1; i--)
    {
        if (deltable->records[i - 1].start <= delrecord->start)
        {
            break;
        }
    }
    
     //now we need to move i, i+1, .. nrecords-1 to i+1, i+2,...nrecords
    for (j = deltable->nrecords; j >= i + 1; j--)
    {
        memcpy(&deltable->records[j], &deltable->records[j - 1], sizeof(tag_delrecord));      
    }
    
    //now insert this record
    memcpy(&deltable->records[i], delrecord, sizeof(tag_delrecord));

    deltable->nrecords++;

    reorganize(i);
#endif
}

void
removefromtable(unsigned long *nrecords,     //���봫����table��record������
                void *records,                      //table��records����ʼ��ַ
                size_t recordsize,                  //record�Ĵ�С
                int index                           //Ҫɾ����record��table�е��±�
                )
{
    unsigned long i = 0;
        
    //move index+1, index+2, ..., nrecords-1 to index, index+1, ..., nrecords-2
    for (i = index; i < *nrecords - 1; i++)
    {
        memcpy(ADDR_RECORD(records, i, recordsize),
               ADDR_RECORD(records, i+1, recordsize),
                recordsize);
    }
    
    memset(ADDR_RECORD(records, *nrecords-1, recordsize), //records[*nrecords - 1], 
        0, recordsize);
    
    *nrecords = *nrecords - 1;
}

void
removefromaddtable(tag_addtable **addtable, int index)
{
    size_t newsize = 0;

    removefromtable(&(*addtable)->nrecords,
                    (*addtable)->records,
                    sizeof(tag_addrecord),
                    index
                    );

    //table nrecords already updated,  -1
    newsize = sizeof((**addtable).nrecords)
              + (*addtable)->nrecords * sizeof(tag_addrecord);

    (*addtable)->records = (tag_addrecord *)realloc((*addtable)->records,
                                        newsize  
                                        );

    if (NULL == *addtable)
    {
        showinfo("Removefromaddtable realloc error\r\n");
    }
#if 0
    int nrecords = 0;
    int i        = 0;

    nrecords = table->nrecords;

    //move index+1, index+2, ..., nrecords-1 to index, index+1, ..., nrecords-2
    for (i = index; i < nrecords - 1; i++)
    {
        memcpy(&table->records[i], &table->records[i+1], sizeof(struct tag_record));
    }
   
    memset(&table->records[nrecords - 1], 0, sizeof(struct tag_record));

    table->nrecords--;
#endif
}


void
removefromdeltable(tag_deltable **deltable, int index)
{
    size_t newsize = 0;

    removefromtable(&(*deltable)->nrecords,
                    (*deltable)->records,
                    sizeof(tag_delrecord),
                    index
                    );

    //deltable nrecords already updated, -1
    newsize = sizeof((**deltable).nrecords)
              + (*deltable)->nrecords * sizeof(tag_delrecord);

    (*deltable)->records = (tag_delrecord *)realloc((*deltable)->records,
                                        newsize
                                        );
    if (NULL == *deltable)
    {
        showinfo("Removefromdeltable realloc error\r\n");
    }
}

void
updatetable(void *records,
            size_t recordsize,
            int index,
            void *newrecord)
{
    memcpy(ADDR_RECORD(records, index, recordsize), //records[index],
            newrecord, recordsize);
}

void
updateaddtable(tag_addtable *addtable, int index, tag_addrecord *addrecord)
{
    updatetable(addtable->records,
                sizeof(tag_addrecord),
                index,
                addrecord
                );
}

void
updatedeltable(tag_deltable *deltable, int index, tag_delrecord *delrecord)
{
    updatetable(deltable->records,
                sizeof(tag_delrecord),
                index,
                delrecord
                );
}

#if 0
void
updatetable(struct tag_table *table, int index, struct tag_record *record)
{
    table->records[index].size  = record->size;
    table->records[index].start = record->start;
}
#endif


int
gettable(unsigned long index, int *max, unsigned long index_add, unsigned long index_del)
{
    unsigned char size_add  = 0;
    unsigned long start_add = 0;
    unsigned char size_del  = 0;
    unsigned long start_del = 0;
    int table               = 0;

    if (index_add != g_add_table.nrecords)
    {
        size_add  = g_add_table.records[index_add].uchsize;
        start_add = g_add_table.records[index_add].ulstart;
    }
        
    if (index_del != g_del_table.nrecords)
    {
        size_del  = g_del_table.records[index_del].uchsize;
        start_del = g_del_table.records[index_del].ulstart;
    }

    if ((index >= start_add) && (index < start_add + size_add))
    {
        *max = start_add + size_add;
        return ADD;
    }

    if ((index >= start_del) && (index <= start_del + size_del))
    {
        *max = start_del + size_del;
        return DEL;
    }

    //should not come to this point.
    return table;
}