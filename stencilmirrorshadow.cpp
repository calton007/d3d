//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: stencilmirrorshadow.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates mirrors and shadows with stencils.  Use the arrow keys
//       and the 'A' and 'S' key to navigate the scene and translate the teapot.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include"chair.h"
#include "cube.h"
//
// Globals
//

IDirect3DDevice9* Device = 0; 

const int Width  = 800;//640
const int Height = 480;//480

IDirect3DVertexBuffer9* VB = 0;
Chair* Cha=0;//定义椅子
Cube*  Box = 0;//定义箱子
D3DXVECTOR4 lightDirection(0.707f, -0.707f, 0.707f, 0.0f);
D3DXVECTOR3 ChairPosition(-12.0f, 5.0f, -12.0f);

IDirect3DTexture9* FloorTex  = 0;//地板贴图
IDirect3DTexture9* PoolTex   = 0;//游泳池贴图
IDirect3DTexture9* ChairTex = 0;//椅子贴图
IDirect3DTexture9* CubeTex = 0;//椅子贴图
D3DMATERIAL9 FloorMtrl  = d3d::WHITE_MTRL;//地板材料
D3DMATERIAL9 PoolMtrl   = d3d::WHITE_MTRL;//游泳池材料
D3DMATERIAL9 ChairMtrl = d3d::WHITE_MTRL;//椅子材料
D3DMATERIAL9 CubeMtrl = d3d::WHITE_MTRL;//椅子材料

void RenderScene();//画场景
void RenderMirror();//画镜子
void RenderShadow();//画影子

//
// Classes and Structures
//
struct Vertex
{//游泳池和地板的顶点定义
	Vertex(){}
	Vertex(float x, float y, float z, 
		   float nx, float ny, float nz,
		   float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//
// Framework Functions
//
bool Setup()
{
	//
	// Make Floor have low specular reflectance - 20%.
	//

	FloorMtrl.Specular = d3d::WHITE * 0.2f;

	//
	// Create the teapot.
	//

    Cha = new Chair(Device);//实例化椅子
	Box = new Cube(Device);//实例化箱子
	//
	// Create Screen
  	//    Lighting
    //   |----|----|----|
    //   |    |    |    |
	//   |    |    |    |
    //   /--------------/
    //Chair/   Pool   / Floor/
	// /--------------/
	//
	Device->CreateVertexBuffer(
		18 * sizeof(Vertex),
		0, // usage
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Vertex* v ;
	VB->Lock(0, 0, (void**)&v, 0);

	/*
	// Floor
	v[0] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(-4.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	
	v[3] = Vertex(-7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(-4.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(-4.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// We leave gap in middle of walls for Pool
	v[6]  = Vertex(4.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[7]  = Vertex(4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[8]  = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	
	v[9]  = Vertex(4.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	//Pool

	v[12] = Vertex(-4.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[13] = Vertex(-4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[14] = Vertex(4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	
	v[15] = Vertex(-4.5f, 0.0f,-10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[16] = Vertex(4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[17] = Vertex(4.5f, 0.0f,-10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
	*/
	// Floor
	v[0] = Vertex(-9.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-9.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(-2.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	
	v[3] = Vertex(-9.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[4] = Vertex(-9.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[5] = Vertex(-2.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	// We leave gap in middle of walls for Pool
	v[6]  = Vertex(4.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[7]  = Vertex(4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[8]  = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	
	v[9]  = Vertex(4.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[10] = Vertex(7.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);

	//Pool

	v[12] = Vertex(-4.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[13] = Vertex(-4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	v[14] = Vertex(4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	
	v[15] = Vertex(-4.5f, 0.0f,-10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	v[16] = Vertex(4.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	v[17] = Vertex(4.5f, 0.0f,-10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);


	VB->Unlock();
	
	//
	// Load Textures, set filters.
	//

	D3DXCreateTextureFromFile(Device, "brick.jpg", &FloorTex);
	D3DXCreateTextureFromFile(Device, "water.jpg", &PoolTex);
	D3DXCreateTextureFromFile(Device, "66.jpg", &ChairTex);
	D3DXCreateTextureFromFile(Device, "111.jpg", &CubeTex);
	/*
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	*/
	//
	// Lights.
	//
	D3DXVECTOR3 lightDir(0.707f, -0.707f, 0.707f);
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light = d3d::InitDirectionalLight(&lightDir, &color);

	Device->SetLight(0, &light);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Set Camera.
	//

	D3DXVECTOR3    pos(-10.0f, 3.0f, -15.0f);
	D3DXVECTOR3 target(0.0, 0.0f, 0.0f);
	D3DXVECTOR3     up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &pos, &target, &up);

	Device->SetTransform(D3DTS_VIEW, &V);

	//
	// Set projection matrix.
	//
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI / 4.0f, // 45 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DTexture9*>(FloorTex);
	d3d::Release<IDirect3DTexture9*>(PoolTex);
	d3d::Release<IDirect3DTexture9*>(ChairTex);
	d3d::Release<IDirect3DTexture9*>(CubeTex);
	d3d::Delete<Chair*>(Cha);
	d3d::Delete<Cube*>(Box);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		//
		// spin 
		//
	

		// rotate 45 degrees on x-axis
		lightDirection.x = 3.14f / 4.0f;

		// incremement y-rotation angle each frame
		//static float z = 0.0f;
		//lightDirection.z=0;
		lightDirection.z += timeDelta;
		//lightDirection.y += timeDelta;

		// reset angle to zero when angle reaches 2*PI
		if( lightDirection.z >= 6.28f )
			lightDirection.z  = 0.0f;

		static float radius = 20.0f;

		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			ChairPosition.x -= 3.0f * timeDelta;

		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			ChairPosition.x += 3.0f * timeDelta;

		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			 ChairPosition.y -= 2.0f * timeDelta;

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			ChairPosition.y += 2.0f * timeDelta;


		static float angle = (3.0f * D3DX_PI) / 2.0f;
	
		if( ::GetAsyncKeyState('A') & 0x8000f )
			angle -= 0.5f * timeDelta;

		if( ::GetAsyncKeyState('S') & 0x8000f )
			angle += 0.5f * timeDelta;

		D3DXVECTOR3 position( cosf(angle) * radius, 3.0f, sinf(angle) * radius );
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &V);
		//
		// Draw the scene:
		//
		Device->Clear(0, 0, 
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
			0xff000000, 1.0f, 0L);

		Device->BeginScene();

		RenderScene();

		// draw shadow before mirror because the depth buffer hasn't been cleared
		// yet and we need the depth buffer so that the shadow is blended correctly.
		// That is, if an object obscures the shadow, we don't want to write the shadow
		// pixel.  Alternatively, we could redraw the scene to rewrite the depth buffer.
		// (RenderMirror clears the depth buffer).
		RenderShadow();

		RenderMirror();	

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

void RenderScene()
{
	// draw teapot
	Device->SetMaterial(&ChairMtrl);
	Device->SetTexture(0, ChairTex);
	Device->SetMaterial(&CubeMtrl);
	Device->SetTexture(0, CubeTex);
	D3DXMATRIX W;
	D3DXMatrixTranslation(&W,
		-12.0f, 
		5.0f,
		-12.0f);
	D3DXMATRIX R;
	D3DXMatrixScaling(&R,0.5f,0.5f,0.5f);//缩放矩阵
	D3DXMATRIX p;
	p=W*R;
	Device->SetTransform(D3DTS_WORLD, &p);
	Cha->draw(0,0,0);
	Box->draw(0, 0, 0);

	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_WORLD, &I);

	Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	Device->SetFVF(Vertex::FVF);

	// draw the floor
	Device->SetMaterial(&FloorMtrl);
	Device->SetTexture(0, FloorTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

	// draw the Pool
	Device->SetMaterial(&PoolMtrl);
	Device->SetTexture(0, PoolTex);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 12, 2);
}	

void RenderMirror()
{
	//
	// Draw Mirror quad to stencil buffer ONLY.  In this way
	// only the stencil bits that correspond to the mirror will
	// be on.  Therefore, the reflected teapot can only be rendered
	// where the stencil bits are turned on, and thus on the mirror 
	// only.
	//

    Device->SetRenderState(D3DRS_STENCILENABLE,    true);
    Device->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
    Device->SetRenderState(D3DRS_STENCILREF,       0x1);
    Device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);

	// disable writes to the depth and back buffers
    Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    // draw the mirror to the stencil buffer
	Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
	Device->SetFVF(Vertex::FVF);
	Device->SetMaterial(&PoolMtrl);
	Device->SetTexture(0, PoolTex);
	D3DXMATRIX I;
	D3DXMatrixIdentity(&I);
	Device->SetTransform(D3DTS_WORLD, &I);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);

	// re-enable depth writes
	Device->SetRenderState( D3DRS_ZWRITEENABLE, true );

	// only draw reflected chair to the pixels where the mirror
	// was drawn to.
	Device->SetRenderState(D3DRS_STENCILFUNC,  D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILPASS,  D3DSTENCILOP_KEEP);

	// position reflection
	D3DXMATRIX W, T, R;
	D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
	D3DXMatrixReflect(&R, &plane);

	D3DXMatrixTranslation(&T,
		-12.0f, 
		5.0f,
		-12.0f); 
		D3DXMATRIX R1;
	D3DXMatrixScaling(&R1,0.5f,0.5f,0.5f);//缩放矩阵
	W = T * R;

	// clear depth buffer and blend the reflected teapot with the mirror
	Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

	// Finally, draw the reflected teapot
	Device->SetTransform(D3DTS_WORLD, &W);
	Device->SetMaterial(&ChairMtrl);
	Device->SetTexture(0, ChairTex);
	Device->SetMaterial(&CubeMtrl);
	Device->SetTexture(0, CubeTex);
	// reverse cull mode
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	Cha->draw(0,0,0);
	Box->draw(0, 0, 0);
	// Restore render states.
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState( D3DRS_STENCILENABLE, false);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void RenderShadow()
{
	Device->SetRenderState(D3DRS_STENCILENABLE,    true);
    Device->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILREF,       0x0);
    Device->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
    Device->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR); // increment to 1

	// position shadow
	
	D3DXPLANE groundPlane(0.0f, -1.0f, 0.0f, 0.0f);

	D3DXMATRIX S;
	D3DXMatrixShadow(
		&S,
		&lightDirection,
		&groundPlane);

	D3DXMATRIX T;
	D3DXMatrixTranslation(
		&T,
		-12.0f,
		5.0f,
		-12.0f);
	D3DXMATRIX R;
	D3DXMatrixScaling(&R,0.5f,0.5f,0.5f);//缩放矩阵
	D3DXMATRIX W = T * S*R;
	Device->SetTransform(D3DTS_WORLD, &W);

	// alpha blend the shadow
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	D3DMATERIAL9 mtrl = d3d::InitMtrl(d3d::BLACK, d3d::BLACK, d3d::BLACK, d3d::BLACK, 0.0f);
	mtrl.Diffuse.a = 0.5f; // 50% transparency.

	// Disable depth buffer so that z-fighting doesn't occur when we
	// render the shadow on top of the floor.
	Device->SetRenderState(D3DRS_ZENABLE, false);

	Device->SetMaterial(&mtrl);
	Device->SetTexture(0, 0);
	Cha->draw(0,0,0);
	Box->draw(0, 0, 0);

	Device->SetRenderState(D3DRS_ZENABLE, true);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_STENCILENABLE,    false);
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);

		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}