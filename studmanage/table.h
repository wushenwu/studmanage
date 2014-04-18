#ifndef  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889
#define  TABLE_H_6AEDF8D6_CF88_44ce_9E06_CCF5E6129889


/*
Function: �������޹صĲ�������

Param   : IN OUT unsigned long *nrecords,         //���봫����table��record������
          void *records,                          //table��record����ʼ��ַ
          size_t recordsize,                      //record�Ĵ�С 
          IN OUT int *index,                      //���ղ����λ��
          int (*compare)(void *, void *)          //�Ƚϼ�¼��С�Ļص�����

Remark  : insertintoaddtable, insertintodeltable��ʵ��ͨ���˺���

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
Function: ��addrecord�����(start, size, namesize)��¼����Ĳ��뵽**addtable��
           start��Ϊkeywords
����  
Param	:  addtable, addrecord
����        
����        
Return  :  None.

Effect  : ע�⣬��Ϊ���ǵ�g_add_table�Ǹ�����Ҫ�����ģ����ԣ�Ҫrealloc���е�����
          ע������Ĵ���
*/
void
insertintoaddtable(tag_addtable **addtable, struct tag_addrecord *addrecord);


/*
Function: ��delrecord�����(start, size)��¼����Ĳ��뵽**deltable��, 
            ����g_del_tableָʾ�������пռ��������
����  
Param	:  deltable, delrecord
����        
Return  :  None.

Effect   : ����g_del_table, ��g_del_tableָʾ���������пռ�������ϣ������ٷ���
            ע�⣬��Ϊ���ǵ�g_del_table�ǰ��������ģ�������Ҫrealloc, 
            ע������Ĵ���
*/
void
insertintodeltable(tag_deltable **deltable, struct tag_delrecord *delrecord);


/*
Function: �������޹صı�����ɾ������

Param   : IN OUT unsigned long *nrecords,     //���봫����table��record������
          void *records,                      //table��records����ʼ��ַ
          size_t recordsize,                  //record�Ĵ�С
          int index                           //Ҫɾ����record��table�е��±�
*/
void
removefromtable(unsigned long *nrecords,     //���봫����table��record������
                void *records,                      //table��records����ʼ��ַ
                size_t recordsize,                  //record�Ĵ�С
                int index                           //Ҫɾ����record��table�е��±�
                );

/*
Function: ��index ����ʶ�ļ�¼��addtable��ɾ��, ����֤addtable�������ԡ�
����  
Param	: addtable, index
����        
Return  :  None.

Effect  : ����addtable;
           ע�⣬��Ϊrealloc�Ա���вü���������Ҫע�⴫��Ĳ�����
*/
void
removefromaddtable(tag_addtable **addtable, int index);

/*
Function: ��index ����ʶ�ļ�¼��deltable��ɾ��, ����֤deltable�������ԡ�
����  
Param	: deltable, index
����        
Return  :  None.

Effect  : ����deltable;
           ע�⣬��Ϊrealloc�Ա���вü���������Ҫע�⴫��Ĳ�����
*/
void
removefromdeltable(tag_deltable **deltable, int index);

/*
Function: �������޹صı��м�¼���²����������¼���£�

Param   :  void *records,          //����records��ʼ��ַ
            size_t recordsize,      //����record�Ĵ�С
            int index,              //Ҫ���µ�record
            void *newrecord         //������

*/
void
updatetable(void *records,          
            size_t recordsize,      
            int index,              
            void *newrecord         
            );

/*
Function: ��addtable����Ӧindex����ʶ�ļ�¼����Ϊ�´����addrecord��
����  
Param	: 
����        
Return  :  None.

Effect  :  ����addtable
*/
void
updateaddtable(tag_addtable *addtable, int index, tag_addrecord *addrecord);


/*
Function: ��deltable����Ӧindex����ʶ�ļ�¼����Ϊ�´����delrecord��
����  
Param	: 
����        
Return  :  None.

Effect  :  ����deltable
*/
void
updatedeltable(tag_deltable *deltable, int index, tag_delrecord *delrecord);


/*
Function: get the table that index belongs to, and set the max value of this range.
           ����space_stat ������ʹ�ñ�־U, F

Param   : buf is the storage buf.
          index_add, index_del Ϊspace_stat�������Ĳ�����
          ����Ŀǰ��g_add_del, g_del_table�����������±ꡣ
          NO_USE_MALLOC, USE_MALLOC_V1�к�Ϊ(0,0)

*/
int
gettable(unsigned long index, int *max, 
         unsigned long index_add, unsigned long index_del);


#endif