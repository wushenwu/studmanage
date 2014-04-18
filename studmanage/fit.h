#ifndef FIT_H_7D199A32_98B9_4858_A226_045DC71B5CF7

#define FIT_H_7D199A32_98B9_4858_A226_045DC71B5CF7

//different ways you can use to get suitable size
#define		FIT_EQUAL				0
#define		FIT_FIRST				1
#define 	FIT_ENOUGH			    2
#define		FIT_EQUAL_ENOUGH	    3
#define		FIT_EQUAL_FIRST		    4


/*
Function: 从g_del_table中找到一个(start, size)记录，使得size = requestsize
Param	: requestsize 是想申请的空间大小， 
　　      index用于接收符合条件(start, size) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS 成功，并设置index.
*/
int 
fitequal(unsigned char requestsize, int *index);

/*
Function: 从g_del_table中找到一个(start, size)记录，使得size 刚好>= 
　　       requestsize，
Param	: requestsize 是想申请的空间大小， 
          index用于接收符合条件(start, size) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS 成功，并设置index.
*/
int 
fitenough(unsigned char requestsize, int *index);

/*
Function: 从g_del_table中找到一个(start, size)记录，使得size 是第一个>= 
　　        requestsize，
Param	: requestsize 是想申请的空间大小， 
          index用于接收符合条件(start, size) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS 成功，并设置index.
*/
int 
fitfirst(unsigned char requestsize, int *index);

/*
Function: 根据g_fitWAY，从g_del_table中找到一个合适的(start, size)记录，
　　      是对上面fitxxx函数的封装。
　　 
Param	: requestsize 是想申请的空间大小， 
          index用于接收符合条件(start, size) 在g_del_table中的下标
Return  :  NOT_EXIST 失败， SUCCESS 成功，并设置index.
*/
int 
getsize(unsigned char requestsize, int *index);

/*
Function: set the default way according to your input
*/
void
setfit(int select);

#endif