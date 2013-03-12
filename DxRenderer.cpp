#include "DxRenderer.h"



DxRenderer DxRenderer::Instance;



/**
 * Constructor
 */
DxRenderer::DxRenderer()
{
    hWnd = NULL;
    pD3D = NULL;
    pd3dDevice = NULL;
    pVB = NULL;
}



/**
 * Destructor
 */
DxRenderer::~DxRenderer()
{
    //
}



/**
 * Access for instace
 */
DxRenderer& DxRenderer::instance()
{
    return Instance;
}



/**
 * Startup DirectX
 */
BOOL DxRenderer::activate()
{
    hWnd = Application::instance().getHwnd();
    
    if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return Application::instance().showFatalError(
            "Невозможно определить COM-интерфейс!"
        );
    
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                    //D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                    &d3dpp, &pd3dDevice ) ) )
    {
        return Application::instance().showFatalError(
            "Невозможно создать DirectX устройство!"
        );
    }
    
    pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    //pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    
    return TRUE;
}



/**
 * Release resources
 */
VOID DxRenderer::cleanup()
{
    if( pVB != NULL )
        pVB->Release();
        
    if( pd3dDevice != NULL )
        pd3dDevice->Release();

    if( pD3D != NULL )
        pD3D->Release();
}



/**
 * Setup DirectX
 */
VOID DxRenderer::initialize()
{
    initVertexBuffer();
}



/**
 * Resize view
 */
VOID DxRenderer::resize(int width, int height, float fov, float _near, float _far)
{
    //
}



/**
 * Render frame
 */
VOID DxRenderer::render()
{
    if( NULL == pd3dDevice )
        return;

    pd3dDevice->Clear(
        0, NULL,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB( 0, 0, 0 ),
        1.0f, 0
    );

    if( SUCCEEDED( pd3dDevice->BeginScene() ) )
    {
    	setupLights();
    	setupMatrices();

    	pd3dDevice->SetStreamSource( 0, pVB, 0, sizeof( CUSTOMVERTEX ) );
    	pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
    	pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2 );

        pd3dDevice->EndScene();
    }

    pd3dDevice->Present( NULL, NULL, NULL, NULL );
}



/**
 *
 */
VOID DxRenderer::setupMatrices()
{

	D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-7.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
	pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

}



/**
 *
 */
VOID DxRenderer::setupLights()
{

	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	pd3dDevice->SetMaterial( &mtrl );

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vecDir = D3DXVECTOR3(
		cosf( timeGetTime() / 350.0f ),
	    1.0f,
	    sinf( timeGetTime() / 350.0f )
	);
	D3DXVec3Normalize( ( D3DXVECTOR3* )&light.Direction, &vecDir );
	light.Range = 1000.0f;
	pd3dDevice->SetLight( 0, &light );
	pd3dDevice->LightEnable( 0, TRUE );
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );

	pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00202020 );
}



/**
 * Vertex Buffer Object
 */
BOOL DxRenderer::initVertexBuffer()
{
    if( FAILED( pd3dDevice->CreateVertexBuffer(
    	50 * 2 * sizeof(CUSTOMVERTEX),
        0 /*Usage*/, D3DFVF_CUSTOMVERTEX,
        D3DPOOL_DEFAULT, &pVB, NULL  ) ) )
    {
        return Application::instance().showFatalError(
            "Невозможно создать буфер вершин!"
        );
    }

    CUSTOMVERTEX* pVertices;
    if( FAILED( pVB->Lock( 0, 0, ( void** )&pVertices, 0 ) ) )
        return Application::instance().showFatalError(
            "Невозможно скопировать в буфер вершин!"
        );

	for( DWORD i = 0; i < 50; i++ ) {
	    FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);
	    pVertices[2*i+0].position = D3DXVECTOR3( sinf(theta),-1.0f, cosf(theta) );
	    pVertices[2*i+0].normal   = D3DXVECTOR3( sinf(theta), 0.0f, cosf(theta) );
	    pVertices[2*i+1].position = D3DXVECTOR3( sinf(theta), 1.0f, cosf(theta) );
	    pVertices[2*i+1].normal   = D3DXVECTOR3( sinf(theta), 0.0f, cosf(theta) );
	}
    pVB->Unlock();

    return TRUE;
}
