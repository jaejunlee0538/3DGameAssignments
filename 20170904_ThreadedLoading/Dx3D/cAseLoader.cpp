#include "StdAfx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cFrame.h"

cAseLoader::cAseLoader(void)
	: m_fp(0)
{
}

cAseLoader::~cAseLoader(void)
{
}

cFrame* cAseLoader::Load( char* szFullPath )
{
	cFrame* pRoot = NULL;

	fopen_s(&m_fp, szFullPath, "r");
	while(true)
	{
		char* szToken = GetToken();
		if (szToken == NULL) break;
		if (IsEqual(szToken, ID_SCENE))
		{
			SkipBlock();
		}
		else if (IsEqual(szToken, ID_MATERIAL_LIST))
		{
			ProcessMATERIAL_LIST();
		}
		else if (IsEqual(szToken, ID_GEOMETRY))
		{
			cFrame* pFrame = ProcessGEOMOBJECT();
			if(pRoot == NULL)
				pRoot = pFrame;
		}
		//OutputDebugString(szToken);
	}
	fclose(m_fp);

	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	if(pRoot)
	{
		pRoot->CalcLocalTM(NULL);
	}
	return pRoot;
}

char* cAseLoader::GetToken()
{
	bool isQuote = false;
	int nReadCount = 0;
	while(!feof(m_fp))
	{
		char c = fgetc(m_fp);
		if (c == '\"')
		{
			if(isQuote) break;
			isQuote = true;
			continue;
		}

		if (c < 33 && !isQuote)
		{
			if (nReadCount == 0) continue;
			break;
		}
		m_szToken[nReadCount++] = c;
	}

	if(nReadCount == 0)
		return NULL;
	
	m_szToken[nReadCount] = '\0';
	return m_szToken;
}

int cAseLoader::GetInteger()
{
	return atoi(GetToken());
}

float cAseLoader::GetFloat()
{
	return (float)atof(GetToken());
}

void cAseLoader::SkipBlock()
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
	} while (nLevel > 0);
}

bool cAseLoader::IsEqual( char* str1, char* str2 )
{
	return strcmp(str1, str2) == 0;
}

void cAseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MATERIAL_COUNT)) 
		{
			m_vecMtlTex.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MATERIAL)) 
		{
			int nMtlRef = GetInteger();
			SAFE_RELEASE(m_vecMtlTex[nMtlRef]);
			m_vecMtlTex[nMtlRef] = new cMtlTex;
			ProcessMATERIAL(m_vecMtlTex[nMtlRef]);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL( cMtlTex* pMtlTex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_AMBIENT)) 
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			float r = GetFloat();
			float g = GetFloat();
			float b = GetFloat();
			stMtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (IsEqual(szToken, ID_DIFFUSE)) 
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			float r = GetFloat();
			float g = GetFloat();
			float b = GetFloat();
			stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (IsEqual(szToken, ID_SPECULAR)) 
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			float r = GetFloat();
			float g = GetFloat();
			float b = GetFloat();
			stMtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
		}
		else if (IsEqual(szToken, ID_MAP_DIFFUSE)) 
		{
			ProcessMAP_DIFFUSE(pMtlTex);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMAP_DIFFUSE( cMtlTex* pMtlTex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_BITMAP)) 
		{
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(GetToken()));
		}
	} while (nLevel > 0);
}

cFrame* cAseLoader::ProcessGEOMOBJECT()
{
	cFrame* pFrame = NULL;

	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_NODE_NAME)) 
		{
			pFrame = new cFrame;
			char* szNodeName = GetToken();
			m_mapFrame[szNodeName] = pFrame;
		}
		else if (IsEqual(szToken, ID_NODE_PARENT)) 
		{
			m_mapFrame[GetToken()]->AddChild(pFrame);
		}
		else if (IsEqual(szToken, ID_NODE_TM)) 
		{
			ProcessNODE_TM(pFrame);
		}
		else if (IsEqual(szToken, ID_MESH)) 
		{
			ProcessMESH(pFrame);
		}
		else if (IsEqual(szToken, ID_MATERIAL_REF))
		{
			pFrame->SetMtlTex(m_vecMtlTex[GetInteger()]);
		}
		else if (IsEqual(szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(pFrame);
		}
	} while (nLevel > 0);

	return pFrame;
}

void cAseLoader::ProcessMESH( cFrame* pFrame )
{
	vector<D3DXVECTOR3>		vecV;
	vector<D3DXVECTOR2>		vecVT;
	vector<ST_PNT_VERTEX>	vecVertex;

	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_NUMVERTEX)) 
		{
			vecV.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MESH_NUMFACES)) 
		{
			vecVertex.resize(GetInteger() * 3);
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX_LIST)) 
		{
			ProcessMESH_VERTEX_LIST(vecV);
		}
		else if (IsEqual(szToken, ID_MESH_FACE_LIST)) 
		{
			ProcessMESH_FACE_LIST(vecV, vecVertex);
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVERTEX)) 
		{
			vecVT.resize(GetInteger());
		}
		else if (IsEqual(szToken, ID_MESH_TVERTLIST)) 
		{
			ProcessMESH_TVERTLIST(vecVT);
		}
		else if (IsEqual(szToken, ID_MESH_NUMTVFACES)) 
		{
			GetToken();
		}
		else if (IsEqual(szToken, ID_MESH_TFACELIST)) 
		{
			ProcessMESH_TFACELIST(vecVT, vecVertex);
		}
		else if (IsEqual(szToken, ID_MESH_NORMALS)) 
		{
			ProcessMESH_NORMALS(vecVertex);
		}
	} while (nLevel > 0);
	
	D3DXMATRIX& matWorld = pFrame->GetWorldTM();
	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &matWorld);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}
	pFrame->SetVertex(vecVertex);
}

void cAseLoader::ProcessMESH_VERTEX_LIST( OUT vector<D3DXVECTOR3>& vecV )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEX)) 
		{
			int nIndex = GetInteger();
			vecV[nIndex].x = GetFloat();
			vecV[nIndex].z = GetFloat();
			vecV[nIndex].y = GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_FACE_LIST( IN vector<D3DXVECTOR3>& vecV, 
	OUT vector<ST_PNT_VERTEX>& vecVertex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_FACE)) 
		{
			int nFaceIndex = GetInteger();
			GetToken(); // A:
			int nA = GetInteger();
			GetToken(); // B:
			int nB = GetInteger();
			GetToken(); // C:
			int nC = GetInteger();

			vecVertex[nFaceIndex * 3 + 0].p = vecV[nA];
			vecVertex[nFaceIndex * 3 + 1].p = vecV[nC];
			vecVertex[nFaceIndex * 3 + 2].p = vecV[nB];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TVERTLIST( OUT vector<D3DXVECTOR2>& vecVT )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_TVERT)) 
		{
			int nIndex = GetInteger();
			vecVT[nIndex].x = GetFloat();
			vecVT[nIndex].y = 1.0f - GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TFACELIST( IN vector<D3DXVECTOR2>& vecVT, OUT vector<ST_PNT_VERTEX>& vecVertex )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_TFACE)) 
		{
			int nFaceIndex = GetInteger();
			int nA = GetInteger();
			int nB = GetInteger();
			int nC = GetInteger();

			vecVertex[nFaceIndex * 3 + 0].t = vecVT[nA];
			vecVertex[nFaceIndex * 3 + 1].t = vecVT[nC];
			vecVertex[nFaceIndex * 3 + 2].t = vecVT[nB];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_NORMALS( OUT vector<ST_PNT_VERTEX>& vecVertex )
{
	int nVCount = 0;
	int aModCount[] = {0, 2, 1};
	int nFaceIndex = 0;
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_MESH_FACENORMAL)) 
		{
			nFaceIndex = GetInteger();
			nVCount = 0;
		}
		else if (IsEqual(szToken, ID_MESH_VERTEXNORMAL)) 
		{
			GetToken(); // index
			
			float x = GetFloat();
			float z = GetFloat();
			float y = GetFloat();

			vecVertex[nFaceIndex * 3 + aModCount[nVCount]].n = D3DXVECTOR3(x, y, z);
			++nVCount;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessNODE_TM( cFrame* pFrame )
{
	D3DXMATRIX& matWorld = pFrame->GetWorldTM();
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_TM_ROW0)) 
		{
			matWorld._11 = GetFloat();
			matWorld._13 = GetFloat();
			matWorld._12 = GetFloat();
			matWorld._14 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW1)) 
		{
			matWorld._31 = GetFloat();
			matWorld._33 = GetFloat();
			matWorld._32 = GetFloat();
			matWorld._34 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW2)) 
		{
			matWorld._21 = GetFloat();
			matWorld._23 = GetFloat();
			matWorld._22 = GetFloat();
			matWorld._24 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW3)) 
		{
			matWorld._41 = GetFloat();
			matWorld._43 = GetFloat();
			matWorld._42 = GetFloat();
			matWorld._44 = 1.0f;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessTM_ANIMATION( cFrame* pFrame )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_POS_TRACK)) 
		{
			ProcessPOS_TRACK(pFrame);
		}
		else if (IsEqual(szToken, ID_ROT_TRACK)) 
		{
			ProcessROT_TRACK(pFrame);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessPOS_TRACK( cFrame* pFrame )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_POS_SAMPLE)) 
		{
			ST_POS_SAMPLE s;
			s.n = GetInteger();

			s.p.x = GetFloat();
			s.p.z = GetFloat();
			s.p.y = GetFloat();
						
			pFrame->GetPosTrack().push_back(s);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessROT_TRACK( cFrame* pFrame )
{
	int nLevel = 0;
	do 
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{")) 
		{
			++nLevel;
		}
		else if (IsEqual(szToken, "}")) 
		{
			--nLevel;
		}
		else if (IsEqual(szToken, ID_ROT_SAMPLE)) 
		{
			ST_ROT_SAMPLE s;
			s.n = GetInteger();

			s.q.x = GetFloat();
			s.q.z = GetFloat();
			s.q.y = GetFloat();
			s.q.w = GetFloat();

			s.q.x = s.q.x * sinf(s.q.w / 2.0f);
			s.q.z = s.q.z * sinf(s.q.w / 2.0f);
			s.q.y = s.q.y * sinf(s.q.w / 2.0f);
			s.q.w = cosf(s.q.w / 2.0f);

			vector<ST_ROT_SAMPLE>& vecRotTrack = pFrame->GetRotTrack();
			if (!vecRotTrack.empty())
			{
				s.q = vecRotTrack.back().q * s.q;
			}
			pFrame->GetRotTrack().push_back(s);
		}
	} while (nLevel > 0);
}




