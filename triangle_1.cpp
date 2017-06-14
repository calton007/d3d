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
// Classes and Structures��ͽṹ�Ķ���
//
//���嶥��ṹ
struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, D3DCOLOR c)
	{
		_x = x;	 _y = y;  _z = z;
		_color = c;
	}//�����εĶ�������


	float _x, _y, _z;
	D3DCOLOR _color;

	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

//
// Framework Functions ��ܹ���
//
bool Setup()
{
	//
	// Create the vertex buffer. �������㻺����
	//

	Device->CreateVertexBuffer(
		3 * sizeof(Vertex), // size in bytes
		D3DUSAGE_WRITEONLY, // flags
		Vertex::FVF,        // vertex format
		D3DPOOL_MANAGED,    // managed memory pool
		&Triangle,          // return create vertex buffer���صĻ�����ָ��
		0);                 // not used - set to 0

	//
	// Fill the buffers with the triangle data.��ͼ�ε�����װ�뻺����
	//

	Vertex* vertices;
	Triangle->Lock(0, 0, (void**)&vertices, 0);

	vertices[0] = Vertex(-2.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	vertices[1] = Vertex(0.0f, 2.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	vertices[2] = Vertex(2.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255));

	Triangle->Unlock();

	
	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);//������������е�λ��
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);//������Ĺ۲��
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);//���������������
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);//������任
	Device->SetTransform(D3DTS_VIEW, &V);//�����������ʾ
	


	//
	// Set the projection matrix.����ͶӰ����
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
	//	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);   //�߿�ͼ��
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//���治����
	Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);       //������ɫ��ͼ��---�������ɫ
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
		D3DXMATRIX Ry;                 //����Y��ת��ľ���
		static float y = 0.0f;         //�任���ȣ�Y��ʼֵΪ0
		D3DXMatrixRotationY(&Ry, y);   //���任

		y += timeDelta;
		if (y >= 6.28f)
			y = 0.0f;                 //Y����ʱ���ڲ�ͣ�ı任
		D3DXMATRIX p = Ry;

		Device->SetTransform(D3DTS_WORLD, &p);//��Ry�������ӽ���ʾ

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();                 //��ʼ��Ⱦ

		Device->SetStreamSource(0, Triangle, 0, sizeof(Vertex));
		Device->SetFVF(Vertex::FVF);

		// Draw one triangle.
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		Device->EndScene();                   //��Ⱦ����
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