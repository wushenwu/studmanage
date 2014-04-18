#include <stdio.h>
#include "ui.h"

char g_gender[4] = {'M',       //Male
                    'F',       //Female
                    'U',       //Unknown
                    'O'        //Other
};

char g_gender_desc[4][8] = { "Male",
                            "Female",
                             "Unknown",
                             "Other"                        
};


void
showmenu_en(void)
{
    showinfo("This is the menu for student management:  \r\n"
        "--------------------------------------------   \r\n"
        "1. Add      : store one student info           \r\n"
        "2. Query    : query student info by info or id \r\n"
        "3. Del      : remove one student by id         \r\n"
        "4. Modify   : change origin to new by id       \r\n"
        "5. Display  : show all stored student info     \r\n"
        "6. Space    : show how space are used          \r\n"
        "7. Set Fit  :  default is fitequal             \r\n"
        "8. Defrag   :  reorganize free space           \r\n"
        "9. Info     :                                  \r\n"
        "10.Score    :                                  \r\n"
        "0. exit                                        \r\n"
        "-------------------------------------------    \r\n"
        "Please enter one number to select:             \r\n");
}

void
showmenu(void)
{
    showinfo("ѧ����Ϣ�洢����˵�:                                    \r\n"
        "--------------------------------------------------------------\r\n"
        "1. ���    : ���ѧ����Ϣ                                      \r\n"
        "2. ��ѯ    : ֧��ѧ����Ϣ�Լ���ŵĶ��ֶ�ģ����ѯ              \r\n"
        "3. ɾ��    : ����ɾ��ָ����ŵ�ѧ����Ϣ�����������ò�ѯȷ�ϱ��\r\n"
        "4. �޸�    : �����޸�ָ����ŵ�ѧ����Ϣ�����������ò�ѯȷ�ϱ��\r\n"
        "5. ��ʾ    : ��ʾ�����Ѵ洢ѧ����Ϣ                            \r\n"
        "6. �ռ�ͳ��: ��ʾ�ļ��ռ�ʹ�����                             \r\n"
        "7. ���䷽ʽ: Ĭ�����ϸ�ƥ��(FIT_EQUAL)                        \r\n"
        "8. ��Ƭ����:                                                  \r\n"
        "9. ��Ϣռ��: ��ѧ����Ϣ��С������ռ��                        \r\n"
        "10.ƽ����  : ��ʾƽ���ɼ�                                     \r\n"
        "0. �˳�                                        \r\n"
        "-------------------------------------------    \r\n"
        "���������ѡ��:             \r\n");
}

void
showfit(void)
{
    showinfo("This is the fit way you can choose:\r\n"
            "---------------------------------------\r\n"
            "0. FIT_EQUAL       : find deled space whose size just equal to your input\r\n"
            "1. FIT_FIRST       : find the first \r\n"
            "2. FIT_ENOUGH      : find the smallest that >= \r\n"
            "3. FIT_EQUAL_ENOUGH: if no equal than enough \r\n"
            "4. FIT_EQUAL_FIRST : if no equal than the first\r\n"
            );
}

void
getselect(int *select)
{
    //take care of the input
    scanf("%d", select);
    fflush(stdin);
}

int 
isvalidgender(char gender)
{
    int valid = 0;

    if (g_gender[0] == gender
        ||g_gender[1] == gender
        || g_gender[2] == gender
        || g_gender[3] == gender
        )
    {
       valid = 1;  
    }

    return valid;
}

void
getgender(char *gender, char *szhitinfo)
{
    char szinfo[MAX_INPUT] = {0};
    int valid = 0;
    int i     = 0;

    while (1)
    {
        while (i < 4)
        {
            sprintf(szinfo, "%c for %s ", g_gender[i], g_gender_desc[i]);
            showinfo(szinfo);

            i++;
        }
        showinfo("\r\n");
        i = 0;

        showinfo(szhitinfo);
        scanf("%c", gender);
        fflush(stdin);
    
        valid = isvalidgender(*gender);
        if (1 == valid)
        {
            break;
        }       

        showinfo("\r\nPlease make the right decision^_^\r\n");
    }
}

void
getbirthday(unsigned short *birthday, char *szhitinfo)
{
    while (1)
    {
        showinfo("Birthday Format is MMDD, like 1212 or 0101\r\n");
        showinfo(szhitinfo);
        scanf("%ud", birthday);
        fflush(stdin);
        
        if (*birthday >= 0101
            && *birthday <= 1231
            //also other check to do
            )
        {
            break;
        }
        
        showinfo("\r\nPlease make the right decision^_^\r\n");
    }
}

void
getscore(float *fscore, char *szhitinfo)
{
    showinfo(szhitinfo);
    scanf("%f", fscore);
    fflush(stdin);
}

void
getinput(char *chinput, char *szhitinfo)
{
    showinfo(szhitinfo);

    //255 is for the MAX_INPUT 256
    scanf("%255s", chinput);    
    fflush(stdin);
}

void
show1info(tag_student *student, char *szname, char *szno, int infoid)
{
    char szhitinfo[MAX_INPUT * 2] = {0};
    
    sprintf(szhitinfo, "%d   %s   %s   %c   %d   %f\r\n",
        infoid,
        szname,
        szno,
        student->chgender,
        student->ushbirth,
        student->fscore
        );
    showinfo(szhitinfo);
}

void
getnewinfo(tag_student *student, char *szname, char *szno, int *infoid, 
           int actiontype,
           tag_newinfo *flag
           )
{
    int ch = 0;

    showinfo("�Ƿ����������(Y/N)");
    ch = getchar();
    fflush(stdin);
    if (ch == 'Y')
    {
        getinput(szname, "Enter Name: ");
        flag->name = 1;
    }
    
    showinfo("�Ƿ����ѧ�ţ�(Y/N)");
    ch = getchar();
    fflush(stdin);
    if (ch == 'Y')
    {
        getinput(szno,   "Enter No:");
        flag->no = 1;
    }
    
    showinfo("�Ƿ�����Ա�(Y/N)");
    ch = getchar();
    fflush(stdin);
    if (ch == 'Y')
    {
        getgender(&student->chgender, "Enter Gender: ");
        flag->gender = 1;
    }
    
    showinfo("�Ƿ�������գ�(Y/N)");
    ch = getchar();
    fflush(stdin);
    if (ch == 'Y')
    {
        getbirthday(&student->ushbirth, "Enter Birthday(mmdd): ");
        flag->birth = 1;
    }
    
    showinfo("�Ƿ�����ɼ���(Y/N)");
    ch = getchar();
    fflush(stdin);
    if (ch == 'Y')
    {
        getscore(&student->fscore, "Enter Score: ");
        flag->score = 1;
    }
    
    if (MODIFY == actiontype)
    {
        return;
    }

    showinfo("�Ƿ������ţ�(Y/N)");
    ch = getchar();
    fflush(stdin);
    if (ch == 'Y')
    {
        showinfo("Enter id: ");
        scanf("%d", infoid);
        fflush(stdin);

        flag->infoid = 1;
    }    
}

void
showinfo(char *szhitinfo)
{
    //can be changed according to your own situation
    printf("%s", szhitinfo);
}
