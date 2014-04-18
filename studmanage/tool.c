#include <windows.h>

void 
DisableForceClose(void)
{
    char szConsoleTitle[256] = {0};
    HWND hwnd    = NULL;
    HMENU hmenu = NULL;

    GetConsoleTitle(szConsoleTitle, 256);    
    hwnd   = FindWindow(NULL, szConsoleTitle);
    hmenu  = GetSystemMenu(hwnd, FALSE);
    RemoveMenu(hmenu, SC_CLOSE, 0);

    return;
}