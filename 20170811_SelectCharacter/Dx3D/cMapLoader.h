#pragma once
class cMapLoader
{
	
private:

	FILE*			m_fp;
	char			m_szToken[1024];
	//vector<char>	m_orderNum;


public:
	cMapLoader();
	~cMapLoader();

	void Load(IN char * szFullPath, OUT vector<vector<int>>& obj);
};

