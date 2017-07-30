#pragma once

class MirrorScene
{
private:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;

	D3DXVECTOR3 m_worldUp;

	D3DXVECTOR3 m_camPos;
	
	D3DLIGHT9 m_lightSun;

	D3DMATERIAL9 m_materialMirror;

	D3DMATERIAL9 m_materialTeapot;
	LPD3DXMESH m_pMeshTeapot;
	LPD3DXBUFFER m_pVBTeapot;
	D3DXVECTOR3 m_teapotPos;

	IDirect3DVertexBuffer9 * m_vertexBufferPlate;
	IDirect3DIndexBuffer9 * m_indexBufferPlate;
public:
	MirrorScene();
	~MirrorScene();

	void Setup();
	void Update();
	void Render();
};

