#include "StdAfx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"

cObjLoader::cObjLoader(void)
{
}

cObjLoader::~cObjLoader(void)
{
}

void cObjLoader::Load(IN char* szFullPath, OUT vector<cGroup*>& vecGroup)
{
	m_mapMtlTex.clear();

	vector<D3DXVECTOR3> vecV;
	vector<D3DXVECTOR2> vecVT;
	vector<D3DXVECTOR3> vecVN;
	vector<ST_PNT_VERTEX> vecVertex;

	char szMtlName[1024] = {'\0', };
	
	FILE* fp = 0;
	fopen_s(&fp, szFullPath, "r");

	while(!feof(fp))
	{
		char szBuf[1024] = {'\0', };
		fgets(szBuf, 1024, fp);
		
		if (strlen(szBuf) == 0)
			continue;
		
		if(szBuf[0] == '#')
		{
			continue;
		}
		else if(szBuf[0] == 'm')
		{
			char szPath[1024] = {'\0', };
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			LoadMtlLib(szPath);
		}
		else if(szBuf[0] == 'g')
		{
			if( !vecVertex.empty() )
			{
				cGroup* pGroup = new cGroup;
				pGroup->SetVertexBuffer(vecVertex);
				vecGroup.push_back(pGroup);
				vecVertex.clear();
				
				pGroup->SetMtlTex(m_mapMtlTex[szMtlName]);
			}
		}
		else if(szBuf[0] == 'v')
		{
			float x, y, z;
			if(szBuf[1] == 't')
			{
				sscanf_s(szBuf, "%*s %f %f %*f", &x, &y);
				vecVT.push_back(D3DXVECTOR2(x, y));
			}
			else if(szBuf[1] == 'n')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if(szBuf[0] == 'u')
		{
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
		}
		else if(szBuf[0] == 'f')
		{
			int aIndex[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d  %d/%d/%d  %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);
			
			for (size_t i = 0; i < 3; ++i)
			{
				D3DXVECTOR3 p = vecV[aIndex[i][0] - 1];
				D3DXVECTOR2 t = vecVT[aIndex[i][1] - 1];
				D3DXVECTOR3 n = vecVN[aIndex[i][2] - 1];
				vecVertex.push_back(ST_PNT_VERTEX(p, n, t));
			}
		}
	}

	for each(auto it in m_mapMtlTex)
	{
		SAFE_RELEASE(it.second);
	}

	fclose(fp);
}

void cObjLoader::LoadMtlLib( char* szFullPath )
{
	FILE* fp = 0;
	fopen_s(&fp, szFullPath, "r");

	char szMtlName[1024] = {'\0', };

	while(!feof(fp))
	{
		char szBuf[1024] = {'\0', };
		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		if(szBuf[0] == '#')
		{
			continue;
		}
		else if(szBuf[0] == 'n')
		{
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
			
			assert(strlen(szMtlName) > 0);
			
			int nAttrID = m_mapMtlTex.size();

			assert(m_mapMtlTex[szMtlName] == NULL);
			
			m_mapMtlTex[szMtlName] = new cMtlTex;
			m_mapMtlTex[szMtlName]->SetAttrID(nAttrID);
		}
		else if(szBuf[0] == 'K')
		{
			D3DMATERIAL9& stMtl = m_mapMtlTex[szMtlName]->GetMtl();

			float r, g, b;

			if(szBuf[1] == 'a')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Ambient = D3DXCOLOR(r, g, b, 1.f);
			}
			else if(szBuf[1] == 'd')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.f);
			}
			else if(szBuf[1] == 's')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Specular = D3DXCOLOR(r, g, b, 1.f);
			}
		}
		else if(szBuf[0] == 'm')
		{
			char szTexturePath[1024] = {'\0', };
			sscanf_s(szBuf, "%*s %s", szTexturePath, 1024);
			LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(szTexturePath);
			m_mapMtlTex[szMtlName]->SetTexture(pTex);
		}
	}
	
	fclose(fp);
}

bool cObjLoader::LoadMesh( IN char* szFullPath,
	OUT LPD3DXMESH* ppMesh,
	OUT vector<cMtlTex*>& vecMtlTex )
{
	m_mapMtlTex.clear();

	vector<DWORD>		vecA;
	vector<D3DXVECTOR3> vecV;
	vector<D3DXVECTOR2> vecVT;
	vector<D3DXVECTOR3> vecVN;
	vector<ST_PNT_VERTEX> vecVertex;

	char szMtlName[1024] = {'\0', };

	FILE* fp = 0;
	fopen_s(&fp, szFullPath, "r");

	while(!feof(fp))
	{
		char szBuf[1024] = {'\0', };
		fgets(szBuf, 1024, fp);

		if (strlen(szBuf) == 0)
			continue;

		if(szBuf[0] == '#')
		{
			continue;
		}
		else if(szBuf[0] == 'm')
		{
			char szPath[1024] = {'\0', };
			sscanf_s(szBuf, "%*s %s", szPath, 1024);
			LoadMtlLib(szPath);
		}
		else if(szBuf[0] == 'g') {}
		else if(szBuf[0] == 'v')
		{
			float x, y, z;
			if(szBuf[1] == 't')
			{
				sscanf_s(szBuf, "%*s %f %f %*f", &x, &y);
				vecVT.push_back(D3DXVECTOR2(x, y));
			}
			else if(szBuf[1] == 'n')
			{
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if(szBuf[0] == 'u')
		{
			sscanf_s(szBuf, "%*s %s", szMtlName, 1024);
		}
		else if(szBuf[0] == 'f')
		{
			int aIndex[3][3];
			sscanf_s(szBuf, "%*s %d/%d/%d  %d/%d/%d  %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

			for (size_t i = 0; i < 3; ++i)
			{
				D3DXVECTOR3 p = vecV[aIndex[i][0] - 1];
				D3DXVECTOR2 t = vecVT[aIndex[i][1] - 1];
				D3DXVECTOR3 n = vecVN[aIndex[i][2] - 1];
				vecVertex.push_back(ST_PNT_VERTEX(p, n, t));
			}

			vecA.push_back(m_mapMtlTex[szMtlName]->GetAttrID());
		}
	}

	vecMtlTex.resize(m_mapMtlTex.size());
	for each(auto it in m_mapMtlTex)
	{
		vecMtlTex[it.second->GetAttrID()] = it.second;
	}

	LPD3DXMESH pMesh = NULL;
	D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&pMesh);

	if (pMesh == NULL) return false;

	ST_PNT_VERTEX* pV = 0;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	pMesh->UnlockVertexBuffer();

	WORD* pI = 0;
	pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		pI[i] = i;
	}
	pMesh->UnlockIndexBuffer();

	DWORD* pA = 0;
	pMesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecA[0], vecA.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();

	vector<DWORD> vecAdj(vecVertex.size());
	pMesh->GenerateAdjacency(FLT_EPSILON, &vecAdj[0]);

	pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT | 
		D3DXMESHOPT_COMPACT | 
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0],
		NULL,
		NULL,
		NULL);

	*ppMesh = pMesh;
	fclose(fp);
}
