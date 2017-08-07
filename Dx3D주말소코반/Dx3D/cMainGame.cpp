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
	//��ư ����
	for each(auto p in m_vecButtons) {
		SAFE_DELETE(p);
	}
	m_vecButtons.clear();
	//���� ��ü ����
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	////////////////���ڹ� ���� ��ü �ʱ�ȭ////////////////////
	m_sokoban.Init();

	///////////////��ư UI ���� �� �ʱ�ȭ//////////////////////
	RECT clntRect;
	GetClientRect(g_hWnd, &clntRect);
	//����ŸƮ ��ư
	cButton* btnRestart = new cButton();
	btnRestart->Init(createRECT(clntRect.left + 10, clntRect.bottom - 50, 200, 50), "Restart", std::bind(&cMainGame::RestartGame, this));
	//���� �������� ��ư
	cButton * btnPrevStage = new cButton();
	btnPrevStage->Init(createRECT(clntRect.left + 220, clntRect.bottom - 50, 150, 50),
		"Prev", std::bind(&cMainGame::StartPrevGame, this));
	//���� �������� ��ư
	cButton * btnNextStage = new cButton();
	btnNextStage->Init(createRECT(clntRect.left + 380, clntRect.bottom - 50, 150, 50),
		"Next", std::bind(&cMainGame::StartNextGame, this));
	//Undo ��ư
	cButton * btnUndo = new cButton();
	btnUndo->Init(createRECT(clntRect.left + 10, clntRect.top + 50, 150, 50),
		"Undo", std::bind(&SokobanGame::Undo, &m_sokoban));
	//Redo ��ư
	cButton * btnRedo = new cButton();
	btnRedo->Init(createRECT(clntRect.left + 170, clntRect.top + 50, 150, 50),
		"Redo", std::bind(&SokobanGame::Redo, &m_sokoban));
	//Moveī��Ʈ
	cButton* btnMoveCount = new cButton();
	btnMoveCount->Init(createRECT(clntRect.right - 300, clntRect.bottom - 50, 250, 50), "�̵� : 0", NULL);
	m_sokoban.SetMovecountUpdateCallback([btnMoveCount](int count) {
		std::string str = "�̵� : " + std::to_string(count);
		btnMoveCount->SetText(str);
	});
	//����������ȣ
	cButton* btnStage = new cButton();
	btnStage->Init(createRECT(clntRect.right - 300, clntRect.top + 50, 250, 50), "�������� : 0", NULL);
	m_sokoban.SetStageLoadCallback([btnStage](std::string stageName) {
		btnStage->SetText(stageName);
	});
	//��ư�� �ݹ� ���

	m_sokoban.SetGameClearCallback(std::bind(&cMainGame::GameCleared, this, std::placeholders::_1));

	m_vecButtons.push_back(btnRestart);
	m_vecButtons.push_back(btnMoveCount);
	m_vecButtons.push_back(btnPrevStage);
	m_vecButtons.push_back(btnNextStage);
	m_vecButtons.push_back(btnStage);
	m_vecButtons.push_back(btnUndo);
	m_vecButtons.push_back(btnRedo);
	/////////////////////Scene �ð� ����/////////////////////////
	m_Firstframe = m_sokoban.m_Firstframe;
	m_Lastframe = m_sokoban.m_Lastframe;
	m_FrameSpeed = m_sokoban.m_FrameSpeed;

	//////////////////////�׸��� �¾�////////////////////////////
	m_pGrid = new cGrid();
	m_pGrid->Setup();
	m_pGrid->SetOrigin(0,-0.2,0);

	///////////////////////ù��° ���� �ε�///////////////////////
	char szbuf[1024] = { '\0', };
	sprintf(szbuf, "./Resource/stage%d%d.txt", m_TenUnitStage, m_OneUnitStage);
	m_sokoban.LoadGame(szbuf);

	//////////////////////ī�޶� ����////////////////////////////
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

	///////////////////���ڹ� ���� ������Ʈ////////////////////////////////////////
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

	////////////////////��ư ������///////////////////////////////////////
	for each(auto p in m_vecButtons) {
		p->Render();
	}
	////////////////////���ڹ� ���� ������///////////////////////////////////////
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
		std::string msg = "�������� ���� �����߽��ϴ�. �������� 1�� ���ư��ϴ�.";
		::MessageBoxA(g_hWnd, msg.c_str(), "����ؿ�!", MB_OK);
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
	std::string msg = "���� ���������� ���� �Ͻðڽ��ϱ�?";
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
