#include "stdafx.h"
#include "cMapLoader.h"
#include "cObjLoader.h"
#include "cMtlTex.h"


cMapLoader::cMapLoader()
	:m_pMeshFloor(NULL)
	, m_pMeshWall(NULL)
{
}


cMapLoader::~cMapLoader()
{
	SAFE_RELEASE(m_pMeshFloor);
	SAFE_RELEASE(m_pMeshWall);

	for each(auto p in m_vecMtlFloor)
	{
		SAFE_RELEASE(p);
	}

	for each(auto p in m_vecMtlWall)
	{
		SAFE_RELEASE(p);
	}
}

void cMapLoader::Setup()
{
	m_vecMap.resize(NODE_ROW, std::vector<int>(NODE_COL, -1));

	cObjLoader Floor;
	//Floor
	Floor.LoadMesh("./Resource/FloorBox.obj", &m_pMeshFloor, m_vecMtlFloor);

	cObjLoader Wall;
	//Wall
	Wall.LoadMesh("./Resource/WallBox.obj", &m_pMeshWall, m_vecMtlWall);

}

void cMapLoader::MapLoad(IN string sPath)
{
	int ix, iy;
	int maxsize;
	ix = -1;
	iy = 0;
	FILE* fp = 0;
	fopen_s(&fp, sPath.c_str(), "r");

	while (!feof(fp))
	{
		char c = fgetc(fp);
		++ix;
		
		if (c == '\n')
		{
			iy++;
			ix = -1;
		}
		else if (c == '0')
		{
			m_vecMap[iy][ix] = 0;
		}
		else if (c == '1')
		{
			m_vecMap[iy][ix] = 1;
		}
	}
	
	fclose(fp);
}

void cMapLoader::Render()
{

	D3DXMATRIX matWorld, matT;

	for (size_t i = 0; i < m_vecMap.size(); ++i)
	{
		for (size_t j = 0; j < m_vecMap[i].size(); ++j)
		{
			
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixTranslation(&matT, i*2, -1.0f, j*2);
			matWorld *= matT;
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
				for each(auto p in m_vecMtlFloor)
				{
					g_pD3DDevice->SetMaterial(&(p->GetMtl()));
					g_pD3DDevice->SetTexture(0, p->GetTexture());
					m_pMeshFloor->DrawSubset(p->GetAttrID());
				}
			
			if(m_vecMap[i][j] == 1)
			{
				D3DXMatrixIdentity(&matWorld);
				D3DXMatrixTranslation(&matT, i * 2, 1.0f, j * 2+2);
				matWorld *= matT;
				g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
				for each(auto p in m_vecMtlWall)
				{
					g_pD3DDevice->SetMaterial(&(p->GetMtl()));
					g_pD3DDevice->SetTexture(0, p->GetTexture());
					m_pMeshWall->DrawSubset(p->GetAttrID());
				}
			}
		}
	}

}
