#pragma once
#include "iMap.h"
class HeightMap : public iMap
{
public:
	HeightMap();
	~HeightMap();

	void Render();
	bool Load(const char* filePath, float heightScale, float cellSize, size_t numVerticesX, size_t numVerticesZ, const char* texturePath);
	bool GetHeight(OUT float& y, IN float x, IN float z) override;
private:
	float m_cellSize;
	size_t m_numVerticesX, m_numVerticesZ;
	std::vector<D3DXVECTOR3> m_collisionVertices;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DTEXTURE9 m_pTexture;
	D3DMATERIAL9 m_mtlGround;
};

