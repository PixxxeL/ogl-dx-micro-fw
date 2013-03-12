#include "Renderer.h"
#include "OglRenderer.h"
#include "DxRenderer.h"



Renderer Renderer::Instance;



/**
 * Constructor
 */
Renderer::Renderer()
{
    //
}



/**
 * Destructor
 */
Renderer::~Renderer()
{
    //
}



/**
 * Access for instace
 */
Renderer& Renderer::instance()
{
    return Instance;
}



/**
 * Connect render API to device context
 */
BOOL Renderer::activate(int renderer)
{
    mode = renderer;
    
    if (mode == OPEN_GL_RENDERER)
        OglRenderer::instance().activate();
    if (mode == DIRECT_X_RENDERER)
        DxRenderer::instance().activate();
        
    return TRUE;
}



/**
 * Release resources
 */
VOID Renderer::cleanup()
{
    if (mode == OPEN_GL_RENDERER)
        OglRenderer::instance().cleanup();
    if (mode == DIRECT_X_RENDERER)
        DxRenderer::instance().cleanup();
}



/**
 * Configure render API
 */
VOID Renderer::initialize()
{
    if (mode == OPEN_GL_RENDERER)
        OglRenderer::instance().initialize();
    if (mode == DIRECT_X_RENDERER)
        DxRenderer::instance().initialize();
}



/**
 * Resize view
 */
VOID Renderer::resize(int width, int height, float fov, float _near, float _far)
{
    if (height == 0) height = 1;
    if (mode == OPEN_GL_RENDERER)
        OglRenderer::instance().resize(width, height, fov, _near, _far);
    if (mode == DIRECT_X_RENDERER)
        DxRenderer::instance().resize(width, height, fov, _near, _far);
}



/**
 * Render frame
 */
VOID Renderer::render()
{
    if (mode == OPEN_GL_RENDERER)
        OglRenderer::instance().render();
    if (mode == DIRECT_X_RENDERER)
        DxRenderer::instance().render();
}
