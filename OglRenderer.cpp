#include "OglRenderer.h"



OglRenderer OglRenderer::Instance;



/**
 * Constructor
 */
OglRenderer::OglRenderer()
{
    hDC = NULL;
    hRC = NULL;
}



/**
 * Destructor
 */
OglRenderer::~OglRenderer()
{
    //
}



/**
 * Access for instace
 */
OglRenderer& OglRenderer::instance()
{
    return Instance;
}



/**
 * Connect render API to device context
 */
BOOL OglRenderer::activate()
{
    hDC = Application::instance().getHdc();
    GLuint pixelFormat;
    
    static PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32; /* TODO (#2#): assign pfd.cColorBits from window */
    pfd.cDepthBits = 32;
    
    if ( !( pixelFormat = ChoosePixelFormat( hDC, &pfd ) ) )
        return Application::instance().showFatalError(
            "Ќевозможно определить формат пикселей!"
        );
    if ( !SetPixelFormat( hDC, pixelFormat, &pfd ) )
        return Application::instance().showFatalError(
            "Ќевозможно назначить формат пикселей!"
        );
    if ( !( hRC = wglCreateContext( hDC ) ) )
        return Application::instance().showFatalError(
            "Ќевозможно создать контекст рендеринга!"
        );
    if ( !wglMakeCurrent( hDC, hRC ) )
        return Application::instance().showFatalError(
            "Ќевозможно активировать контекст рендеринга!"
        );
    
    return TRUE;
}



/**
 * Release resources
 */
VOID OglRenderer::cleanup()
{
    if (hRC) {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hRC );
        hRC = NULL;
    }
}



/**
 * Configure OpenGL
 */
VOID OglRenderer::initialize()
{
    glShadeModel( GL_SMOOTH );
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth( 1.0f );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
}



/**
 * Resize view
 */
VOID OglRenderer::resize(int width, int height, float fov, float _near, float _far)
{
    if (height == 0) height = 1;
    glViewport( 0, 0, width, height );
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(fov, (GLfloat)width / (GLfloat)height, _near, _far);
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}



/**
 * Render frame
 */
VOID OglRenderer::render()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    
    glTranslatef(-1.5f,0.0f,-6.0f);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
    glEnd();
    
    glTranslatef(3.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f( 1.0f,-1.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f,-1.0f, 0.0f);
    glEnd();
    
    SwapBuffers(hDC);
}
