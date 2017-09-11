#pragma once

class cMtlTex;
class cGroup;

class cObjLoader
{
private:
	map<string, cMtlTex*> m_mapMtlTex;

public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(IN char* szFullPath, OUT vector<cGroup*>& vecGroup);
	bool LoadMesh(IN char* szFullPath, 
		OUT LPD3DXMESH* ppMesh,
		OUT vector<cMtlTex*>& vecMtlTex);

protected:
	void LoadMtlLib(char* szFullPath);
};

