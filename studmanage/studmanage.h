#ifndef MEMMANAGE_H_CF771D01_FB96_4a42_BA47_C7B9C76D8B18    //��ֹͷ�ļ����ظ�����

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

//��ַ���ͣ���ԣ����ԣ�
#define     RELATIVE                0x00
#define     ABSOLUTE                0x01


//for memory space statistics
#define     SPLIT                   8

/*
����ԭ��memmanage�Ĵ�����ʵ�ֶ�ѧ����Ϣ�洢�Ĺ���
*/
#pragma pack(push)
#pragma pack(1)

//ѧ����Ϣ��ʽ(gender, birthday, score, �䳤�������� �䳤��ѧ��)
typedef struct tag_student
{
    char           chgender;    //
    unsigned short ushbirth;    //mmdd
    float          fscore;  
    /*
    ��������ı䳤��������ѧ����Ϣ
    */
}tag_student;

//��ӱ��м�¼�ĸ�ʽ(start, totalsize, namesize)
typedef struct tag_addrecord
{
    unsigned long ulstart;      //ѧ����Ϣ���ļ��е�ƫ��
    unsigned char uchsize;      //ѧ����Ϣռ�õ��ܿռ�
    unsigned char uchnamesize;  //ѧ�������ĳ���
                                //ѧ��ѧ�ų��� = size - namesize - sizeof(char)
                                //               - sizeof(short) - sizeof(float)
}tag_addrecord;

//��ӱ�
typedef struct tag_addtable
{
    unsigned long nrecords;             
    tag_addrecord *records;  //�䳤����������Ҫʵʱ����
}tag_addtable;

//ɾ�����м�¼�ĸ�ʽ(start, totalsize)
typedef struct tag_delrecord
{
    unsigned long ulstart;
    unsigned char uchsize;
}tag_delrecord;
 
//ɾ����
typedef struct tag_deltable
{
    unsigned long nrecords;
    tag_delrecord *records;   //�䳤����������Ҫʵʱ����
}tag_deltable;

//�Ƿ��ṩ�ֶ�����Ϣ
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


/*���ᳫʹ��ȫ�ֱ����� ������Ҫ��ͷ�ļ���ʹ��extern int value ��������*/

/*used for maintain*/
extern struct tag_addtable g_add_table;
extern struct tag_deltable g_del_table;

extern char g_szinfofile[MAX_INPUT];
extern char g_szaddtablefile[MAX_INPUT];
extern char g_szdeltablefile[MAX_INPUT];



/*
Function: ϵͳ��ʼ�������ѧ����Ϣ�洢�ļ��Ƿ���ڣ�������Ƿ���ڡ�
          ������������½������������ص��ڴ���
Return  : SUCCESS �ɹ���ʼ��
          FAILED  ��ʼ��ʧ��
*/
int
init(void);

/*
Function: ϵͳ�˳�ǰ�Ĵ������ݷ�����ļ������ڴ��еķ����д�뵽�ļ���
*/
int
finish(void);

/*
Function: ������Ϣid���в�ѯ��ȷ���Ƿ�������ѧ����
          ��Ҫ����ѧ����Ϣ���޸ġ�ɾ�������� 
����
Param	: ���id, ��id�Ǹ����ִ�ѧ����¼����̬�仯�ģ���Ҫ�û�����ģ����ѯ�������ȷ�ϡ�
           (���Ψһ)
           
Return  :  NOT_EXIST�����ڣ� SUCCESS����,
*/
int 
info_query_by_id(unsigned long infoid);

/*
Function: ���û������ѧ����Ϣ����Ž���ģ����ѯ�����ϲ�ѯ��ȷ�����ѧ�����

Param   : tag_student *student, char *szname, char *szno,    //Ҫ��ѯ��ѧ����Ϣ
          unsigned long id,                                  //Ҫ��ѯ�ı��
          int *indexary,                                     //���ղ�ѯ������Ŀ��ţ��±꣩
          int *num                                           //��ѯƥ�䵽������Ŀ
          tag_newinfo *flag                                  //��Ӧ�ֶ��Ƿ����������

Return  : SUCCESS���ڣ�������indexary.
          NOT_EXIST �����ڣ� 
          FAILED  ��ѯʧ��
          
*/
int
info_query_about(tag_student *student, char *szname, char *szno,    //Ҫ��ѯ��ѧ����Ϣ
                 unsigned long id,                                  //Ҫ��ѯ�ı��
                 int *indexary,                                     //���ղ�ѯ������Ŀ��ţ��±꣩
                 int *num,                                           //��ѯƥ�䵽������Ŀ
                 tag_newinfo *flag
                 );


/*
Function: ���û������ѧ����Ϣ��ӽ��ļ��С�
����  
Param	: student�д����ѧ��������Ϣ��szname, sznoΪ�䳤��������ѧ��
����        
Return  :  NO_SPACE ���ʧ�ܣ� ������ʾ��ӵ�λ��.

Effect  : ����g_add_table, g_del_table


  info_add(student, szname, szno):
  ����sizereal = ��ѧ��ȫ����Ϣ����
  �������û�д�g_del_table�в�ѯ�����ʣ� �ں����������(start��size)��¼��
        �����ļ�ĩβ�����Ϣ������g_add_table��׷��(start, size, namesize)��¼��
  ����
  ��������g_del_table�в�ѯ��һ�����ʵģ��ں������) size�ļ�¼(start, start)��
        ��ѧ����Ϣ������startָʾ���ļ�ƫ�ƴ���
        ����sizereal��size�Ĺ�ϵ�������¼��������
  ����  sizereal = size, ��(start, start)��g_del_table��ɾ����
                        ͬʱ��(start, sizereal, namesize)����Ĳ��뵽g_add_table�� (start��Ϊkeyword)
        sizereal < size, ��g_del_table�еĸ�(start, size)��Ϊ(start + sizereal, size - sizereal), 
                        ͬʱ����(start, sizereal, namesize)����Ĳ��뵽g_add_table�С�

*/
int
info_add(tag_student *student, char *szname, char *szno);

/*
Function: ɾ��ָ����ŵ�ѧ����Ϣ
����  
Param	: infoidΪ��ţ���Ϣ��ţ���g_add_table�ı仯���仯�����û�����ģ����ѯȷ�ϡ�
����        
Return  :  NOT_EXIST for failed�� SUCCESS �ɹ�.

Effect  : ����g_add_table, 
           ����g_del_table�� ��������Կ��пռ�������ϣ�Ӱ��g_del_table

info_del(infoid):��
        ���ü�¼��g_add_table��ɾ����
            ͬʱ���Ǹ��������g_del_table. 
        ���ǵ����ܵ��������пռ�����ϣ���Ϊ���������
����    ����к��ʵ�ǰ��(size1, start1) ���� start1 + size1 = size, ������������пռ���������Ӧ��(size1, start1)����Ϊ(size1+size, start1)
����    ���(size,start)�к��ʵĺ��(size2, start2)���� start + size = start2, ��(size2, start2)Ӧ����Ϊ(size+size2, start)
*/
int
info_del(int infoid);

/*
Function: �޸�ָ����ŵ�ѧ����Ϣ
����  
Param	: tag_student *student, char *szname, char *szno,  Ϊ�µ���Ϣ
            int infoid ָ�����
            tag_newinfo *flag  ָʾ���ֶ��Ƿ�����
����        
Return  :  NOT_EXIST for not exist, NO_SPACE for no space, SUCCESS for success

Effect  : ����g_add_table, 
           ����g_del_table�� ��������Կ��пռ�������ϣ�Ӱ��g_del_table

info_modify(origin, new):
����index = info_query(origin)
�������index == -1 ����������origin, �޸�ʧ�ܡ�
����oldsize = len(origin)
����newsize = len(new)
�������index != -1, ����ʾorigin��(size, start)��g_add_table�е��±꣬ȡ��start.
��������oldsize��newsize�Ĵ�С����Ϊ���������
������� newsize < oldsize, �����g_add_table�е�(size, start) Ϊ(newsize, start), ͬʱ��g_del_table������Ĳ���(oldsize - newsize, start + newsize)
������� newsize = oldsize, �򲻷������
�������newsize > oldsize, ���ʱ���Լ򵥵��õ�info_del(origin)��info_add(string), ��ɿռ���ͷź������ϣ��Լ��ٷ��䡣
����Ȼ��Ҳ���ܳ��ָ�startǰ�����ߺ�̱����Ѿ�Ϊ���е��������ô��ʱ���п��е���������õģ��������ǻ��ǽ�������һ������̽��д�����
*/
int
info_modify(tag_student *student, char *szname, char *szno, int infoid, tag_newinfo *flag);


/*
Function: ��chInput[]�洢��chBuf[start]
����  
Param	 : chBuf, start, chInput, requestsize          
����        
Return  :  NO_SPACE for failed, SUCESS for success

Effect   : ����chBuf[],������Ҫ����g_indexMAX_buf
*/
int
store(char *chBuf, int start, char *chInput, int requestsize);

/*
Function: ��student, szname, sznoָ����ѧ����Ϣ������ļ���

Param   : tag_student *student, char *szname, char *szno,   //ѧ����Ϣ
          unsigned long start,                              //���봫�����ļ���ַ
          unsigned char namesize ,                          //ѧ������ռ�ÿռ��С
          unsigned char nosize                              //ѧ��ѧ��ռ�ÿռ��С

Return  : SUCCESS �ɹ��洢
          FAILED  �洢ʧ��
*/
int
storeinfo(tag_student *student, char *szname, char *szno,   
          unsigned long *start,                              
          unsigned char namesize ,                          
          unsigned char nosize
      );

/*
Function: for USE_MALLOC_V2, USE_MALLOC_V3, store szinput into space malloced.

Param   : ppszstart ���ڽ���malloc�ĵ�ַ��
*/
int
store_by_malloc(char *szinput, char **ppszstart);

/*
Function: ����g_add_table, ���g_chBuf, ������д洢���ַ������롣
����  
Param	 : None         
����        
Return  : None.

Effect  : None
*/
void
display(void);


/*
Function: ͳ��startaddr��ָʾ�Ĵ�СΪsize�������ռ��и���ַ��ʹ�������
����  
Param	 : unsigned long startaddr ������ַ�ռ����ʼ��ַ��
            ��Ϊ��Ե�ַ�����ļ���ַ����Ҳ����Ϊ���Ե�ַ����malloc���õ�ַ��
           relativeaddrflg ��Ե�ַ��־ ��RELATIVE Ϊ��ԣ�ABSOLUTEΪ���ԣ�
           
            ��Ȼ����������Ҳ��Ҫ������ָ�������ļ���szfile, �����ơ�
����        
Return  : SUCCESS �ɹ�  FAILED ʧ��
Remark  : ��space_stat_for_file���е���
*/
int
space_stat(unsigned long startaddr, unsigned long size, int relativeaddrflg, char *szfile);

/*
Function: ���ļ��ռ��ַʹ�������ͳ��
          �����space_stat��
*/
int
space_stat_for_file(void);

/*
Function: ������Ƭ����
*/
void
defrag(void);

/*
Function: ͳ�Ƹ��ַ����ֵĴ�����Ƶ��
*/
void
char_stat(void);

/*
Function: ���szText[]��action, ����g_char_table�и��ַ����ֵĴ���

        called whenever info_add, info_del, info_modify success.
*/
static void
updatechar(char *szText, int action);

/*
Function: ��USE_MALLOC_V2��USE_MALLOC_V3�е�g_add_table, g_del_table��ά���Ŀռ�����ͷš�
*/
void
free_space(void);


/*
Function: ����ӱ��¼ָ����λ�ô�ȡ��ѧ����Ϣ���ֶ�

Param   : tag_student *student, char *szname, char *szno,  //���ն�ȡ��ѧ����Ϣ
          tag_addrecord *addrecord                          //

Return  : SUCCESS �ɹ���ȡ
          FAILED  ��ȡʧ��
*/
int
fetch1info(tag_student *student, char *szname, char *szno,  //���ն�ȡ��ѧ����Ϣ
           tag_addrecord *addrecord                         // Ҫ��ȡ��ѧ����Ϣ       
           );

/*
Function: ��ָ����λ�ô�ȡ��������ѧ����Ϣ

Param   : void *infobuf,             //���ն�ȡ��ѧ��������Ϣ
          unsigned long ulstart,     //ָ���ĵ�ַ
          unsigned char uchsize      //ָ���Ĵ�С

Return  : SUCCESS �ɹ���ȡ
          FAILED  ��ȡʧ��
*/
int
fetch1info_whole(void *infobuf,             //���ն�ȡ��ѧ��������Ϣ
                 unsigned long ulstart,     //
                 unsigned char uchsize
                 );

/*
Function: ͳ�Ƹ�ѧ����Ϣռ�ÿռ��С�������ļ��ܴ�Сռ�ȡ�
*/
void
info_stat(void);

#endif