#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCrtCtrl.h"
#include "cObjLoader.h"
#include "cLerp.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cMapLoader.h"
#include "cButton.h"
#include "cPlayer.h"
#include "TickCounter.h"
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
	, m_pFont(NULL)
{

}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_RELEASE(m_pFont);
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
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 30;
	fd.Width = 15;
	fd.Weight = FW_NORMAL;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	//AddFontResource("umberto.ttf");
	strcpy_s(fd.FaceName, "고딕");	//글꼴 스타일
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);


	{
		D3DXMATRIX matR, matS;
		//D3DXMatrixScaling(&matS, 0.01, 0.01, 0.01);
		D3DXMatrixScaling(&matS, 1,1,1);

		D3DXMatrixRotationX(&matR, D3DXToRadian(-90));
		{
			cObjLoader objLoader;
			objLoader.Load("obj/map_surface.obj", m_floorCollision);

			for (auto pFrame : m_floorCollision) {
				if (pFrame) {
					pFrame->TransformVertices(matS*matR);
				}
				pFrame->UpdateVertexBuffer();
			}
		}
		{
			cObjLoader objLoader;
			objLoader.Load("obj/Map.obj", m_gameMap);
			for (auto pFrame : m_gameMap) {
				if (pFrame) {
					pFrame->TransformVertices(matS*matR);
				}
				pFrame->UpdateVertexBuffer();
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
	char strBuffer[256];
	const size_t NDraws = 1;

	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f, 0);

	g_pD3DDevice->BeginScene();
	{
		RECT rect1{ 0,0,300,50 };
		TickCounter stopWatch;
		stopWatch.Start();
		for (size_t i = 0; i < NDraws; ++i) {
			for (auto pFrame : m_gameMap) {
				pFrame->RenderUP();
			}
			m_pPlayer->RenderUP();
			m_pGrid->RenderUP();
		}
		DWORD elapsedTimeUP = stopWatch.GetElapsedTime();
		sprintf_s(strBuffer, 256, "DrawUP:%u 밀리초", elapsedTimeUP);
		m_pFont->DrawTextA(NULL, strBuffer, strlen(strBuffer), &rect1, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 255, 255));
	}
	{
		TickCounter stopWatch;
		RECT rect2{ 0, 50,300,100 };
		stopWatch.Start();
		for (size_t i = 0; i < NDraws; ++i) {
			for (auto pFrame : m_gameMap) {
				pFrame->Render();
			}
			m_pPlayer->Render();
			m_pGrid->Render();
		}
		DWORD elapsedTimeVB = stopWatch.GetElapsedTime();
		sprintf_s(strBuffer, 256, "DrawVB:%u 밀리초", elapsedTimeVB);
		m_pFont->DrawTextA(NULL, strBuffer, strlen(strBuffer), &rect2, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 255, 255));
	}
	{
		RECT rect3{ 0,100,300,150 };
		sprintf_s(strBuffer, 256, "Draw횟수:%u", NDraws);
		m_pFont->DrawTextA(NULL, strBuffer, strlen(strBuffer), &rect3, DT_LEFT | DT_TOP, D3DCOLOR_XRGB(255, 255, 255));
	}

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
