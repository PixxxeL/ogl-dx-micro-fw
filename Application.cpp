#include "Application.h"
#include "Renderer.h"



#define KEY_DOWN(vk_code) \
    ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) \
    ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)



extern const int OPEN_GL_RENDERER;
extern const int DIRECT_X_RENDERER;

bool Application::isActive = true;
bool Application::keys[256];
Application Application::Instance;



/**
 * Constructor
 */
Application::Application()
{
    className = "OGLDXWindowsApp010";
    appTitle = "framework 0.1.0, by .p.i.x.e.l.";
    oglTitle = "OpenGL";
    dxTitle  = "DirectX";
    width = 640;
    height = 480;
    bits = 32;
    hWnd = NULL;
    hDC = NULL;
    isFullscreen = true;
    renderTimer = new Timer();
    fps = 60;
    renderer = OPEN_GL_RENDERER; // OPEN_GL_RENDERER or DIRECT_X_RENDERER
}



/**
 * Destructor
 */
Application::~Application()
{
    //
}



/**
 * Access for instace
 */
Application& Application::instance()
{
    return Instance;
}



/**
 * Startup application
 */
BOOL Application::start()
{
    if( MessageBox(
            NULL,
            "Запустить приложение в полноэкранном режиме?",
            "Выбор режима окна",
            MB_YESNO | MB_ICONQUESTION
        ) == IDNO
    ) {
        isFullscreen = false;
    }
    
    if ( createWindow() )  
        run();
        
    delete renderTimer;
    renderTimer = NULL;
    
    return FALSE;
}



/**
 * Window's handle
 */
HWND Application::getHwnd()
{
    return hWnd;
}



/**
 * Device context
 */
HDC Application::getHdc()
{
    return hDC;
}



/**
 * Main loop
 */
BOOL Application::run()
{
    static DWORD ms = (DWORD)(1000 / fps);
    MSG msg;
    ZeroMemory( &msg, sizeof( msg ) );
    while( msg.message != WM_QUIT )
    {   
        // keyboards processing will here...
        
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            if( msg.message == WM_QUIT || KEY_DOWN(VK_ESCAPE) ) {
                cleanup();
                PostQuitMessage( 0 );
            }
            else {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
        }
               
        else
            if (isActive) {
                renderTimer->start();
                Renderer::instance().render();
                DWORD newTime = renderTimer->get();
                if (newTime < ms)
                    renderTimer->wait(ms - newTime);
            }

        if (isActive && keys[VK_F1])
        {
            keys[VK_F1] = false;
            cleanup();
            isFullscreen =! isFullscreen;
            createWindow();
        }
        if (isActive && keys[VK_F2])
        {
            keys[VK_F2] = false;
            cleanup();
            renderer = 1 - renderer;
            createWindow();
        }
    }
    
    return 0;
}



/**
 * Message for user about fatal error
 */
BOOL Application::showFatalError(const string& msg)
{
    cleanup();
    MessageBox(NULL, (char*)msg.c_str(), "Ошибка", MB_OK | MB_ICONSTOP);
    return FALSE;
}



/**
 * Windows message process procedure
 */
LRESULT CALLBACK Application::winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch( msg )
    {
        case WM_CLOSE:
            PostQuitMessage( 0 );
            return 0;
        
        case WM_ACTIVATE:
            isActive = HIWORD( wParam ) ? false : true;
            return 0;
            
        case WM_SYSCOMMAND:
            /* TODO (#1#): add here alt+tab or alt+enter */ 
            if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER)
                return 0;
            else
                break;

        case WM_PAINT:
            ValidateRect( hWnd, NULL );
            return 0;
            
        case WM_SIZE:
            Renderer::instance().resize( (int)LOWORD(lParam), (int)HIWORD(lParam) );
            return 0;
            
        case WM_KEYDOWN:
            keys[wParam] = true;
            return 0;
        
        case WM_KEYUP:
            keys[wParam] = false;
            return 0;
            
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}



/**
 * Creating window
 */
BOOL Application::createWindow()
{
    
    DWORD dwExStyle;
    DWORD dwStyle;
     
    
    ZeroMemory(&wc, sizeof(wc)); 
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = winProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle( NULL );
    wc.hIcon         = NULL;
    wc.hCursor       = NULL;
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = (char*)className.c_str();
    wc.hIconSm       = NULL;

    if ( !RegisterClassEx(&wc) ) {
        string msg = "Невозможно зарегистрировать класс окна: ";
        msg += className;
        return showFatalError(msg);
    }
    
    
    if (isFullscreen) {
        DEVMODE dmScreenSettings;
        ZeroMemory( &dmScreenSettings, sizeof( dmScreenSettings ) );
        dmScreenSettings.dmSize       = sizeof( dmScreenSettings );
        dmScreenSettings.dmPelsWidth  = width;
        dmScreenSettings.dmPelsHeight = height;
        dmScreenSettings.dmBitsPerPel = bits;
        dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        
        // Possible switch in fullscreen
        if( ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
            
            if( MessageBox(
                NULL,
                "Полноэкранный режим не поддерживается вашей видеокартой. Запустить в оконном режиме?",
                "Ошибка",
                MB_YESNO | MB_ICONEXCLAMATION
            ) == IDYES ) {
                isFullscreen = false;
            }
            else {
                return showFatalError("Программа будет закрыта");
            }
        }
    }
    
    if (isFullscreen) {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_POPUP;
        ShowCursor( false );
    }
    else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle   = WS_OVERLAPPEDWINDOW;
    }

    hWnd = CreateWindowEx(
        dwExStyle,
        (char*)className.c_str(),
        (char*)((renderer ? dxTitle : oglTitle) + ": " + appTitle).c_str(),
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );
    if (!hWnd) {
        string txt = "Невозможно создать окно: ";
        txt += appTitle;
        return showFatalError(txt);
    }
    
    
    if ( !( hDC = GetDC( hWnd ) ) )
        return showFatalError("Невозможно создать контекст устройства!");
    
    if ( !Renderer::instance().activate(renderer) )
        return FALSE;
    
    ShowWindow( hWnd, SW_SHOW );
    SetForegroundWindow( hWnd );
    SetFocus( hWnd );

    Renderer::instance().resize( width, height );    
    Renderer::instance().initialize();
    
    
    return TRUE;
}



/**
 * Release resources
 */
VOID Application::cleanup()
{
    if (isFullscreen) {
        ChangeDisplaySettings( NULL, 0 );
        ShowCursor( true );
    }
    
    Renderer::instance().cleanup();
    
    if (hDC) {
        ReleaseDC( hWnd, hDC );
        hDC = NULL;
    }
    if (hWnd) {
        DestroyWindow(hWnd);
        hWnd = NULL;
    }
    
    UnregisterClass((char*)className.c_str(), wc.hInstance);
}
