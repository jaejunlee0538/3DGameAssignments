#include "StdAfx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager(void)
{
}

cTextureManager::~cTextureManager(void)
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture( char* szFullPath )
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
				//�� ��ġ��, ��ȸ�� ��ũ���� ���� �ϰ� ���� �ʴ�.
				break;
			case D3DERR_OUTOFVIDEOMEMORY:
				//Microsoft�� Direct3D�� �� ó���� �ǽ��ϴµ� ����� ���÷��� �޸𸮰� ����.
				break;
			case D3DERR_INVALIDCALL:
				//�޼����� ȣ���� ��ȿ�̴�. ���� ���, �޼����� �Ķ���Ϳ� ��ȿ�� ���� �����Ǿ� �ִ� ��� ���̴�.
				break;
			case D3DXERR_INVALIDDATA:
				//�����Ͱ� ��ȿ�̴�.
				break;
			case E_OUTOFMEMORY:
				//Direct3D �� ȣ���� �Ϸ��ϱ� ���� ����� �޸𸮸� �Ҵ��� ���� ������.
				break;
			}
			return NULL;
		}

		m_mapTexture[szFullPath] = pTexture;
	}
	m_mapTexture[szFullPath]->AddRef();
	return m_mapTexture[szFullPath];
}

void cTextureManager::Destroy()
{
	for each(auto it in m_mapTexture)
	{
		SAFE_RELEASE(it.second);
	}
}
