//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: cube.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides an interface to create and render a cube.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "cube.h"
#include "vertex.h"

Cube::Cube(IDirect3DDevice9* device)
{
	// save a ptr to the device
	_device = device;

	_device->CreateVertexBuffer(
		8 * sizeof(Vertex), 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	// build box       

	// fill in vertex data
	/*
	v[0] = Vertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[1] = Vertex(-1.0f,  2.0f, 1.0f, 0.0f, 1.0f);
	v[2] = Vertex( 1.0f,  2.0f, 1.0f,  1.0f, 1.0f);
	v[3] = Vertex( 1.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[4] = Vertex(-1.0f, 0.0f, 3.0f, 1.0f, 1.0f);
	v[5] = Vertex( 1.0f, 0.0f, 3.0f,  0.0f, 1.0f);
	v[6] = Vertex( 1.0f,  2.0f, 3.0f,0.0f, 0.0f);
	v[7] = Vertex(-1.0f,  2.0f, 3.0f,  1.0f, 0.0f);
	*/
	v[0] = Vertex(-1.0f, 0.0f, 4.0f, 0.0f, 0.0f);
	v[1] = Vertex(-1.0f, 2.0f, 4.0f, 0.0f, 1.0f);
	v[2] = Vertex(1.0f, 2.0f, 4.0f, 1.0f, 1.0f);
	v[3] = Vertex(1.0f, 0.0f, 4.0f, 1.0f, 0.0f);
	v[4] = Vertex(-1.0f, 0.0f, 6.0f, 1.0f, 1.0f);
	v[5] = Vertex(1.0f, 0.0f, 6.0f, 0.0f, 1.0f);
	v[6] = Vertex(1.0f, 2.0f, 6.0f, 0.0f, 0.0f);
	v[7] = Vertex(-1.0f, 2.0f, 6.0f, 1.0f, 0.0f);
	_vb->Unlock();

	_device->CreateIndexBuffer(
		36 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// fill in the back face index data
	i[6] = 6; i[7]  = 7; i[8]  = 4;
	i[9] = 6; i[10] = 4; i[11] = 5;

	// fill in the top face index data
	i[12] = 7; i[13] =  6; i[14] = 2;
	i[15] = 7; i[16] = 2; i[17] = 1;

	// fill in the bottom face index data
	i[18] = 5; i[19] = 4; i[20] = 0;
	i[21] = 5; i[22] = 0; i[23] = 3;

	// fill in the left face index data
	i[24] = 7; i[25] = 1; i[26] = 0;
	i[27] = 7; i[28] =0; i[29] = 4;

	// fill in the right face index data
	i[30] = 2; i[31] =6; i[32] = 5;
	i[33] = 2; i[34] = 5; i[35] = 3;

	_ib->Unlock();
}

Cube::~Cube()
{
	if(_vb){_vb->Release(); _vb = 0;}
	if(_ib){_ib->Release(); _ib = 0;}
}

bool Cube::draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
{   if( world )
        _device->SetTransform(D3DTS_WORLD, world);
	if( mtrl )
		_device->SetMaterial(mtrl);
	if( tex )
		_device->SetTexture(0, tex);

	_device->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_device->SetIndices(_ib);
	_device->SetFVF(FVF_VERTEX);
	_device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 
		0,                  
		0,                  
		8,
		0,
		12);  

	return true;
}