#include "StdAfx.h"
#include "cAseLoader.h"
#include "Asciitok.h"
#include "cMtlTex.h"
#include "cFrame.h"

cAseLoader::cAseLoader(void)
	: m_fp(0)
{
	D3DXMatrixIdentity(&rootMatrix);
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
			ProcessSCENE();
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
	}
	fclose(m_fp);

	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
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

void cAseLoader::ProcessSCENE()
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
		else if (IsEqual(szToken, ID_FIRSTFRAME))
		{
			
			m_Firstframe = GetInteger();
		}
		else if (IsEqual(szToken, ID_LASTFRAME))
		{
			m_Lastframe = GetInteger();
		}
		else if (IsEqual(szToken, ID_FRAMESPEED))
		{
			m_FrameSpeed = GetInteger();
		}
		else if (IsEqual(szToken, ID_TICKSPERFRAME))
		{
			m_Ticksperframe = GetInteger();
		}

	} while (nLevel > 0);
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
	std::string szparentName;
	std::string szNodeName;
	D3DXMATRIX*				parentsMatrix;

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
			szNodeName = GetToken();
			m_mapFrame[szNodeName] = pFrame;
			pFrame->SetFrameName(szNodeName);
			
		}
		else if (IsEqual(szToken, ID_NODE_PARENT)) 
		{
			
			szparentName = GetToken();
			m_mapFrame[szparentName]->AddChild(pFrame);
			
		}
		else if (IsEqual(szToken, ID_NODE_TM))
		{
			
			currentMatrix = ProcessNODETM();

			m_mapParensMatrix[szNodeName] = ProcessParentInverse(currentMatrix);
			auto itparent = m_mapParensMatrix.find(szparentName);

			if (itparent != m_mapParensMatrix.end()) {
				currentMatrix = (currentMatrix * (itparent->second));
			}
			
			pFrame->setTransForm(currentMatrix);
			currentMatrix = m_mapParensMatrix[szNodeName];
		}
		else if (IsEqual(szToken, ID_MESH)) 
		{
			ProcessMESH(pFrame);
		}
		else if (IsEqual(szToken, ID_MATERIAL_REF))
		{
			int ref = GetInteger();
			if (ref >= 0) {
				pFrame->SetMtlTex(m_vecMtlTex[ref]);
			}
		}
		else if (IsEqual(szToken, ID_TM_ANIMATION))
		{
			ProcessTM_ANIMATION(pFrame);
		}
	} while (nLevel > 0);

	return pFrame;
}

D3DXMATRIX cAseLoader::ProcessNODETM(void)
{

	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;


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
			_11 = GetFloat();
			_12 = GetFloat();
			_13 = GetFloat();
			_14 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW1))
		{
			_21 = GetFloat();
			_22 = GetFloat();
			_23 = GetFloat();
			_24 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW2))
		{
			_31 = GetFloat();
			_32 = GetFloat();
			_33 = GetFloat();
			_34 = 0.0f;
		}
		else if (IsEqual(szToken, ID_TM_ROW3))
		{
			_41 = GetFloat();
			_42 = GetFloat();
			_43 = GetFloat();
			_44 = 1.0f;
		}
	} while (nLevel > 0);

	D3DXMATRIX world(
		 _11,  _13,  _12,  _14,
		 _31,  _33,  _32,  _34,
		 _21,  _23,  _22,  _24,
		 _41,  _43,  _42,  _44);

	return world;
}

D3DXMATRIX cAseLoader::ProcessParentInverse(D3DXMATRIX mat)
{
	D3DXMATRIX inverse;

	D3DXMatrixInverse(&inverse, NULL, &mat);


	return inverse;
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
			ProcessMESH_VERTEX_LIST(currentMatrix,vecV);
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
			ProcessMESH_NORMALS(currentMatrix,vecVertex);
		}
	} while (nLevel > 0);



	pFrame->SetVertex(vecVertex);
}

void cAseLoader::ProcessMESH_VERTEX_LIST(IN D3DMATRIX wd ,OUT vector<D3DXVECTOR3>& vecV )
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
			float x, y, z;
			x = GetFloat();
			z = GetFloat();
			y = GetFloat();

		
			D3DXVECTOR3 localVertex(x, y, z);
			D3DXMATRIX parentsMatrix = wd;
			//D3DXMatrixInverse(&parentsMatrix, NULL, &parentsMatrix);
			D3DXVec3TransformCoord(&localVertex, &localVertex, &parentsMatrix);
			vecV[nIndex] = localVertex;

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

void cAseLoader::ProcessMESH_NORMALS(IN D3DMATRIX wd, OUT vector<ST_PNT_VERTEX>& vecVertex )
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
			

			D3DXVECTOR3 localVertex(x, y, z);
			D3DXMATRIX parentsMatrix = wd;
			D3DXMatrixInverse(&parentsMatrix, NULL, &parentsMatrix);
			D3DXVec3TransformNormal(&localVertex, &localVertex, &parentsMatrix);
		

			vecVertex[nFaceIndex * 3 + aModCount[nVCount]].n = localVertex;

			++nVCount;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessTM_ANIMATION(cFrame * pFrame)
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

void cAseLoader::ProcessPOS_TRACK(cFrame* pFrame)
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
			KeyFrameTranslation keyPos;
			keyPos.time = GetInteger();
			keyPos.translation.x = GetFloat();
			keyPos.translation.z = GetFloat();
			keyPos.translation.y = GetFloat();

			pFrame->GetkeyFramesTranslation().push_back(keyPos);
			//pFrame->GetkeyFramesTranslation().size();
		}
	} while (nLevel > 0);


}



void cAseLoader::ProcessROT_TRACK(cFrame* pFrame)
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
			KeyFrameRotation keyRot;
			D3DXQUATERNION quat;
			keyRot.time = GetInteger();
			float x = GetFloat();
			float z = GetFloat();
			float y = GetFloat();
			float w = GetFloat();

			quat.x = sinf(w / 2.0f)*x;
			quat.y = sinf(w / 2.0f)*y;
			quat.z = sinf(w / 2.0f)*z;
			quat.w = cosf(w / 2.0f);


			if (pFrame->GetkeyFramesRotation().empty()) {

				keyRot.quat = quat;
				pFrame->GetkeyFramesRotation().push_back(keyRot);
			}
			else
			{
				
				keyRot.quat = ((pFrame->GetkeyFramesRotation().back().quat)*(quat));
				pFrame->GetkeyFramesRotation().push_back(keyRot);
			}
		}
		

	} while (nLevel > 0);



}

