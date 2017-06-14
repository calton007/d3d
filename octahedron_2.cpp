//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: cube.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides an interface to create and render a cube.
//  8面体        
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "octahedron.h"
#include "vertex.h"

Octahedron::Octahedron(IDirect3DDevice9* device)
{
	// save a ptr to the device
	_device = device;

	_device->CreateVertexBuffer(
		6 * sizeof(Vertex), 
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);

	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	// build box

	// fill in vertex data
	v[0] = Vertex(-2.0f, -2.0f, -3.0f,0.0f,0.0f);
	v[1] = Vertex(-3.0f,  -2.0f, -2.0f,0.0f,1.0f);//140
	v[2] = Vertex(-2.0f,  -2.0f, -1.0f,1.0f,0.0f);//124
	v[3] = Vertex( -1.0f, -2.0f, -2.0f,0.0f,1.0f);//423
	v[4] = Vertex(-2.0f,  -1.0f,  -2.0f,1.0f,1.0f);//043
	v[5] = Vertex(-2.0f,  -3.0f,  -2.0f,1.0f,1.0f );//105,215

	

	_vb->Unlock();

	_device->CreateIndexBuffer(
		24 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

		// 索引点构成各个面
	//周面
	i[0]  = 1; i[1]  = 4; i[2]  = 0;
	i[3]  = 0; i[4]  = 4; i[5]  = 3;
	i[6]  = 1; i[7]  = 2; i[8]  = 4;
	i[9]  = 3; i[10] = 4; i[11] = 2;
	i[12] = 1; i[13] = 0; i[14] = 5;
	i[15] = 5; i[16] = 2; i[17] = 1;
	i[18] = 0; i[19] = 3; i[20] = 5;
	i[21] = 5; i[22] = 3; i[23] = 2;

	_ib->Unlock();
}

Octahedron::~Octahedron()
{
	if(_vb){_vb->Release(); _vb = 0;}
	if(_ib){_ib->Release(); _ib = 0;}
}

bool Octahedron::draw2(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
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
		6,
		0,
		8);  

	return true;
}