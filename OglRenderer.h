#ifndef _OGL_RENDERER_H
#define _OGL_RENDERER_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>

#include "Application.h"


class OglRenderer
{

public:
    static OglRenderer& instance();
    BOOL activate();
    VOID initialize();
    VOID resize(int width, int height, float fov, float _near, float _far);
    VOID render();
    VOID cleanup();
    
protected:
    static OglRenderer Instance;
    
private:
    HDC hDC;
    HGLRC hRC;
    
    OglRenderer();
    ~OglRenderer();
    
};

#endif
