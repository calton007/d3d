//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: cube.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Provides an interface to create and render a tetrahedron.
//四面体的具体顶点定义
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "tetrahedron.h"
#include "vertex.h"

Tetrahedron::Tetrahedron(IDirect3DDevice9* device)
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

	// fill vertex data
	/*
	v[0] = Vertex(2.0f, 3.0f, -2.0f,0.0f,0.0f);
	v[1] = Vertex(1.0f,  1.0f, -3.0f,0.0f,1.0f);
	v[2] = Vertex(3.0f,  0.0f, -3.0f,1.0f,0.0f);//012

	v[3] = Vertex( 1.0f, 0.0f, -1.0f,0.0f,1.0f);//301
	v[4] = Vertex(3.0f,  0.0f,  -1.0f,1.1f,1.1f);//431,412
	v[5] = Vertex(1.0f,  1.0f, -3.0f,1.0f,0.0f);//1
	*/

	v[0] = Vertex(0.0f, 3.0f, -2.0f, 0.0f, 0.0f);
	v[1] = Vertex(-1.0f, 1.0f, -3.0f, 0.0f, 1.0f);
	v[2] = Vertex(1.0f, 0.0f, -3.0f, 1.0f, 0.0f);//012

	v[3] = Vertex(-1.0f, 0.0f, -1.0f, 0.0f, 1.0f);//301
	v[4] = Vertex(1.0f, 0.0f, -1.0f, 1.1f, 1.1f);//431,412
	v[5] = Vertex(-1.0f, 1.0f, -3.0f, 1.0f, 0.0f);//1
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

	// fill index data，共6个面18个索引
	i[0] = 1; i[1] = 0; i[2] = 2;
	i[3] = 2; i[4] = 0; i[5] = 4;
	i[6] = 0; i[7]  = 3; i[8]  = 4;
	i[9] = 3; i[10] = 0; i[11] = 5;
	i[12] = 4; i[13] =  3; i[14] = 1;
	i[15] = 4; i[16] = 1; i[17] = 2;


	_ib->Unlock();
}

Tetrahedron::~Tetrahedron()
{
	if(_vb){_vb->Release(); _vb = 0;}
	if(_ib){_ib->Release(); _ib = 0;}
}

bool Tetrahedron::draw1(D3DXMATRIX* world,D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
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
		6);//5个点画6个三角形  

	return true;
}