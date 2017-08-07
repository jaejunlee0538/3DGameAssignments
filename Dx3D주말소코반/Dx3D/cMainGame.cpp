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
{
}

cMainGame::~cMainGame(void)
{
	m_sokoban.Reset();
	m_sokoban.Uninitalize();
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	//버튼 삭제
	for each(auto p in m_vecButtons) {
		SAFE_DELETE(p);
	}
	m_vecButtons.clear();
	//전역 객체 삭제
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	////////////////소코반 게임 객체 초기화////////////////////
	m_sokoban.Init();

	///////////////버튼 UI 생성 및 초기화//////////////////////
	RECT clntRect;
	GetClientRect(g_hWnd, &clntRect);
	//리스타트 버튼
	cButton* btnRestart = new cButton();
	btnRestart->Init(createRECT(clntRect.left + 10, clntRect.bottom - 50, 200, 50), "Restart", std::bind(&cMainGame::RestartGame, this));
	//이전 스테이지 버튼
	cButton * btnPrevStage = new cButton();
	btnPrevStage->Init(createRECT(clntRect.left + 220, clntRect.bottom - 50, 150, 50),
		"Prev", std::bind(&cMainGame::StartPrevGame, this));
	//다음 스테이지 버튼
	cButton * btnNextStage = new cButton();
	btnNextStage->Init(createRECT(clntRect.left + 380, clntRect.bottom - 50, 150, 50),
		"Next", std::bind(&cMainGame::StartNextGame, this));
	//Undo 버튼
	cButton * btnUndo = new cButton();
	btnUndo->Init(createRECT(clntRect.left + 10, clntRect.top + 50, 150, 50),
		"Undo", std::bind(&SokobanGame::Undo, &m_sokoban));
	//Redo 버튼
	cButton * btnRedo = new cButton();
	btnRedo->Init(createRECT(clntRect.left + 170, clntRect.top + 50, 150, 50),
		"Redo", std::bind(&SokobanGame::Redo, &m_sokoban));
	//Move카운트
	cButton* btnMoveCount = new cButton();
	btnMoveCount->Init(createRECT(clntRect.right - 300, clntRect.bottom - 50, 250, 50), "이동 : 0", NULL);
	m_sokoban.SetMovecountUpdateCallback([btnMoveCount](int count) {
		std::string str = "이동 : " + std::to_string(count);
		btnMoveCount->SetText(str);
	});
	//스테이지번호
	cButton* btnStage = new cButton();
	btnStage->Init(createRECT(clntRect.right - 300, clntRect.top + 50, 250, 50), "스테이지 : 0", NULL);
	m_sokoban.SetStageLoadCallback([btnStage](std::string stageName) {
		btnStage->SetText(stageName);
	});
	//버튼에 콜백 등록

	m_sokoban.SetGameClearCallback(std::bind(&cMainGame::GameCleared, this, std::placeholders::_1));

	m_vecButtons.push_back(btnRestart);
	m_vecButtons.push_back(btnMoveCount);
	m_vecButtons.push_back(btnPrevStage);
	m_vecButtons.push_back(btnNextStage);
	m_vecButtons.push_back(btnStage);
	m_vecButtons.push_back(btnUndo);
	m_vecButtons.push_back(btnRedo);
	/////////////////////Scene 시간 설정/////////////////////////
	m_Firstframe = m_sokoban.m_Firstframe;
	m_Lastframe = m_sokoban.m_Lastframe;
	m_FrameSpeed = m_sokoban.m_FrameSpeed;

	//////////////////////그리드 셋업////////////////////////////
	m_pGrid = new cGrid();
	m_pGrid->Setup();
	m_pGrid->SetOrigin(0,-0.2,0);

	///////////////////////첫번째 게임 로드///////////////////////
	char szbuf[1024] = { '\0', };
	sprintf(szbuf, "./Resource/stage%d%d.txt", m_TenUnitStage, m_OneUnitStage);
	m_sokoban.LoadGame(szbuf);

	//////////////////////카메라 생성////////////////////////////
	m_pCamera = new cCamera;
	m_pCamera->Setup(m_sokoban.GetPlayerPosition());

	///////////////////////////////////////////////////////////
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	UpdateLight();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	DWORD animationTime = ((DWORD)(GetTickCount()*m_FrameSpeed) % (m_Lastframe - m_Firstframe) + m_Firstframe);

	///////////////////소코반 게임 업데이트////////////////////////////////////////
	m_sokoban.Update(animationTime);

	///////////////////////////////////////////////////////////
	if (m_pCamera) m_pCamera->Update(m_sokoban.GetPlayerPosition(), m_sokoban.GetPlayerLookAngle());
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

	////////////////////버튼 렌더링///////////////////////////////////////
	for each(auto p in m_vecButtons) {
		p->Render();
	}
	////////////////////소코반 게임 렌더링///////////////////////////////////////
	m_sokoban.Render();
	///////////////////////////////////////////////////////////
	m_pGrid->Render();
	//////////////////////////////////////////////////////////

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


bool cMainGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	for each(auto p in m_vecButtons) {
		if (p->MsgProc(hWnd, message, wParam, lParam))
			return true;
	}
	if (m_sokoban.MsgProc(hWnd, message, wParam, lParam)) {
		return true;
	}
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
	stLight.Ambient = D3DXCOLOR(0.4f,0.4f,0.4f, 1.0f) * 0.15f;
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	g_pD3DDevice->SetLight(0, &stLight);
	
	g_pD3DDevice->LightEnable(0, true);
}

void cMainGame::StartNextGame()
{
	m_OneUnitStage++;
	if (m_OneUnitStage > 10) {
		std::string msg = "스테이지 끝에 도달했습니다. 스테이지 1로 돌아갑니다.";
		::MessageBoxA(g_hWnd, msg.c_str(), "대단해요!", MB_OK);
		m_OneUnitStage = 1;
	}
	char szbuf[1024] = { '\0', };
	sprintf(szbuf, "./Resource/stage%02d.txt", m_OneUnitStage);
	m_sokoban.LoadGame(szbuf);
}

void cMainGame::StartPrevGame()
{
	m_OneUnitStage--;
	if (m_OneUnitStage <= 0) {
		m_OneUnitStage = 1;
		return;
	}

	char szbuf[1024] = { '\0', };
	sprintf(szbuf, "./Resource/stage%02d.txt", m_OneUnitStage);
	m_sokoban.LoadGame(szbuf);

}

void cMainGame::RestartGame()
{
	char szbuf[1024] = { '\0', };
	sprintf(szbuf, "./Resource/stage%d%d.txt", m_TenUnitStage, m_OneUnitStage);
	m_sokoban.LoadGame(szbuf);
}

void cMainGame::GameCleared(int totalMovements)
{
	std::string msg = "다음 스테이지를 진행 하시겠습니까?";
	if (::MessageBoxA(g_hWnd, msg.c_str(), "Game Clear!", MB_YESNO) == IDYES) {
		StartNextGame();
	}
	else {
		RestartGame();
	}
}

void cMainGame::GoToFirstStage()
{
}
