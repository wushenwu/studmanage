#include <stdio.h>
#include "studmanage.h"
#include "fit.h"

//the way you can get suitable size, can only be dealed within this file
static int g_fitWAY = FIT_EQUAL;

int 
fitequal(unsigned char requestsize, int *index)
{
    int result = NOT_EXIST;
    unsigned long i      = 0;

    for (i = 0; i < g_del_table.nrecords; i++)
    {
        if (requestsize == g_del_table.records[i].uchsize)
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

int 
fitenough(unsigned char requestsize, int *index)
{
    int result = NOT_EXIST;
    int min    = -1;
    unsigned long i      = 0;

    for (i = 0, min = -1; i < g_del_table.nrecords; i++)
    {
        if (requestsize <= g_del_table.records[i].uchsize)
        {
            if ((-1 == min) 
                || (g_del_table.records[i].uchsize < g_del_table.records[min].uchsize)
                )
            {
                min = i;
            }
        }
    }

    if (-1 != min)
    {
        *index = min;
        result = SUCCESS;
    }

    return result;
}

int 
fitfirst(unsigned char requestsize, int *index)
{
    int result = NOT_EXIST;
    unsigned long i      = 0;

    for (i = 0; i < g_del_table.nrecords; i++)
    {
        if (requestsize <= g_del_table.records[i].uchsize)
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

int 
getsize(unsigned char requestsize, int *index)
{
    int result = NOT_EXIST;

    switch (g_fitWAY)
    {
    case FIT_EQUAL:
        {
            result = fitequal(requestsize, index);
            break;
        }       

    case FIT_ENOUGH:
        {
            result = fitenough(requestsize, index);
            break;
        }        

    case FIT_FIRST:
        {
            result = fitfirst(requestsize, index);
            break;
        }        

    case FIT_EQUAL_ENOUGH:
        {
            result = fitequal(requestsize, index);
            if (NOT_EXIST == result)
            {
                result = fitenough(requestsize, index);
            }
            break;
        }        

    case FIT_EQUAL_FIRST:
        {
            result = fitequal(requestsize, index);
            if (NOT_EXIST == result)
            {
                result = fitfirst(requestsize, index);
            }
            break;
        }        

    default:
        {
            //sth wrong ?
            result = NOT_EXIST;
            break;
        }        
    }

    return result;
}

void
setfit(int nselect)
{
   g_fitWAY = nselect;
}