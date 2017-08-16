#pragma once
#include "cMeshCreateHelper.h"
class cMtlTex;
class cFrame;

class cObjLoader
{
private:
	map<string, cMtlTex*> m_mapMtlTex;
	cMeshCreateHelper m_meshCreator;
public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(IN char* szFullPath, OUT vector<cFrame*>& vecGroup);
	void LoadMesh(IN char* szFullPath, D3DXMATRIX* vertMat, OUT cFrame** meshObject);
private:

protected:
	void LoadMtlLib(char* szFullPath);
};

