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

#include "arrowhead.h"
#include "vertex.h"

Arrowhead::Arrowhead(IDirect3DDevice9* device)
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

	// build arrowhead

	v[0] = Vertex(2.0f, -2.0f, -2.0f,0.0f,0.0f);
	v[1] = Vertex(1.0f,  -3.0f, -3.0f,0.0f,1.0f);
	v[2] = Vertex( 2.0f,  0.0f, -2.0f,1.1f,1.1f);
	v[3] = Vertex( 2.0f, -3.0f, -1.0f,0.0f,1.0f);	
	v[4] = Vertex(3.0f,  -3.0f,  -3.0f,0.0f,1.0f);
	_vb->Unlock();

	_device->CreateIndexBuffer(
		18 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	// 索引点构成各个面
	//周面
	i[0]  = 1; i[1]  = 2; i[2]  = 0;
	i[3]  = 0; i[4]  = 2; i[5]  = 1;
	i[6]  = 0; i[7]  = 2; i[8]  = 4;
	i[9]  = 4; i[10] = 2; i[11] = 0;
	i[12] = 0; i[13] = 2; i[14] = 3;
	i[15] = 3; i[16] = 2; i[17] = 0;

	_ib->Unlock();
}

Arrowhead::~Arrowhead()
{
	if(_vb){_vb->Release(); _vb = 0;}
	if(_ib){_ib->Release(); _ib = 0;}
}

bool Arrowhead::draw3(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
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
		5,
		0,
		6);  

	return true;
}