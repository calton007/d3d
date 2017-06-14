//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dxcreate.cpp
// 
// Author: Frank D. Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Renders several D3DX shapes in wireframe mode and has the camera
//       fly around the scene.  Demonstrates the D3DXCreate* functions, and
//       demonstrates more complex transformations used to position the objects
//       in the world and move the camera around the world.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"

//
// Globals
//

IDirect3DDevice9*     Device = 0; 

const int Width  = 640;
const int Height = 480;

// Store 5 objects.
ID3DXMesh* Objects[4] = {0, 0, 0, 0};

// World matrices for each object.  These matrices
// specify the locations of the objects in the world.
D3DXMATRIX ObjWorldMatrices[4];
D3DMATERIAL9 Mtrl[4];
D3DLIGHT9 dirLight[3];
//
// Framework Functions
//
bool Setup()
{
	//
	// Create the objects.
	//

	D3DXCreateTeapot(Device,&Objects[0],0);
	D3DXCreateSphere(
		Device,
		1.0f, // radius
		20,   // slices
		20,   // stacks
		&Objects[1],
		0);

	D3DXCreateTorus(
		Device,
		0.5f, // inner radius
		1.0f, // outer radius
		20,   // sides
		20,   // rings
		&Objects[2],
		0);
	// cylinder is built aligned on z-axis
	D3DXCreateCylinder(
		Device,
		0.5f, // radius at negative z end
		0.5f, // radius at positive z end
		2.0f, // length of cylinder
		20,   // slices
		20,   // stacks
		&Objects[3],
		0);

	

	

	//
	// Build world matrices - position the objects in world space.
	// For example, ObjWorldMatrices[1] will position Objects[1] at
	// (-5, 0, 5).  Likewise, ObjWorldMatrices[2] will position
	// Objects[2] at (5, 0, 5).
	//

	D3DXMatrixTranslation(&ObjWorldMatrices[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[1], 0.0f, -2.0f, 0.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[3], 3.0f, 0.0f, 0.0f);
	//
	// Build world matrices - position the objects in world space.
	//
	
	//
	// Setup the object's materials.
	//
	Mtrl[0] = d3d::RED_MTRL;
	Mtrl[1] = d3d::BLUE_MTRL;
	Mtrl[2] = d3d::GREEN_MTRL;
	Mtrl[3] = d3d::YELLOW_MTRL;
	//
	// Set and Enable the light.
	//
	D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXVECTOR3 position(1.0f, -0.0f, 0.25f);
	D3DXCOLOR   c = d3d::WHITE;
	//D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);
	dirLight[0] = d3d::InitDirectionalLight(&dir, &c);
	dirLight[1] = d3d::InitPointLight(&dir, &c);
	dirLight[2] = d3d::InitSpotLight(&position, &dir, &c);
	
	//Device->SetLight(0, &dirLight);
	Device->LightEnable(0, true);


	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, false);
	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.25f, // 90 - degree角度变大在这里修改
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//
	// Switch to wireframe mode.
	//

	//Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	return true;
}
void Cleanup()
{
	for(int i = 0; i < 4; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);
}
int index = 0;
bool Display(float timeDelta)
{
	if( Device )
	{
		if (index%3==0){
			Device->SetLight(0, &dirLight[index%3]);
		}
		else if (index % 3 == 1){
			Device->SetLight(0, &dirLight[index % 3]);
		}
		else if (index % 3 == 2){
			Device->SetLight(0, &dirLight[index % 3]);
		}
		//Device->SetLight(0, &dirLight);
		// Animate the camera:
		// The camera will circle around the center of the scene.  We use the
		// sin and cos functions to generate points on the circle, then scale them
		// by 10 to further the radius.  In addition the camera will move up and down
		// as it circles about the scene.
		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float height = 5.0f;
		//static float cameraHeightDirection = 2.0f;
		
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f * timeDelta;

		// the camera is targetted at the origin of the world
		D3DXVECTOR3 position( cosf(angle) * 7.0f, height, sinf(angle) * 7.0f );
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		Device->SetTransform(D3DTS_VIEW, &V);

		//
		// Draw the Scene:
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		for(int i = 0; i < 4; i++)
		{
			Device->SetMaterial(&Mtrl[i]);
			// Set the world matrix that positions the object.
			Device->SetTransform(D3DTS_WORLD, &ObjWorldMatrices[i]);

			// Draw the object using the previously set world matrix.
			Objects[i]->DrawSubset(0);
		}
		
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
		index++;
		Sleep(2000);
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