#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;

private:
	SINGLETON(cTextureManager);

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szFullPath);
	void Destroy();
};

