#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCrtCtrl.h"
#include "cAseLoader.h"
#include "cAllocateHierarchy.h"
#include "cSkinnedMesh.h"
#include "cSkinnedMeshManager.h"

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	, m_pCrtCtrl(NULL)
	, m_pFont(NULL)
	, m_pMap(NULL)
	,m_fpsCalculator(1.0f)
{

}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCrtCtrl);
	SAFE_DELETE(m_pMap);
	
	SAFE_RELEASE(m_pFont);
	for each(auto p in m_vecSkinnedMesh)
	{
		SAFE_DELETE(p);
	}
	DeleteCriticalSection(&m_csLoading);
	
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pDeviceManager->Destroy();


}

void cMainGame::Setup()
{
	InitializeCriticalSection(&m_csLoading);
	m_nZealotLoaded = 0;

	DWORD idThread;
	HANDLE hThread = CreateThread(0, 0, &cMainGame::ZealotLoadingThread, this, CREATE_SUSPENDED, &idThread);
	ResumeThread(hThread);
	CloseHandle(hThread);

	D3DXMATRIX matWorld, matS, matR;
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	matWorld = matS * matR;

	D3DXFONT_DESC fd;
	ZeroMemory(&fd,sizeof(D3DXFONT_DESC));
	fd.Height			= 25;
	fd.Width			= 12;
	fd.Weight			= FW_NORMAL;
	fd.Italic			= false;
	fd.CharSet			= DEFAULT_CHARSET;
	fd.OutputPrecision  = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily   = FF_DONTCARE;
	//AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "궁서체");	//글꼴 스타일
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);

	//D3DXCreateSphere(g_pD3DDevice, 3, 50, 50, &m_pMesh, NULL);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCrtCtrl = new cCrtCtrl;
	
	m_pCamera = new cCamera;
	m_pCamera->Setup(m_pCrtCtrl->GetPosition());

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	SetLight();
	m_tickCounter.Start();
	m_fpsCalculator.reset();
}

void cMainGame::Update()
{
	m_fpsCalculator.update(m_tickCounter.GetElapsedTime()*0.001);
	g_pTimeManager->Update();

	if (m_pCrtCtrl) m_pCrtCtrl->Update(m_pMap);
	if (m_pCamera) m_pCamera->Update();

	g_pAutoReleasePool->Drain();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,0),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	// 그림을 그린다.
	if(m_pGrid) m_pGrid->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	
	int numInsideFrustum = 0;
	EnterCriticalSection(&m_csLoading);
	size_t numLoaded = m_nZealotLoaded;
	LeaveCriticalSection(&m_csLoading);

	for(size_t i=0; i < numLoaded; ++i)
	{
		if (m_pCamera->CheckFrustumCulling(m_vecSkinnedMesh[i]->GetPosition(), 0.1f)) {
			m_vecSkinnedMesh[i]->UpdateAndRender();
			numInsideFrustum++;
		}
	}

	RECT rect;
	rect.left = 10;
	rect.right = 10;
	rect.top = 10;
	rect.bottom = 10;

	char buffer[100];
	sprintf_s(buffer, 100, "FPS : %.1f FPS\n총 질럿 : %d\nInFrustum : %d 마리", m_fpsCalculator.getFPS(), m_vecSkinnedMesh.size(), numInsideFrustum);
	m_pFont->DrawTextA(NULL, buffer, strlen(buffer), &rect, DT_NOCLIP , D3DCOLOR_XRGB(255,255,255));
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

DWORD cMainGame::ZealotLoadingThread(LPVOID userData)
{
	cMainGame* pGame = (cMainGame*)userData;
	int nX = 60;
	int nY = 60;
	pGame->m_vecSkinnedMesh.resize(nX*nY, nullptr);
	int count = 0;
	for (int x = -30; x < 30; ++x)
	{
		for (int z = -30; z < 30; ++z)
		{
			cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh("Zealot/", "Zealot.X");
			pSkinnedMesh->SetPosition(D3DXVECTOR3(x, 0, z));
			pSkinnedMesh->SetRandomTrackPosition();
			EnterCriticalSection(&pGame->m_csLoading);
			pGame->m_vecSkinnedMesh[count++] = pSkinnedMesh;
			pGame->m_nZealotLoaded = count;
			LeaveCriticalSection(&pGame->m_csLoading);
		}
	}
	return 0;
}

