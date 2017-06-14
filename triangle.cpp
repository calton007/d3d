//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: triangle.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Renders a triangle in wireframe mode.  Demonstrates vertex buffers, 
//       render states, and drawing commands.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"

//
// Globals
//

IDirect3DDevice9* Device = 0;

const int Width = 640;
const int Height = 480;

IDirect3DVertexBuffer9* Triangle = 0; // vertex buffer to store
// our triangle data.

//
// Classes and Structures类和结构的定义
//
//定义顶点结构
struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x;	 _y = y;  _z = z;
		_color = c;
	}//三角形的顶点数组


	float _x, _y, _z;
	D3DCOLOR _color;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

//
// Framework Functions 框架功能
//
bool Setup()
{
	//
	// Create the vertex buffer. 创建顶点缓冲区
	//

	Device->CreateVertexBuffer(
		3 * sizeof(Vertex), // size in bytes
		D3DUSAGE_WRITEONLY, // flags
		Vertex::FVF,        // vertex format
		D3DPOOL_MANAGED,    // managed memory pool
		&Triangle,          // return create vertex buffer返回的缓冲区指针
		0);                 // not used - set to 0

	//
	// Fill the buffers with the triangle data.把图形的数据装入缓冲区
	//

	Vertex* vertices;
	Triangle->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(-2.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	vertices[1] = Vertex(0.0f, 2.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	vertices[2] = Vertex(2.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255));

	Triangle->Unlock();

	
	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);//摄像机在世界中的位置
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);//摄像机的观察点
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);//摄像机的向上向量
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);//做矩阵变换
	Device->SetTransform(D3DTS_VIEW, &V);//摄像机坐标显示
	


	//
	// Set the projection matrix.设置投影矩阵
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,                        // result
		D3DX_PI * 0.5f,               // 90 - degrees
		(float)Width / (float)Height, // aspect ratio
		1.0f,                         // near plane
		1000.0f);                     // far plane
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//
	// Set wireframe mode render state.
	//
	//Device->SetRenderState(D3DRS_LIGHTING,false);
	//	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);   //线框图形
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//背面不隐藏
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);       //画带颜色的图形---高洛德着色
	Device->SetRenderState(D3DRS_LIGHTING, false);
	return true;
}
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(Triangle);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		D3DXMATRIX Ry;                 //关于Y旋转后的矩阵
		static float y = 0.0f;         //变换弧度，Y初始值为0
		D3DXMatrixRotationY(&Ry, y);   //做变换

		y += timeDelta;
		if (y >= 6.28f)
			y = 0.0f;                 //Y随着时间在不停的变换
		D3DXMATRIX p = Ry;

		Device->SetTransform(D3DTS_WORLD, &p);//将Ry给世界视角显示

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();                 //开始渲染

		Device->SetStreamSource(0, Triangle, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		// Draw one triangle.
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		Device->EndScene();                   //渲染结束
		Device->Present(0, 0, 0, 0);
	}
	return true;
}


//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
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
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}