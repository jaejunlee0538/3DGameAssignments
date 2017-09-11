#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cCrtCtrl.h"
#include "cAseLoader.h"
#include "cAllocateHierarchy.h"
#include "cSkinnedMesh.h"
#include "HeighMap.h"
#include "ZealotPlayer.h"
#include "Grid.h"

cMainGame::cMainGame(void)
	:m_pCamera(NULL)
	,m_pZealot(NULL),
	m_pHeightMap(NULL),
	m_pGrid(NULL)
{

}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pZealot);
	SAFE_DELETE(m_pHeightMap);
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	D3DXMATRIX matWorld, matS, matR;
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	D3DXMatrixScaling(&matS, 0.01f, 0.01f, 0.01f);
	matWorld = matS * matR;

	cSkinnedMesh* zealotMesh = new cSkinnedMesh("","JinF.X");
	//cSkinnedMesh* zealotMesh = new cSkinnedMesh("Unit/Ghost/", "Ghost.X");

	m_pZealot = new ZealotPlayer();
	m_pZealot->setup(zealotMesh);
	m_pZealot->setPosition(D3DXVECTOR3(100, 0, 100));
	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pGrid = new Grid();
	m_pGrid->init(20, 20, 1.0f, 1.0f);
	//Height ¸Ê ·Îµå
	m_pHeightMap = new HeightMap();
	m_pHeightMap->Load("HeightMapData/HeightMap.raw", 0.1f, 1.0f, 257, 257, "HeightMapData/terrain.jpg");

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	SetLight();                 
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	m_pZealot->update(false);
	D3DXVECTOR3 zealotPos = m_pZealot->getWorldPosition();
	float height;
	m_pHeightMap->GetHeight(height, zealotPos.x, zealotPos.z);

	m_pZealot->setPositionY(height);
	m_pZealot->reupdateTransform();

	if (m_pCamera) m_pCamera->Update(m_pZealot->getWorldPosition());
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
	m_pZealot->render();
	//m_pGrid->render();
	m_pHeightMap->Render();

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

