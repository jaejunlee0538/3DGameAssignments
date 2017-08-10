#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCrtCtrl.h"
#include "cGroup.h"
#include "cObjLoader.h"
#include "cLerp.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cMapLoader.h"
#include "cButton.h"
#include "cPlayer.h"

namespace {
	RECT createRECT(int left, int top, int width, int height) {
		return { left, top, left + width, top + height };
	}
}

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	,m_pCamera(NULL)
	, m_TenUnitStage(0)
	, m_OneUnitStage(1)
	,m_pPlayer(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	for (auto pFrame : m_gameMap) {
		if (pFrame) {
			pFrame->Destroy();
		}
	}
	for (auto pFrame : m_floorCollision) {
		if (pFrame) {
			pFrame->Destroy();
		}
	}
	
	//전역 객체 삭제
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	{
		D3DXMATRIX matR, matS;
		D3DXMatrixScaling(&matS, 0.01, 0.01, 0.01);
		//D3DXMatrixScaling(&matS, 1,1,1);

		D3DXMatrixRotationX(&matR, D3DXToRadian(-90));
		{
			cObjLoader objLoader;
			objLoader.Load("obj/map_surface.obj", m_floorCollision);

			for (auto pFrame : m_floorCollision) {
				if (pFrame) {
					pFrame->TransformVertices(matS*matR);
				}
			}
		}
		{
			cObjLoader objLoader;
			objLoader.Load("obj/Map.obj", m_gameMap);
			for (auto pFrame : m_gameMap) {
				if (pFrame) {
					pFrame->TransformVertices(matS*matR);
				}
			}
		}
	}

	m_pPlayer = new cPlayer();
	m_pPlayer->Init();
	m_pPlayer->SetPosition({ -5,0,0 });
	m_pPlayer->ResolveCollisionByLiftingUp(m_floorCollision);
	//////////////////////그리드 셋업////////////////////////////
	m_pGrid = new cGrid();
	m_pGrid->Setup();
	m_pGrid->SetOrigin(0,-0.2,0);

	//////////////////////카메라 생성////////////////////////////
	m_pCamera = new cCamera;
	m_pCamera->Setup(m_pPlayer->GetPosition());

	///////////////////////////////////////////////////////////
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	UpdateLight();
}

void cMainGame::Update()
{
	static DWORD time = 0;
	time += 13;
	g_pTimeManager->Update();

	//DWORD time = GetTickCount();

	m_pPlayer->Update(time);
	m_pPlayer->ResolveCollision(m_floorCollision);
	///////////////////////////////////////////////////////////
	if (m_pCamera) m_pCamera->Update(m_pPlayer->GetPosition());
	if (m_pGrid) m_pGrid->Update();
	UpdateLight();
	///////////////////////////////////////////////////////////
	g_pAutoReleasePool->Drain();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	if (g_pKeyManager->isToggleKey(VK_TAB)) {
		for (auto pFrame : m_gameMap) {
			pFrame->Render();
		}
	}
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	for (auto pFrame : m_floorCollision) {
		pFrame->Render();
	}
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pPlayer->Render();
	///////////////////////////////////////////////////////////
	//m_pGrid->Render();
	//////////////////////////////////////////////////////////

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


bool cMainGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera && m_pCamera->MsgProc(hWnd, message, wParam, lParam)){
		return true;
	}
	return false;
}

void cMainGame::UpdateLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Direction = m_pCamera->GetLookDir();
	stLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.25f;
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	g_pD3DDevice->SetLight(0, &stLight);
	
	g_pD3DDevice->LightEnable(0, true);
}
