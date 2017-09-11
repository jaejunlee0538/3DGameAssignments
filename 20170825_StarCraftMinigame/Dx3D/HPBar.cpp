#include "stdafx.h"
#include "HPBar.h"


HPBar::HPBar()
	:m_pVBBkg(NULL), m_pVBHP(NULL)
{
}

HPBar::~HPBar()
{
	SAFE_RELEASE(m_pVBBkg);
	SAFE_RELEASE(m_pVBHP);
}

void HPBar::Init(float length, float height, float maxHP)
{
	m_currentHP = maxHP;
	m_maxHP = maxHP;
	m_scales.x = length;
	m_scales.y = height;
	m_scales.z = 1.0f;


	g_pD3DDevice->CreateVertexBuffer(
		sizeof(ST_PC_VERTEX) * 6, D3DUSAGE_WRITEONLY, ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED, &m_pVBBkg, NULL);
	g_pD3DDevice->CreateVertexBuffer(
		sizeof(ST_PC_VERTEX) * 6, D3DUSAGE_WRITEONLY, ST_PC_VERTEX::FVF,
		D3DPOOL_MANAGED, &m_pVBHP, NULL);

	D3DXVECTOR3 vertices[6] = {
		D3DXVECTOR3(-0.5f, 0.5f,0.0f),D3DXVECTOR3(0.5f, -0.5f,0.0f),D3DXVECTOR3(0.5f, 0.5f,0.0f),
		D3DXVECTOR3(-0.5f, 0.5f,0.0f),D3DXVECTOR3(-0.5f, -0.5f,0.0f),D3DXVECTOR3(0.5f, -0.5f,0.0f)
	};

	ST_PC_VERTEX *pV = NULL;
	m_pVBBkg->Lock(0, 0,(LPVOID*)&pV,0);
	for (int i = 0; i < 6; ++i) {
		pV->p = vertices[i];
		pV->c = D3DCOLOR_XRGB(50, 150, 50);
		++pV;
	}
	m_pVBBkg->Unlock();
	
	pV = NULL;
	m_pVBHP->Lock(0, 0, (LPVOID*)&pV, 0);
	for (int i = 0; i < 6; ++i) {
		pV->p = vertices[i] - D3DXVECTOR3(0.5f,0,0);
		pV->c = D3DCOLOR_XRGB(200, 50, 50);
		++pV;
	}
	m_pVBHP->Unlock();
}

void HPBar::UpdatePosition(D3DXVECTOR3 position)
{
	m_position = position;
}

void HPBar::UpdateCameraPosition(D3DXVECTOR3 position)
{
	m_cameraPosition = position;
}

void HPBar::UpdateCurrentHP(float currentHP)
{
	m_currentHP = currentHP;
}

void HPBar::Render()
{
	DWORD prevLighting;
	g_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &prevLighting);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	g_pD3DDevice->SetTexture(0, 0);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &GetBkgBarMatrix());
	g_pD3DDevice->SetStreamSource(0, m_pVBBkg, 0, sizeof(ST_PC_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &GetHPBarMatrix());
	g_pD3DDevice->SetStreamSource(0, m_pVBHP, 0, sizeof(ST_PC_VERTEX));
	g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, prevLighting);
}

D3DXMATRIX HPBar::GetBkgBarMatrix() const
{
	D3DXMATRIX matLookCam, matS;
	D3DXMatrixLookAtLH(&matLookCam, &m_position, &m_cameraPosition, &D3DXVECTOR3(0,1,0));
	D3DXMatrixInverse(&matLookCam, NULL, &matLookCam);
	D3DXMatrixScaling(&matS, m_scales.x, m_scales.y, m_scales.z);
	return matS * matLookCam;
}

D3DXMATRIX HPBar::GetHPBarMatrix() const
{
	D3DXMATRIX matLookCam, matS, matLocalOffset;
	D3DXMatrixTranslation(&matLocalOffset, m_scales.x*0.5, 0, 0);
	D3DXMatrixLookAtLH(&matLookCam, &m_position, &m_cameraPosition, &D3DXVECTOR3(0, 1, 0));
	D3DXMatrixInverse(&matLookCam, NULL, &matLookCam);
	D3DXMatrixScaling(&matS, m_scales.x*m_currentHP/m_maxHP, m_scales.y, m_scales.z);
	return matS * matLocalOffset*matLookCam;
}
