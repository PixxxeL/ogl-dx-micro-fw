#ifndef _DX_RENDERER_H
#define _DX_RENDERER_H

#include <windows.h>
#include <mmsystem.h>
#include <d3dx9.h>

#include "Application.h"



struct CUSTOMVERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)



class DxRenderer
{

public:
    static DxRenderer& instance();
    BOOL activate();
    VOID initialize();
    VOID resize(int width, int height, float fov, float _near, float _far);
    VOID render();
    VOID cleanup();
    
protected:
    static DxRenderer Instance;
    
private:
    HWND                    hWnd;
    LPDIRECT3D9             pD3D;
    LPDIRECT3DDEVICE9       pd3dDevice;
    LPDIRECT3DVERTEXBUFFER9 pVB;
    
    DxRenderer();
    ~DxRenderer();
    BOOL initVertexBuffer();
    VOID setupLights();
    VOID setupMatrices();
    
};

#endif
