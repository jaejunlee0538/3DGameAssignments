#pragma once

class cMtlTex;
class cFrame;

class cAseLoader
{
private:
	FILE*					m_fp;
	char					m_szToken[1024];
	vector<cMtlTex*>		m_vecMtlTex;
	map<string, cFrame*>	m_mapFrame;

public:
	cAseLoader(void);
	~cAseLoader(void);

	cFrame*	Load(char* szFullPath);
	char*	GetToken();
	int		GetInteger();
	float	GetFloat();
	void	SkipBlock();
	bool	IsEqual(char* str1, char* str2);
	void	ProcessMATERIAL_LIST();
	void	ProcessMATERIAL(cMtlTex* pMtlTex);
	void	ProcessMAP_DIFFUSE(cMtlTex* pMtlTex);
	cFrame* ProcessGEOMOBJECT();
	void	ProcessMESH(cFrame* pFrame);
	void	ProcessMESH_VERTEX_LIST(OUT vector<D3DXVECTOR3>& vecV);
	void	ProcessMESH_FACE_LIST(IN vector<D3DXVECTOR3>& vecV, OUT vector<ST_PNT_VERTEX>& vecVertex);
	void	ProcessMESH_TVERTLIST(OUT vector<D3DXVECTOR2>& vecVT);
	void	ProcessMESH_TFACELIST(IN vector<D3DXVECTOR2>& vecVT, OUT vector<ST_PNT_VERTEX>& vecVertex);
	void	ProcessMESH_NORMALS(OUT vector<ST_PNT_VERTEX>& vecVertex);
	void	ProcessNODE_TM(cFrame* pFrame);
	void	ProcessTM_ANIMATION(cFrame* pFrame);
	void	ProcessPOS_TRACK(cFrame* pFrame);
	void	ProcessROT_TRACK(cFrame* pFrame);
};


