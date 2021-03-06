//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: texCube.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Renders a textured cube.  Demonstrates creating a texture, setting
//       texture filters, enabling a texture, and texture coordinates.  Use
//       the arrow keys to orbit the scene.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
#include "cube.h"
#include "chair.h"
#include "tetrahedron.h"
#include "octahedron.h"
#include "arrowhead.h"
#include "vertex.h"

//
// Globals
//

IDirect3DDevice9*     Device = 0; 

const int Width  = 640;
const int Height = 480;
 
Cube*              Box = 0;
Tetrahedron*        Tet=0;
Octahedron*         Oct=0;
Arrowhead*           Arr=0;
Chair*             Cha = 0;
IDirect3DTexture9* Tex = 0;
IDirect3DTexture9* Tex1 = 0;
IDirect3DTexture9* Tex2 = 0;
IDirect3DTexture9* Tex3 = 0;

//
// Framework Functions
//
bool Setup()
{
	//
	// Create .
	//

	Box = new Cube(Device);
	Tet = new Tetrahedron(Device);
	Oct = new Octahedron(Device);
	Arr = new Arrowhead(Device);
	Cha = new Chair(Device);

	//
	// Set a directional light.
	//

	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	light.Diffuse   = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.Specular  = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	Device->SetLight(0, &light);
	Device->LightEnable(0, true);

	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	//
	// Create texture.
	//
	D3DXCreateTextureFromFile(
		Device,
		"crate.jpg",
		&Tex);
	D3DXCreateTextureFromFile(
		Device,
		"59.jpg",
		&Tex1);
	D3DXCreateTextureFromFile(
		Device,
		"62.jpg",
		&Tex2);
	D3DXCreateTextureFromFile(
		Device,
		"111.jpg",
		&Tex3);

	// 
	// Set Texture Filter States.
	//

	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.5f, // 90 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	d3d::Delete<Cube*>(Box);
	d3d::Delete<Tetrahedron*>(Tet);
	d3d::Delete<Octahedron*>(Oct);
	d3d::Delete<Arrowhead*>(Arr);
	d3d::Delete<Chair*>(Cha);
	d3d::Release<IDirect3DTexture9*>(Tex);
	d3d::Release<IDirect3DTexture9*>(Tex1);
	d3d::Release<IDirect3DTexture9*>(Tex2);
	d3d::Release<IDirect3DTexture9*>(Tex3);
}

bool Display(float timeDelta)
{
	if( Device )
	{
		// 
		// Update the scene: update camera position.
		//
		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float cameraHeight = 0.0f;
		static float cameraHeightDirection = 5.0f;
		
		D3DXVECTOR3 position( cosf(angle) * 10.0f, cameraHeight, sinf(angle) * 10.0f );

		// the camera is targetted at the origin of the world
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);

		// the worlds up vector
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		Device->SetTransform(D3DTS_VIEW, &V);

		// compute the position for the next frame
		angle += timeDelta;
		if( angle >= 6.28f )
			angle = 0.0f;

		// compute the height of the camera for the next frame
			
		
		//
		// Draw the scene:
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		Device->SetMaterial(&d3d::WHITE_MTRL);
		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			{Device->SetTexture(0, Tex1);
		 }//  按左箭头时为图片1                                                                                                        
		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			{Device->SetTexture(0, Tex2);
	}//  按右箭头时为图片2
		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			{Device->SetTexture(0, Tex3);}//  按上箭头时为图片3
		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			{Device->SetTexture(0, Tex);
		}//  按下箭头时为图片0


		Box->draw(0, 0, 0);
		//画正方体
		Tet->draw1(0,0, 0);
	   //画四面体
		Oct->draw2(0,0, 0);
		//画八面体
		Arr->draw3(0,0, 0);
		//画箭头
		Cha->draw7(0, 0, 0);
		//画椅子
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
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