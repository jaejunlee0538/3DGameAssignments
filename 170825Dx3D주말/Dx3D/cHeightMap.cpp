#include "StdAfx.h"
#include "cHeightMap.h"


cHeightMap::cHeightMap(void)
	: m_pMesh(NULL)
	, m_nTile(0)
{
}

cHeightMap::~cHeightMap(void)
{
	SAFE_RELEASE(m_pMesh);
}

void cHeightMap::Load(char* szFolder,
	char* szFile,
	char* szTexture,
	DWORD dwBytesPerPixel/* = 1*/)
{
	string sPath = string(szFolder) + string(szFile);
	m_sTexture = string(szFolder) + string(szTexture);

	FILE* fp = 0;
	fopen_s(&fp, sPath.c_str(), "rb");

	fseek(fp, 0, SEEK_END);
	int nFileSize = ftell(fp);
	int nNumVertex = nFileSize / dwBytesPerPixel;
	int nRow = (int)sqrt(nNumVertex + FLT_EPSILON);
	if( nNumVertex != nRow * nRow )
	{
		fclose(fp);
		assert(false && "정사각형태의 RAW파일만 사용 가능합니다.");
	}
	fseek(fp, 0, SEEK_SET);

	int nCol = nRow;
	m_nTile = nRow - 1;
	vector<ST_PNT_VERTEX> vecVertex(nNumVertex);
	m_vecVertex.resize(nNumVertex);
	for (int z = 0; z < nRow; ++z)
	{
		for (int x = 0; x < nCol; ++x)
		{
			unsigned char c = fgetc(fp);
			int i = z * nRow + x;
			vecVertex[i].p = D3DXVECTOR3(x, c / 10.f, z);
			vecVertex[i].n = D3DXVECTOR3(0, 1, 0);
			vecVertex[i].t = D3DXVECTOR2(x / (float)m_nTile, z / (float)m_nTile);

			m_vecVertex[i] = vecVertex[i].p;

			if(dwBytesPerPixel > 1)
			{
				fseek(fp, dwBytesPerPixel - 1, SEEK_CUR);
			}
		}
	}

	fclose(fp);

	//  ---U---
	//  |\ |\ |
	//  | \| \|
	//  L--I--R
	//  |\ |\ |
	//  | \| \|
	//  ---D---
	for (int z = 1; z < nRow - 1; ++z)
	{
		for (int x = 1; x < nCol - 1; ++x)
		{
			int l = (z + 0) * nCol + x - 1;
			int r = (z + 0) * nCol + x + 1;
			int u = (z + 1) * nCol + x + 0;
			int d = (z - 1) * nCol + x + 0;

			D3DXVECTOR3 lr = vecVertex[r].p - vecVertex[l].p;
			D3DXVECTOR3 du = vecVertex[u].p - vecVertex[d].p;
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);

			vecVertex[z * nCol + x].n = n;
		}
	}

	//  1--3
	//  |\ |
	//  | \|
	//  0--2                          
	vector<DWORD> vecIndex;
	vecIndex.reserve(m_nTile * m_nTile * 2 * 3);
	for (int z = 0; z < m_nTile; ++z)
	{
		for (int x = 0; x < m_nTile; ++x)
		{
			int _0 = (z + 0) * nCol + x + 0;
			int _1 = (z + 1) * nCol + x + 0;
			int _2 = (z + 0) * nCol + x + 1;
			int _3 = (z + 1) * nCol + x + 1;
			vecIndex.push_back(_0); vecIndex.push_back(_1); vecIndex.push_back(_2);
			vecIndex.push_back(_3); vecIndex.push_back(_2); vecIndex.push_back(_1);
		}
	}

	D3DXCreateMeshFVF(vecIndex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = 0;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = 0;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = 0;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, (vecIndex.size() / 3) * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdj(vecIndex.size());
	m_pMesh->GenerateAdjacency(0, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT | 
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0], 0, 0, 0);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
}

bool cHeightMap::GetHeight( IN float x, OUT float& y, IN float z )
{
	if (x < 0 || z < 0 || x > m_nTile || z > m_nTile)
		return false;

	//  1--3
	//  |\ |
	//  | \|
	//  0--2 
	int nX = x;
	int nZ = z;
	
	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;
	
	int _0 = (nZ + 0) * (m_nTile + 1) + nX + 0;
	int _1 = (nZ + 1) * (m_nTile + 1) + nX + 0;
	int _2 = (nZ + 0) * (m_nTile + 1) + nX + 1;
	int _3 = (nZ + 1) * (m_nTile + 1) + nX + 1;

	if(fDeltaX + fDeltaZ < 1.0f)
	{
		D3DXVECTOR3 _01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 _02 = m_vecVertex[_2] - m_vecVertex[_0];
		y = (_01 * fDeltaZ + _02 * fDeltaX).y + m_vecVertex[_0].y;
	}
	else
	{
		D3DXVECTOR3 _31 = m_vecVertex[_1] - m_vecVertex[_3];
		D3DXVECTOR3 _32 = m_vecVertex[_2] - m_vecVertex[_3];
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		y = (_31 * fDeltaX + _32 * fDeltaZ).y + m_vecVertex[_3].y;
	}

	return true;
}

void cHeightMap::Render()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	//g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture(m_sTexture));
	g_pD3DDevice->SetTexture(0, 0);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetFVF(m_pMesh->GetFVF());
	m_pMesh->DrawSubset(0);
}

