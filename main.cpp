#include <windows.h>
#include "Application.h"


BOOL DEBUG_CONSOLE = FALSE;


int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int)
{
    Application::instance().start();
    return 0;
}
