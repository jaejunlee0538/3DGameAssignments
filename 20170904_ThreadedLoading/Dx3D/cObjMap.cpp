#include "StdAfx.h"
#include "cObjMap.h"


cObjMap::cObjMap(void)
{
}


cObjMap::~cObjMap(void)
{
}

void cObjMap::Load( char* szFullPath, D3DXMATRIX* pmat /*= NULL*/ )
{
	vector<D3DXVECTOR3> vecV;

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
		}
		else if(szBuf[0] == 'g')
		{
		}
		else if(szBuf[0] == 'v')
		{
			float x, y, z;
			if(szBuf[1] == 't')
			{
			}
			else if(szBuf[1] == 'n')
			{
			}
			else
			{
				sscanf_s(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if(szBuf[0] == 'u')
		{
		}
		else if(szBuf[0] == 'f')
		{
			int aIndex[3];
			sscanf_s(szBuf, "%*s %d/%*d/%*d  %d/%*d/%*d  %d/%*d/%*d",
				&aIndex[0], &aIndex[1], &aIndex[2]);

			for (size_t i = 0; i < 3; ++i)
			{
				D3DXVECTOR3 p = vecV[aIndex[i] - 1];
				if(pmat)
				{
					D3DXVec3TransformCoord(&p, &p, pmat);
				}
				m_vecSurface.push_back(p);
			}
		}
	}

	fclose(fp);
}

bool cObjMap::GetHeight(IN float x, OUT float& y, IN float z)
{
	D3DXVECTOR3 vRayPos(x, 1000.f, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);
	float u, v, d;
	for (size_t i = 0; i < m_vecSurface.size(); i += 3)
	{
		if(D3DXIntersectTri(&m_vecSurface[i],
			&m_vecSurface[i + 1],
			&m_vecSurface[i + 2],
			&vRayPos,
			&vRayDir, 
			&u, &v, &d))
		{
			y = 1000 - d;
			return true;
		}
	}
	return false;
}
