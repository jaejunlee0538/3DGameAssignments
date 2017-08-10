#pragma once

class cMtlTex;
class cFrame;

class cObjLoader
{
private:
	map<string, cMtlTex*> m_mapMtlTex;

public:
	cObjLoader(void);
	~cObjLoader(void);

	void Load(IN char* szFullPath, OUT vector<cFrame*>& vecGroup);
protected:
	void LoadMtlLib(char* szFullPath);
};

