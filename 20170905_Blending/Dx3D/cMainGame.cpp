#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#define CASE_NUM	1
cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);

	g_pFontManager->Destroy();
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);
#if CASE_NUM == 1
	D3DCOLOR colorA = D3DCOLOR_ARGB(150, 100, 0, 255);
	D3DCOLOR colorB = D3DCOLOR_ARGB(150, 18, 255, 80);
#elif CASE_NUM == 2
	D3DCOLOR colorA = D3DCOLOR_ARGB(150, 100, 0, 255);
	D3DCOLOR colorB = D3DCOLOR_ARGB(150, 18, 255, 80);
#elif CASE_NUM == 3
	D3DCOLOR colorA = D3DCOLOR_ARGB(150, 100, 0, 255);
	D3DCOLOR colorB = D3DCOLOR_ARGB(150, 18, 255, 80);
#elif CASE_NUM == 4
	D3DCOLOR colorA = D3DCOLOR_ARGB(150, 100, 100, 255);
	D3DCOLOR colorB = D3DCOLOR_ARGB(150, 255, 255, 100);
#elif CASE_NUM == 5
	D3DCOLOR colorA = D3DCOLOR_ARGB(150, 100, 100, 255);
	D3DCOLOR colorB = D3DCOLOR_ARGB(150, 255, 255, 100);
#endif
	m_vecVertex1.resize(6);
	m_vecVertex2.resize(6);

	m_vecVertex1[0].p = D3DXVECTOR3(-1, -1, 0);
	m_vecVertex1[0].t = D3DXVECTOR2( 0, 1);
	m_vecVertex1[0].c = colorA;

	m_vecVertex1[1].p = D3DXVECTOR3(-1,  1, 0);
	m_vecVertex1[1].t = D3DXVECTOR2( 0, 0);
	m_vecVertex1[1].c = colorA;

	m_vecVertex1[2].p = D3DXVECTOR3( 1,  1, 0);
	m_vecVertex1[2].t = D3DXVECTOR2( 1, 0);
	m_vecVertex1[2].c = colorA;

	m_vecVertex1[3].p = D3DXVECTOR3(-1, -1, 0);
	m_vecVertex1[3].t = D3DXVECTOR2( 0, 1);
	m_vecVertex1[3].c = colorA;

	m_vecVertex1[4].p = D3DXVECTOR3( 1,  1, 0);
	m_vecVertex1[4].t = D3DXVECTOR2( 1, 0);
	m_vecVertex1[4].c = colorA;

	m_vecVertex1[5].p = D3DXVECTOR3( 1, -1, 0);
	m_vecVertex1[5].t = D3DXVECTOR2( 1, 1);
	m_vecVertex1[5].c = colorA;

	for (size_t i = 0; i < m_vecVertex1.size(); ++i)
	{
		m_vecVertex2[i] = m_vecVertex1[i];
		m_vecVertex2[i].c = colorB;
	}

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	SetLight();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if (m_pCamera) m_pCamera->Update();

	g_pAutoReleasePool->Drain();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		//D3DCOLOR_XRGB(47, 121, 112),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	//if(m_pGrid) m_pGrid->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADD);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);

	// D3DBLENDOP_ADD (기본값)	: src + dest
	// D3DBLENDOP_SUBTRACT		: src - dest
	// D3DBLENDOP_REVSUBTRACT	: dest - src
	// D3DBLENDOP_MIN			: MIN(src, dest)
	// D3DBLENDOP_MAX			: MAX(src, dest)
	
	
	// D3DBLEND_ZERO : (0, 0, 0, 0)
	// D3DBLEND_ONE : (1, 1, 1, 1)
	// D3DBLEND_SRCCOLOR : (rs, gs, bs, as)
	// D3DBLEND_INVSRCCOLOR : (1-rs, 1-gs, 1-bs, 1-as)
	// D3DBLEND_SRCALPHA : (as, as, as, as) (Source Blend 기본값)
	// D3DBLEND_INVSRCALPHA : (1-as, 1-as, 1-as, 1-as) (Destination Blend 기본값)
	// D3DBLEND_DESTALPHA : (ad, ad, ad, ad)
	// D3DBLEND_INVDESTALPHA : (1-ad, 1-ad, 1-ad, 1-ad)
	// D3DBLEND_DESTCOLOR : (rd, gd, bd, ad)
	// D3DBLEND_INVDESTCCOLOR : (1-rd, 1-gd, 1-bd, 1-ad)
	// D3DBLEND_SRCALPHASAT : (f, f, f, 1); f=min(as, 1-ad)
	

#if CASE_NUM == 1
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
#elif CASE_NUM == 2
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MAX);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
#elif CASE_NUM == 3
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_BLENDFACTOR, D3DCOLOR_ARGB(0,255,0,0));
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
#elif CASE_NUM == 4
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
#elif CASE_NUM == 5
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
#endif

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PCT_VERTEX::FVF);

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	matWorld._43 = 0;
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecVertex1[0], sizeof(ST_PCT_VERTEX));

	matWorld._41 = 0.5;
	matWorld._42 = 0.5;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecVertex2[0], sizeof(ST_PCT_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


void cMainGame::MsgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
	{
		m_pCamera->MsgProc(hWnd, message, wParam, lParam);
	}
	switch (message)
	{
	case WM_LBUTTONDOWN:
		{
		}
		break;
	case WM_KEYDOWN:
		switch(wParam) {
		case VK_SPACE:
			{
				//static int n = 0;
				//m_pSkinnedMesh->SetAnimationIndex(++n);
			}
			break;
		}
		break;
	}
}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vDir( 1,-1, 1);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}
