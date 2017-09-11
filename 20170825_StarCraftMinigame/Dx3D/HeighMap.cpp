#include "stdafx.h"
#include "HeighMap.h"
#include <fstream>

#define GET_INDEX(ix, iy, width)	((iy)*(width) + (ix))

HeightMap::HeightMap()
	:m_pTexture(NULL)
{
}


HeightMap::~HeightMap()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

void HeightMap::Render()
{
	D3DXMATRIX identity;
	D3DXMatrixIdentity(&identity);
	g_pD3DDevice->SetTexture(0, m_pTexture);
	g_pD3DDevice->SetMaterial(&m_mtlGround);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &identity);
	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
	g_pD3DDevice->SetIndices(m_pIB);
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	g_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0, m_collisionVertices.size(),0, (m_numVerticesX - 1)*(m_numVerticesZ - 1) * 2);

	//g_pD3DDevice->DrawPrimitive(D3DPT_LINESTRIP, 0, m_numVerticesX * m_numVerticesZ -1);

	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

bool HeightMap::Load(const char * filePath, float heightScale, float cellSize, size_t numVerticesX, size_t numVerticesZ, const char* texturePath)
{
	m_cellSize = cellSize;
	m_numVerticesX = numVerticesX;
	m_numVerticesZ = numVerticesZ;
	std::vector<ST_PNT_VERTEX> renderVertices;
	renderVertices.resize(numVerticesX*numVerticesZ);//버텍스를 담을 벡터
	m_collisionVertices.resize(numVerticesX*numVerticesZ);
	std::ifstream file(filePath, std::ios_base::binary | std::ios_base::in);
	if (file.good() == false) {
		return false;
	}

	uint8_t read;
	ST_PNT_VERTEX* renderVertex = renderVertices.data();
	D3DXVECTOR3* collisionVertex = m_collisionVertices.data();

	float uScale = 1.0f / (m_numVerticesX - 1.0f);
	float vScale = 1.0f / (m_numVerticesZ - 1.0f);
	
	for (size_t iz = 0; iz < m_numVerticesZ; ++iz) {
		for (size_t ix = 0; ix < m_numVerticesX; ++ix) {
			//file >> read;//높이값 파싱(0~255)
			file.read((char*)&read, 1);
			
			//높이 * heightScale을 곱해서 높이를 계산한다.
			renderVertex->p = D3DXVECTOR3(ix*cellSize, read*heightScale, iz*cellSize);
			*collisionVertex = renderVertex->p;
			renderVertex->n = D3DXVECTOR3(0, 1, 0);//노멀은 일단 0,1,0으로 초기화
			renderVertex->t = D3DXVECTOR2(ix *uScale, iz * vScale);//UV값 계산
			++renderVertex;//버텍스 포인터를 한칸 이동시킨다.
			++collisionVertex;
		}
	}
	
	//노멀값 계산
	for (size_t iz = 1; iz < m_numVerticesZ - 1; ++iz) {
		for (size_t ix = 1; ix < m_numVerticesX - 1; ++ix) {
			D3DXVec3Cross(
				&renderVertices[GET_INDEX(ix, iz, m_numVerticesX)].n,
				&(renderVertices[GET_INDEX(ix, iz + 1, m_numVerticesX)].p - renderVertices[GET_INDEX(ix, iz - 1, m_numVerticesX)].p),
				&(renderVertices[GET_INDEX(ix + 1, iz, m_numVerticesX)].p - renderVertices[GET_INDEX(ix - 1, iz, m_numVerticesX)].p)
			);
			D3DXVec3Normalize(&renderVertices[GET_INDEX(ix, iz, m_numVerticesX)].n, &renderVertices[GET_INDEX(ix, iz, m_numVerticesX)].n);
		}
	}
	//버텍스 버퍼와 인덱스 버퍼 생성
	//버텍스 버퍼 생성 및 초기화
	g_pD3DDevice->CreateVertexBuffer(
		sizeof(ST_PNT_VERTEX)*renderVertices.size(),
		D3DUSAGE_WRITEONLY,
		ST_PNT_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVB,
		0);
	if (m_pVB == NULL) {
		return false;
	}
	ST_PNT_VERTEX* pVertex = NULL;
	m_pVB->Lock(0, 0, (LPVOID*)&pVertex, 0);
	memcpy(pVertex, renderVertices.data(), sizeof(ST_PNT_VERTEX)*renderVertices.size());
	m_pVB->Unlock();

	//인덱스 버퍼 생성 및 초기화
	// ----------------
	// |\   |\   |\   |
	// | \  | \  | \  |
	// |  \ |  \ |  \ |
	// ----------------
	// |\   |\   |\   |
	// | \  | \  | \  |
	// |  \ |  \ |  \ |
	// ----------------
	int nTriangles = (m_numVerticesX - 1)*(m_numVerticesZ - 1) * 2;
	g_pD3DDevice->CreateIndexBuffer(
		sizeof(uint32_t)*nTriangles * 3,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_pIB,
		0);
	if (m_pIB == NULL) {
		SAFE_RELEASE(m_pVB);
		return false;
	}

	uint32_t* index = 0;
	m_pIB->Lock(0, 0, (LPVOID*)&index, 0);
	for (int iz = 0; iz < m_numVerticesZ - 1; ++iz) {
		for (int ix = 0; ix < m_numVerticesX - 1; ++ix) {
			index[0] = GET_INDEX(ix, iz, m_numVerticesX);
			index[1] = GET_INDEX(ix, iz + 1, m_numVerticesX);
			index[2] = GET_INDEX(ix + 1, iz + 1, m_numVerticesX);
			index[3] = GET_INDEX(ix, iz, m_numVerticesX);
			index[4] = GET_INDEX(ix + 1, iz + 1, m_numVerticesX);
			index[5] = GET_INDEX(ix + 1, iz, m_numVerticesX);
			index += 6;
		}
	}
	m_pIB->Unlock();

	//텍스쳐 
	m_pTexture = g_pTextureManager->GetTexture(texturePath);

	//매터리얼 초기화
	m_mtlGround.Ambient = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
	m_mtlGround.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_mtlGround.Specular = D3DXCOLOR(0.6f,0.6f, 0.6f, 1.0f);
	m_mtlGround.Emissive = D3DXCOLOR(0,0,0, 1.0f);
	m_mtlGround.Power = 20.0f;

	//fclose(file);
	return true;
}

bool HeightMap::GetHeight(OUT float& y, IN float x, IN float z)
{
	bool ret = true;
	int ix, iz;
	float dx, dz;
	ix = x / m_cellSize;
	iz = z / m_cellSize;

	if (x < 0) {
		ix = 0;
		x = 0.0f;
		dx = 0.0f;
		ret = false;
	}
	else if (ix >= m_numVerticesX) {
		ix = m_numVerticesX - 1;
		x = m_numVerticesX;
		dx = 1.0f;
		ret = false;
	}
	else {
		dx = x - ix;
	}
	if (z < 0) {
		iz = 0;
		z = 0.0;
		dz = 0.0f;
		ret = false;
	}
	else if (iz >= m_numVerticesZ) {
		iz = m_numVerticesZ - 1;
		z = m_numVerticesZ;
		dz = 1.0f;
		ret = false;
	}
	else {
		dz = z - iz;
	}
	//LogDebugMessage("%.3f %.3f \n", dx, dz);
	assert(dx >= 0.0);
	assert(dz >= 0.0);
	assert(dx <= 1.0f);
	assert(dz <= 1.0f);

	int idx[3];
	idx[0] = GET_INDEX(ix, iz, m_numVerticesX);
	idx[2] = GET_INDEX(ix + 1, iz + 1, m_numVerticesX);
	if (dx < dz) {
		idx[1] = GET_INDEX(ix, iz + 1, m_numVerticesX);
	}
	else {
		idx[1] = GET_INDEX(ix + 1, iz, m_numVerticesX);
	}

	if (dx < dz) {
		float yVert = m_collisionVertices[idx[0]].y - m_collisionVertices[idx[1]].y;
		float yHorz = m_collisionVertices[idx[2]].y - m_collisionVertices[idx[1]].y;
		y = m_collisionVertices[idx[1]].y + yVert * (1.0f - dz) + yHorz * dx;
	}
	else {
		float yHorz = m_collisionVertices[idx[0]].y - m_collisionVertices[idx[1]].y;
		float yVert = m_collisionVertices[idx[2]].y - m_collisionVertices[idx[1]].y;
		y = m_collisionVertices[idx[1]].y + yVert * dz + yHorz * (1.0f - dx);
	}
	return ret;
}

