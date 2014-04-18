#ifndef UI_H_BF2C7655_C46A_4bdf_96DA_C45A738352FA

#define UI_H_BF2C7655_C46A_4bdf_96DA_C45A738352FA

#include "studmanage.h"

/*
Function: menu options for user to select.
*/
void
showmenu(void);

/*
Function: show fit ways you can choose 
*/
void
showfit(void);

/*
Function: 检查是否为正确的性别

Return  : 1 正确， 0 不正确
*/
int 
isvalidgender(char gender);

/*
Function: deal with user selection for menu
*/
void
getselect(int *select);

/*
Function: deal with gender input
*/
void
getgender(char *gender, char *szhitinfo);

/*
Function: deal with birthday input (mmdd)
*/
void
getbirthday(unsigned short *birthday, char *szhitinfo);

/*
Function: deal with score input
*/
void
getscore(float *fscore, char *szhitinfo);


/*
Function: deal with user input,
          like string to add, del, query, or modify
*/
void
getinput(char *chinput, char *hitinfo);

/*
Function: get new info to query or modify
          如果输入了相应的字段，则设置tag_newinfo *flag.
*/
void
getnewinfo(tag_student *student, char *szname, char *szno, int *infoid, 
           int actiontype,
           tag_newinfo *flag
           );

/*
Function: show one specific student info.
*/
void
show1info(tag_student *student, char *szname, char *szno, int infoid);

/*
Function: show info for users
          encapsulation of printf
*/
void
showinfo(char *szhitinfo);

#endif