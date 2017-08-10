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
	map<string, D3DMATRIX>	m_mapParensMatrix;
	D3DXMATRIX				currentMatrix;
	D3DXMATRIX				currMatrix;
	D3DXMATRIX				rootMatrix;
	D3DXMATRIX*				parentMatrix;
	int						m_Firstframe;
	int						m_Lastframe;
	int						m_FrameSpeed;
	int						m_Ticksperframe;

public:
	cAseLoader(void);
	~cAseLoader(void);

	cFrame*	Load(char* szFullPath);
	char*	GetToken();
	int		GetInteger();
	float	GetFloat();
	void	SkipBlock();
	bool	IsEqual(char* str1, char* str2);
	void	ProcessSCENE();
	void	ProcessMATERIAL_LIST();
	void	ProcessMATERIAL(cMtlTex* pMtlTex);
	void	ProcessMAP_DIFFUSE(cMtlTex* pMtlTex);
	cFrame* ProcessGEOMOBJECT();
	D3DXMATRIX	ProcessNODETM(void);
	D3DXMATRIX	ProcessParentInverse(D3DXMATRIX mat);
	void	ProcessMESH(cFrame* pFrame);
	void	ProcessMESH_VERTEX_LIST(IN D3DMATRIX wd, OUT vector<D3DXVECTOR3>& vecV);
	void	ProcessMESH_FACE_LIST(IN vector<D3DXVECTOR3>& vecV, OUT vector<ST_PNT_VERTEX>& vecVertex);
	void	ProcessMESH_TVERTLIST(OUT vector<D3DXVECTOR2>& vecVT);
	void	ProcessMESH_TFACELIST(IN vector<D3DXVECTOR2>& vecVT, OUT vector<ST_PNT_VERTEX>& vecVertex);
	void	ProcessMESH_NORMALS(IN D3DMATRIX wd,OUT vector<ST_PNT_VERTEX>& vecVertex);
	void	ProcessTM_ANIMATION(cFrame* pFrame);
	void	ProcessPOS_TRACK(cFrame* pFrame);
	void	ProcessROT_TRACK(cFrame* pFrame);
	D3DXMATRIX* getParentMatrix(void) { return parentMatrix; }

	int getFirstframe(void) { return m_Firstframe; }
	int getLastframe(void) { return m_Lastframe; }
	int getFrameSpeed(void) { return m_FrameSpeed; }
	int getTicksperframe(void) { return m_Ticksperframe; }
	
};


