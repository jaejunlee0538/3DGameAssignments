#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	map<string, LPDIRECT3DTEXTURE9> m_mapTexture;
	map<string, D3DXIMAGE_INFO>		m_mapImageInfo;

private:
	SINGLETON(cTextureManager);

public:
	LPDIRECT3DTEXTURE9 GetTexture(const char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(const string& sFullPath);

	LPDIRECT3DTEXTURE9 GetTextureEx(const char* szFullPath, OUT D3DXIMAGE_INFO* pImageInfo);
	LPDIRECT3DTEXTURE9 GetTextureEx(const string& sFullPath, OUT D3DXIMAGE_INFO* pImageInfo);

	void Destroy();
};

