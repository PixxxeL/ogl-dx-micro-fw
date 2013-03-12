#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <windows.h>
#include <string>

#include "Timer.h"

using namespace std;

class Application
{

public:
    static Application& instance();
    BOOL start();
    BOOL showFatalError(const string&);
    HWND getHwnd();
    HDC getHdc();

protected:
    static Application Instance;

private:
    string className;
    string appTitle;
    string oglTitle;
    string dxTitle;
    UINT width;
    UINT height;
    int bits;
    WNDCLASSEX wc;
    HWND hWnd;
    HDC hDC;
    bool isFullscreen;
    static bool isActive;
    static bool keys[256];
    Timer* renderTimer;
    int fps;
    int renderer;
    
    Application();
    ~Application();
    static LRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM);
    BOOL createWindow();
    BOOL run();
    VOID cleanup();

};


#endif
