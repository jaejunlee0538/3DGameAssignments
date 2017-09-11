#include "StdAfx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager(void)
{
}

cTextureManager::~cTextureManager(void)
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( const char* szFullPath )
{
	if(m_mapTexture.find(szFullPath) == m_mapTexture.end())
	{
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		
		HRESULT hr = D3DXCreateTextureFromFile(g_pD3DDevice, szFullPath, &pTexture);
		
		if(hr != D3D_OK)
		{
			switch(hr)
			{
			case D3DERR_NOTAVAILABLE:
				//이 장치는, 조회된 테크닉을 지원 하고 있지 않다.
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
				//Microsoft® Direct3D® 가 처리를 실시하는데 충분한 디스플레이 메모리가 없다.
				break;
			case D3DERR_INVALIDCALL:
				//메서드의 호출이 무효이다. 예를 들어, 메서드의 파라미터에 무효인 값이 설정되어 있는 경우 등이다.
				break;
			case D3DXERR_INVALIDDATA:
				//데이터가 무효이다.
				break;
			case E_OUTOFMEMORY:
				//Direct3D 가 호출을 완료하기 위한 충분한 메모리를 할당할 수가 없었다.
				break;
			}
			return NULL;
		}

		m_mapTexture[szFullPath] = pTexture;
	}

	return m_mapTexture[szFullPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( const string& sFullPath )
{
	return GetTexture(sFullPath.c_str());
}

void cTextureManager::Destroy()
{
	for each(auto it in m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTextureEx( const char* szFullPath, OUT D3DXIMAGE_INFO* pImageInfo )
{
	if(m_mapTexture.find(szFullPath) == m_mapTexture.end())
	{
		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			szFullPath, 
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN, 
			D3DPOOL_MANAGED, 
			D3DX_FILTER_NONE, 
			D3DX_DEFAULT, 
			0,
			&m_mapImageInfo[szFullPath],
			NULL,
			&m_mapTexture[szFullPath] );
	}
	else if(m_mapImageInfo.find(szFullPath) == m_mapImageInfo.end())
	{
		assert(true && "이전에 로드된적 있음~!");
	}

	if (pImageInfo)
	{
		*pImageInfo = m_mapImageInfo[szFullPath];
	}

	return m_mapTexture[szFullPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTextureEx( const string& sFullPath, OUT D3DXIMAGE_INFO* pImageInfo )
{
	return GetTextureEx(sFullPath.c_str(), pImageInfo);
}
