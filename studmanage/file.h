#ifndef FILE_H_A9582592_6060_4472_A92F_3CD06A7500FC

#define FILE_H_A9582592_6060_4472_A92F_3CD06A7500FC

/*
Function: �ļ�������ÿ4�ֽڽ��У�

Param   : 

Return  : SUCCESS �ɹ������� FAILED ����ʧ��
*/
int
filecopy(char *filesrc, char *filedst);

/*
Function: ����ļ��Ƿ���ڣ� ����������ͼ�½���

Param   : szfile �ļ����� ��MAX_INPUT���ƣ�

Return  : SUCCESS ȷ�ϴ��ڻ����½��ɹ�
          NOT_EXIST �����ڣ��½�ʧ�ܣ�

Remark  : ����init�׶Σ������Ϣ�洢�ļ�����ӱ��ļ���ɾ�����ļ�
*/
int
checkfile(char *szfile);

/*
Function: ��tablefile��д��һ��record. ���������޹أ�

Param   : void *record ��Ҫд���record��Ϣ
          initflg  1 ��ʾ����д�룬�Ὣ֮ǰ��������ա� 0 ������ӡ�

Return  : SUCCESS �ɹ�д��  FAILED ʧ��
*/
int
storerecord(char *tablefile, void *record, size_t recordsize, int initflg);

/*
Function: ������ӱ����ڴ��е���ӱ�д�뵽��ӱ��ļ���

Return  : SUCCESS �ɹ����� FAILED ʧ��
*/
int
saveaddtable(void);

/*
Function: ����ɾ�������ڴ��е�ɾ����д�뵽ɾ�����ļ���

Return : SUCCESS �ɹ����� FAILED ʧ��
*/
int
savedeltable(void);

/*
Function: �ӷ�����ļ��м��ط��������������ͼ����

Param   : sztablefile����Ӧ������·������
          tabletype�Ƿ��������ͣ�ADD ����DEL

Return  : SUCCESS ���سɹ������½��ɹ�
          NOT_EXIST ��Ӧ�ļ�������
          FAILED  ����ʧ��

Remark  : 
*/
int
loadtable(char *sztablefile, int tabletype);

/*
Function: ��szfile��offset��ַ��ȡsize�ֽڵ�buf��

Return : SUCCESS �ɹ���ȡ�� FAILED ʧ��
*/
int
readbuf(char *buf, unsigned long offset, size_t size, char *szfile);

/*
Function: ������defragδ����truncate, 
        ����addtable, deltable�����һ����ȷ����Ϣ�洢�ļ��Ĵ�С��

Return : SUCCESS �ɹ�  FAILED ʧ��
*/
int
getfilesize(long *filesize);


#endif