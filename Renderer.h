#ifndef _RENDERER_H
#define _RENDERER_H

#include <windows.h>

#include "Application.h"


const int OPEN_GL_RENDERER  = 0;
const int DIRECT_X_RENDERER = 1;


class Renderer
{

public:
    static Renderer& instance();
    BOOL activate(int);
    VOID initialize();
    VOID resize(int width, int height, float fov = 45.f, float _near = .1f, float _far = 1000.f);
    VOID render();
    VOID cleanup();
    
protected:
    static Renderer Instance;
    
private:
    int mode;
    
    Renderer();
    ~Renderer();
    
};

#endif
