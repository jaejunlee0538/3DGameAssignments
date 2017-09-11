#include "stdafx.h"
#include "MeshRender.h"


MeshRender::MeshRender()
{
	m_vecPosition = D3DXVECTOR3(0, 0, 0);
}

MeshRender::~MeshRender()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pEffect);
}

bool MeshRender::Setup(std::string xFile, std::string texturePath, std::string effectFile)
{
	LPD3DXBUFFER pAdjacentBuffer;
	LPD3DXBUFFER pMaterialBuffer;
	DWORD numMaterial;
	if (FAILED(D3DXLoadMeshFromX(
		xFile.c_str(),
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		&pAdjacentBuffer,
		&pMaterialBuffer,
		NULL,
		&numMaterial,
		&m_pMesh))) {
		return false;
	}
	//assert(numMaterial == 1);
	//D3DXMATERIAL * mtl = (D3DXMATERIAL*)pMaterialBuffer->GetBufferPointer();
	//for (int i = 0; i < numMaterial; ++i) {
	//	m_texturePath = mtl->pTextureFilename;
	//}

	m_texturePath = texturePath;

	//Effect ·Îµå
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;

#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(g_pD3DDevice, effectFile.c_str(),
		NULL, NULL, dwShaderFlags, NULL, &m_pEffect, &pError);

	if (!m_pEffect && pError)
	{
		int size = pError->GetBufferSize();
		void *ack = pError->GetBufferPointer();

		if (ack)
		{
			char* str = new char[size];
			sprintf(str, (const char*)ack, size);
			OutputDebugString(str);
			delete[] str;
		}
		return false;
	}

	m_hTime = m_pEffect->GetParameterByName(0, "time");
	m_hMatWorld = m_pEffect->GetParameterByName(0, "matWorld");
	m_hMatView = m_pEffect->GetParameterByName(0, "matView");
	m_hMatProj = m_pEffect->GetParameterByName(0, "matProj");
	m_hTexture = m_pEffect->GetParameterByName(0, "Earth_Tex");
	return true;
}

void MeshRender::Update()
{

}

void MeshRender::Render()
{
	g_pD3DDevice->SetFVF(D3DFVF_XYZ);
	LPDIRECT3DVERTEXBUFFER9 vb;
	
	//vb->GetDesc()
	if (m_pEffect) {
		D3DXMATRIX matWorld, matView, matProj;
		D3DXMatrixTranslation(&matWorld, m_vecPosition.x, m_vecPosition.y, m_vecPosition.z);
		g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
		g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

		m_pEffect->SetFloat(m_hTime, g_pTimeManager->GetTime());
		m_pEffect->SetMatrix(m_hMatWorld, &matWorld);
		m_pEffect->SetMatrix(m_hMatView, &matView);
		m_pEffect->SetMatrix(m_hMatProj, &matProj);
		auto texture = g_pTextureManager->GetTexture(m_texturePath);
		m_pEffect->SetTexture(m_hTexture, texture);
		UINT numPass = 0;
		m_pEffect->Begin(&numPass, 0);
		for (UINT i = 0; i < numPass; ++i) {
			m_pEffect->BeginPass(i);
			m_pMesh->DrawSubset(0);
			m_pEffect->EndPass();
		}
		m_pEffect->End();
	}

}
