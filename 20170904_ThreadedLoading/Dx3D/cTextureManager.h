#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

private:
	SINGLETON(cTextureManager);

public:
	LPDIRECT3DTEXTURE9 GetTexture(const char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(const string& sFullPath);
	void Destroy();
};

