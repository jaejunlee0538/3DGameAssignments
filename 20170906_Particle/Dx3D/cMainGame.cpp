#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "SparkParticle.h"
#include "cKeyManager.h"

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

cMainGame::cMainGame(void)
	: m_pCamera(NULL),
	m_pSparkParticle(NULL),
	m_pFireParticle(NULL),
	m_pSmokeParticle(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pSparkParticle);
	SAFE_DELETE(m_pFireParticle);
	SAFE_DELETE(m_pSmokeParticle);
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();

}

void cMainGame::Setup()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);

	m_vecVertex.resize(1000);
	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		float fRadius = rand() % 100 / 10.0f;

		m_vecVertex[i].p = D3DXVECTOR3(0, 0, fRadius);


		D3DXVECTOR3 vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));

		D3DXMATRIX matRX, matRY, matRZ, matWorld;
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ;

		D3DXVec3TransformCoord(
			&m_vecVertex[i].p,
			&m_vecVertex[i].p,
			&matWorld);

		m_vecVertex[i].c = D3DCOLOR_ARGB(255, 180, 70, 20);
		//m_vecParticle[i]->m_pVertex = &m_vecVertex[i];
	}

	// 포인트를 확대 축소 할 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE); 

	//// 포인트 사이즈 설정
	//g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(1.f));

	////// 포인트 스케일링 Factor값 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	//// 포인트 최소 크기
	//g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));

	//// 포인트 최대 크기
	//g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(1000.0f));

	////// 포인트에 텍스쳐를 입힐 수 있게 해줌
	//g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 텍스쳐 알파 옵션 설정
	//g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );  
	//g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );  
	//g_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );  

	InitSimpleParticle();
	InitFire();
	InitSmoke();
	SetLight();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if (g_pKeyManager->isOnceKeyDown(VK_SPACE)) {
		if (m_pSparkParticle->IsPaused()) {
			m_pSparkParticle->Resume();
		}
		else {
			m_pSparkParticle->Pause();
		}
		if (m_pFireParticle->IsPaused()) {
			m_pFireParticle->Resume();
		}
		else {
			m_pFireParticle->Pause();
		}
		if (m_pSmokeParticle->IsPaused()) {
			m_pSmokeParticle->Resume();
		}
		else {
			m_pSmokeParticle->Pause();
		}
	}
	m_pFireParticle->Update();
	m_pSparkParticle->Update();
	m_pSmokeParticle->Update();
	if (m_pCamera) m_pCamera->Update();
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

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	m_pSparkParticle->Render();
	m_pFireParticle->Render();
	m_pSmokeParticle->Render();
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

void cMainGame::InitSimpleParticle()
{
	m_pSparkParticle = new SparkParticle();
	m_pSparkParticle->Init(300, D3DXVECTOR3(0, 0, 0));
	m_pSparkParticle->m_bLoop = true;
	m_pSparkParticle->m_bGravity = true;
	m_pSparkParticle->m_iNParticlesPerSec = 250;
	m_pSparkParticle->m_fInitSize = 0.001f;
	m_pSparkParticle->m_bColorBySpeed = true;
	m_pSparkParticle->m_vec4ColorForSpeed1 = D3DXVECTOR4(1, 1, 1, 1);
	m_pSparkParticle->m_vec4ColorForSpeed0 = D3DXVECTOR4(1, 0, 0, 0);
	m_pSparkParticle->m_fSpeed0 = 0.0f;
	m_pSparkParticle->m_fSpeed1 = 3.0f;
	m_pSparkParticle->m_funcInitSizeNoise = []() {
		return rand() % 100 * 0.001;
	};
	m_pSparkParticle->m_pTexture = g_pTextureManager->GetTexture("SparkTexture2.png");
	m_pSparkParticle->m_funcPositionNoise = [](float t) ->D3DXVECTOR3 {
		return D3DXVECTOR3(rand() % 1000 * 0.001, rand() % 1000 * 0.00001, rand() % 1000 * 0.001);
	};
	m_pSparkParticle->m_funcDirectionNoise = [](float t) ->D3DXVECTOR3 {
		return D3DXVECTOR3(rand() % 1000 * 0.001 - 0.5, rand() % 1000 * 0.001 - 0.5, rand() % 1000 * 0.001 - 0.5);
	};
	m_pSparkParticle->Start();
}

void cMainGame::InitFire()
{
	m_pFireParticle = new SparkParticle();
	m_pFireParticle->Init(600, D3DXVECTOR3(-2.0, 0, 0));
	m_pFireParticle->m_iNParticlesPerSec = 200;
	m_pFireParticle->m_fInitSize = 2.0f;
	m_pFireParticle->m_fInitEmitSpeed = 5.0f;
	m_pFireParticle->m_fInitLifeTime = 3.0f;
	m_pFireParticle->m_bGravity = true;
	m_pFireParticle->m_vec3Gravity = D3DXVECTOR3(2.0f, 0.0f, 0.0f);
	m_pFireParticle->m_bColorBySpeed = true;
	m_pFireParticle->m_vec4ColorForSpeed1 = D3DXVECTOR4(1, 1, 1, 1);
	m_pFireParticle->m_vec4ColorForSpeed0 = D3DXVECTOR4(1, 0, 0, 0);
	m_pFireParticle->m_fSpeed0 = 0.0f;
	m_pFireParticle->m_fSpeed1 = 3.0f;
	m_pFireParticle->m_bSizeOverTime = true;
	m_pFireParticle->m_vec4InitColor = D3DXVECTOR4(0.8f, 0.2f, 0.2f, 1.0f);
	m_pFireParticle->m_bLoop = true;
	m_pFireParticle->m_bColorOverTime = true;
	//m_pFireParticle->m_bGravity = true;
	m_pFireParticle->m_vec4ColorLifeTime0 = D3DXVECTOR4(1.0f, 0.5f, 0.5f, 0.8f);
	m_pFireParticle->m_vec4ColorLifeTime1 = D3DXVECTOR4(0.1f, 0.1f, 1.0f, 0.1f);

	m_pFireParticle->m_funcPositionNoise = [](float t) ->D3DXVECTOR3 {
		return D3DXVECTOR3(rand() % 1000 * 0.002 - 1,
			rand() % 1000 * 0.0001 - 0.05,
			rand() % 1000 * 0.002 - 1);
	};
	m_pFireParticle->m_pTexture = g_pTextureManager->GetTexture("flame_bubble.png");
	m_pFireParticle->Start();
}

void cMainGame::InitSmoke()
{
	m_pSmokeParticle = new SparkParticle();
	m_pSmokeParticle->Init(4000, D3DXVECTOR3(2.0, 0, 0));
	m_pSmokeParticle->m_iNParticlesPerSec = 2000;
	m_pSmokeParticle->m_fInitSize = 1.0f;
	m_pSmokeParticle->m_fInitEmitSpeed = 3.0f;
	m_pSmokeParticle->m_fInitLifeTime = 2.0f;
	m_pSmokeParticle->m_bGravity = true;
	m_pSmokeParticle->m_vec3Gravity = D3DXVECTOR3(0.0f, -6.0f, 0.0f);
	m_pSmokeParticle->m_bSizeOverTime = true;
	m_pSmokeParticle->m_vec4InitColor = D3DXVECTOR4(0.4,0.4,0.4, 0.6f);
	m_pSmokeParticle->m_bLoop = true;
	m_pSmokeParticle->m_bColorOverTime = true;
	m_pSmokeParticle->m_vec4ColorLifeTime0 = D3DXVECTOR4(0.8f, 0.4f, 0.4f, 1.0f);
	m_pSmokeParticle->m_vec4ColorLifeTime1 = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);

	m_pSmokeParticle->m_funcPositionNoise = [](float t) ->D3DXVECTOR3 {
		return D3DXVECTOR3(rand() % 1000 * 0.001 - 0.5,
			rand() % 1000 * 0.001 - 0.5,
			rand() % 1000 * 0.001 - 0.5);
	};
	m_pSmokeParticle->m_pTexture = g_pTextureManager->GetTexture("smoke.png");
	m_pSmokeParticle->Start();
}
