#include "StdAfx.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cFrame.h"

cObjLoader::cObjLoader(void)
{
}

cObjLoader::~cObjLoader(void)
{
}

void cObjLoader::Load(IN char* szFullPath, OUT vector<cFrame*>& vecGroup)
{
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
				cFrame* pFrame = new cFrame();
				pFrame->SetVertex(vecVertex);
				vecGroup.push_back(pFrame);
				vecVertex.clear();
				
				pFrame->SetMtlTex(m_mapMtlTex[szMtlName]);
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
	m_mapMtlTex.clear();
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
			
			assert(m_mapMtlTex[szMtlName] == NULL);

			m_mapMtlTex[szMtlName] = new cMtlTex;
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
			else if (szBuf[1] == 'e') {
				sscanf_s(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Emissive = D3DXCOLOR(r, g, b, 1.f);
			}
		}
		else if(szBuf[0] == 'm')
		{
			char szTexturePath[1024] = {'\0', };
			sscanf_s(szBuf, "%*s %s", szTexturePath, 1024);
			LPDIRECT3DTEXTURE9 pTex = g_pTextureManager->GetTexture(szTexturePath);
			m_mapMtlTex[szMtlName]->SetTexture(pTex);
		}
		else if (szBuf[0] == 'd') {
			D3DMATERIAL9& stMtl = m_mapMtlTex[szMtlName]->GetMtl();
			float power;
			sscanf_s(szBuf, "%*s %f", &power);
			stMtl.Power = power;
		}
	}
	
	fclose(fp);
}

