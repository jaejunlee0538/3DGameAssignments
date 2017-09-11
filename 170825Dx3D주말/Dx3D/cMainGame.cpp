#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cCrtCtrl.h"
#include "cAseLoader.h"
#include "cAllocateHierarchy.h"
#include "cSkinnedMesh.h"
#include "cSkinnedMeshManager.h"
#include "cFrustum.h"
#include "cObjMap.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cUIButton.h"
#include "cHeightMap.h"
#include "cAstar.h"
#include "cMapLoader.h"
#include "cActionMove.h"
#include "cActionSeq.h"
#include "CountDown.h"
#include "HPBar.h"


enum
{
	E_CONFIRM_BUTTON = 213,
	E_CANCEL_BUTTON = 214,
	E_TEXT_VIEW,
};

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL)
	, m_pCrtCtrl(NULL)
	, m_pFont(NULL)
	, m_pMap(NULL)
	, m_pFrustum(NULL)
	, m_pSprite(NULL)
	, m_pTexture(NULL)
	, m_pAstar(NULL)
	, m_pNodeMap(NULL)
	, m_RootObject(NULL)
	, m_EnemyObject(NULL)
	, m_pAstarMarin(NULL)
	, m_bNearCrt(false)
	, m_pCount(NULL)
	, m_bRender(false)
	, Zealothp(NULL)
	, Marinhp(NULL)
	, totalZealotHp(150)
	, Zealotdamage(10)
	, totalMarinHp(100)
	, Marindamage(10)
	, currentZealotHp(150)
	, currentMarinHp(100)
	, attackting(false)
	, m_bStageClear(false)
	, m_bStageOver(false)
	, m_ClearNum(0)
{
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 50;
	fd.Weight = 25;
	fd.Weight = FW_NORMAL | FW_BOLD;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;

	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
}

cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pCrtCtrl);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pFrustum);
	SAFE_DELETE(m_pAstar);
	SAFE_DELETE(m_pNodeMap);
	SAFE_RELEASE(m_RootObject);
	SAFE_RELEASE(m_EnemyObject);
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pAstarMarin);
	SAFE_DELETE(m_pCount);
	SAFE_DELETE(Zealothp);
	SAFE_DELETE(Marinhp);

	for each(auto p in m_vecSkinnedMesh)
	{
		SAFE_DELETE(p);
	}

	for each(auto p in m_vecSkinnedMeshMarin)
	{
		SAFE_DELETE(p);
	}

	SAFE_RELEASE(m_pSprite);

	g_pFontManager->Destroy();
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pFrustum = new cFrustum;

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCrtCtrl = new cCrtCtrl;

	m_pAstar = new cAstar;
	m_pAstar->Setup();
	m_pAstar->MapLoad("map.txt");

	m_pAstarMarin = new cAstar;
	m_pAstarMarin->Setup();
	m_pAstarMarin->MapLoad("map.txt");

	m_pNodeMap = new cMapLoader;
	m_pNodeMap->Setup();
	m_pNodeMap->MapLoad("map.txt");

	m_RootObject = new cGameObject;
	m_RootObject->SetPosition(D3DXVECTOR3(5, 0, 5));
	cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh("Unit/Zealot/", "Zealot.X");
	pSkinnedMesh->m_matRoot = m_RootObject->GetWorldTM();
	m_vecSkinnedMesh.push_back(pSkinnedMesh);

	m_EnemyObject = new cGameObject;
	cSkinnedMesh* pSkinnedMeshEnemy = new cSkinnedMesh("Unit/Marine/", "Marine.X");
	m_EnemyObject->SetPosition(D3DXVECTOR3(30.0f, 0.0f, 30.0f));
	pSkinnedMeshEnemy->m_matRoot = m_EnemyObject->GetWorldTM();
	m_vecSkinnedMeshMarin.push_back(pSkinnedMeshEnemy);

	m_pCamera = new cCamera;
	D3DXVECTOR3* crtPos = m_RootObject->getPointpos();
	m_pCamera->Setup(crtPos);

	D3DCOLOR c = D3DCOLOR_XRGB(255, 255, 255);
	m_vecSquareVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-1.0f, 0.7f, -1.0f), c));
	m_vecSquareVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-1.0f, 0.7f, 31.0f), c));
	m_vecSquareVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(31.0f, 0.7f, -1.0f), c));
	m_vecSquareVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(31.0f, 0.7f, -1.0f), c));
	m_vecSquareVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(-1.0f, 0.7f, 31.0f), c));
	m_vecSquareVertex.push_back(ST_PC_VERTEX(D3DXVECTOR3(31.0f, 0.7f, 31.0f), c));

	m_pCamera->setSquarevertex(m_vecSquareVertex);

	m_pCount = new CountDown;
	m_pCount->Init(3, 700, 200, std::bind(&cMainGame::setRender, this));
	m_pCount->Restart();

	Zealothp = new HPBar;
	Zealothp->Init(3.0f, 0.3f, totalZealotHp, currentZealotHp);

	Marinhp = new HPBar;
	Marinhp->Init(3.0f, 0.3f, totalMarinHp, currentMarinHp);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	SetLight();
}

void cMainGame::Update()
{
	static float hpRecover = 0.0f;
	g_pTimeManager->Update();
	if (m_pCount)m_pCount->Update();
	if (m_pCamera) m_pCamera->Update();
	Zealothp->UpdateCameraPosition(m_pCamera->getEyePos());

	if (m_RootObject)m_RootObject->Update();
	if (Zealothp)Zealothp->UpdatePosition(m_RootObject->GetPosition());
	Zealothp->UpdateCurrentHP(currentZealotHp);
	hpRecover += 0.01f;
	if (hpRecover > 1.0f) {
		currentZealotHp += 1;
		if (currentZealotHp > 150) {
			currentZealotHp = 150;
		}
		hpRecover = 0.0f;
	}
	for each (auto p in m_vecSkinnedMesh)
	{
		p->m_matRoot = m_RootObject->GetWorldTM();
	}

	if (m_RootObject->getMoving())
	{
		for each (auto p in m_vecSkinnedMesh)
		{
			p->SetAnimationIndex(3);
		}
	}else {
		for each (auto p in m_vecSkinnedMesh)
		{
			p->SetAnimationIndex(4);
		}
	}

	if (g_pKeyManager->isOnceKeyDown(VK_SPACE))
	{
		for each (auto p in m_vecSkinnedMesh)
		{
			p->SetAnimationIndex(2);
			if (m_bNearCrt) {
				currentMarinHp -= Zealotdamage;
			}
		}
	}

	//m_RootObject->setDirection(m_EnemyObject->GetPosition());
	//m_EnemyObject->setDirection(m_RootObject->GetPosition());


	int crtCol, crtRow;
	int DestCol, DestRow;

	//캐릭터 위치인덱스
	crtRow = m_RootObject->GetWorldTM()._41 / 2;
	crtCol = m_RootObject->GetWorldTM()._43 / 2;

	//픽킹한거위치인덱스
	D3DXVECTOR3 picking;
	vector<D3DXVECTOR3> AstarMove;
	if (m_bRender) {
		if (g_pKeyManager->isOnceKeyDown(VK_RBUTTON)) {

			if (m_pCamera->getPicking(picking)) {

				DestRow = ((picking.x + 1.0f) / 2);
				DestCol = ((picking.z + 1.0f) / 2);

				if (!m_pAstar->ClickBlock(DestRow, DestCol)) {
					m_pAstar->NavigateAstar(crtRow, crtCol, DestRow, DestCol);
					lstNode = m_pAstar->m_lstRoute;
					while (lstNode.empty() == false) {
						int x = lstNode.front()->nRow;
						int z = lstNode.front()->nCol;
						AstarMove.push_back(D3DXVECTOR3(x * 2, 0, z * 2));
						lstNode.pop_front();

					}
					{
						cActionSeq* pActionMoveSeq = new cActionSeq;
						pActionMoveSeq->AddAction(CreateActionMove(m_RootObject->GetPosition(), AstarMove[1], m_RootObject));
						for (size_t i = 2; i < AstarMove.size(); ++i) {
							pActionMoveSeq->AddAction(CreateActionMove(AstarMove[i - 1], AstarMove[i], m_RootObject));
						}

						pActionMoveSeq->SetTarget(m_RootObject);
						pActionMoveSeq->Start();
						m_RootObject->SetAction(pActionMoveSeq);
						m_RootObject->SetIsDirection(true);
						pActionMoveSeq->AutoRelease();
						AstarMove.clear();
					}
				}
			}
		}
	}
	//적 AI

	if (m_EnemyObject)m_EnemyObject->Update();
	if (Marinhp)Marinhp->UpdatePosition(m_EnemyObject->GetPosition());
	Marinhp->UpdateCameraPosition(m_pCamera->getEyePos());
	Marinhp->UpdateCurrentHP(currentMarinHp);
	for each (auto p in m_vecSkinnedMeshMarin)
	{
		p->m_matRoot = m_EnemyObject->GetWorldTM();
	}

	if (m_EnemyObject->getMoving())
	{
		for each (auto p in m_vecSkinnedMeshMarin)
		{
			p->SetAnimationIndex(1);
		}
	}
	else if (!m_bNearCrt)
	{
		for each (auto p in m_vecSkinnedMeshMarin)
		{
			p->SetAnimationIndex(0);
		}
	}
	int MarinCol, MarinRow;
	int CrtDestCol, CrtDestRow;

	vector<D3DXVECTOR3> AstarMoveMarin;

	//적현재 위치인덱스
	static int count = 0;
	count++;
	if (count % 90 == 0) {
		CrtDestRow = m_RootObject->GetWorldTM()._41 / 2;
		CrtDestCol = m_RootObject->GetWorldTM()._43 / 2;
		MarinRow = round(m_EnemyObject->GetWorldTM()._41 / 2);
		MarinCol = round(m_EnemyObject->GetWorldTM()._43 / 2);

		auto curPos = D3DXVECTOR3(m_EnemyObject->GetWorldTM()._41, 0, m_EnemyObject->GetWorldTM()._43);
		if (D3DXVec3Length(&(curPos - D3DXVECTOR3(int(curPos.x), 0, int(curPos.z)))) > 0.05) {
			AstarMoveMarin.push_back(curPos);
		}

		count = 0;
		m_pAstarMarin->NavigateAstar(MarinRow, MarinCol, CrtDestRow, CrtDestCol);
		lstNodeMarin = m_pAstarMarin->m_lstRoute;
		while (lstNodeMarin.empty() == false) {
			int x = lstNodeMarin.front()->nRow;
			int z = lstNodeMarin.front()->nCol;
			AstarMoveMarin.push_back(D3DXVECTOR3(x * 2, 0, z * 2));
			lstNodeMarin.pop_front();
		}
		
		if (abs(MarinRow - CrtDestRow) > 1 || abs(MarinCol - CrtDestCol) > 1)
		{
			m_bNearCrt = false;
		}
		else
		{
			m_bNearCrt = true;
		}
		if (!m_EnemyObject->getMoving() && m_bNearCrt) {
			for each (auto p in m_vecSkinnedMeshMarin)
			{
				p->SetAnimationIndex(3);
				currentZealotHp -= Marindamage;
			}
		}
	}

	if (!AstarMoveMarin.empty()) {
		cActionSeq* pActionMoveSeqMarin = new cActionSeq;
		for (size_t i = 1; i < AstarMoveMarin.size() - 1; ++i) {
			pActionMoveSeqMarin->AddAction(CreateActionMove(AstarMoveMarin[i - 1], AstarMoveMarin[i], m_EnemyObject));
		}

		pActionMoveSeqMarin->SetTarget(m_EnemyObject);
		pActionMoveSeqMarin->Start();
		m_EnemyObject->SetAction(pActionMoveSeqMarin);
		m_EnemyObject->SetIsDirection(true);
		AstarMoveMarin.clear();
		pActionMoveSeqMarin->AutoRelease();
	}


	//스테이지 클리어시
	if (currentMarinHp <= 0) {
		m_bStageClear = true;
		m_bRender = false;
		m_pCount->Restart();
		m_ClearNum++;
		currentZealotHp = 150;
		currentMarinHp = 100;

		Marindamage += 5;

		m_RootObject->SetPosition(D3DXVECTOR3(5.0f, 0.0f, 5.0f));
		m_EnemyObject->SetPosition(D3DXVECTOR3(30.0f, 0.0f, 30.0f));
	}

	if (currentZealotHp <= 0)
	{
		m_bStageOver = true;
	}

	g_pAutoReleasePool->Drain();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		//D3DCOLOR_XRGB(0, 0, 255),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	RECT rc;
	rc.left = 700;
	rc.right = 700 + 1;
	rc.top = 100;
	rc.bottom = 100 + 1;
	if (m_bStageOver) {
		string gameover = "Game Over";
		m_pFont->DrawText(NULL,
			gameover.c_str(),
			gameover.length(),
			&rc,
			DT_CENTER | DT_NOCLIP,
			D3DCOLOR_XRGB(255, 255, 255));
	}

	if (m_bStageClear) {
		string clear = "기다려 주세요..";
		m_pFont->DrawText(NULL,
			clear.c_str(),
			clear.length(),
			&rc,
			DT_CENTER | DT_NOCLIP,
			D3DCOLOR_XRGB(255, 255, 255));
	}

	if (m_pNodeMap)m_pNodeMap->Render();
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	// 
	D3DXMATRIX matWorld, matT;
	D3DXMatrixIdentity(&matWorld);

	if (!m_bStageOver) {
		if (m_pCount)m_pCount->Render();
		if (m_bRender) {
			Zealothp->Render();
			Marinhp->Render();
			D3DXMatrixIdentity(&matWorld);
			for each (auto p in m_vecSkinnedMeshMarin)
			{
				ST_SPHERE stSphere = p->GetBoundingSphere();
				stSphere.p += p->GetPosition();
				if (m_pFrustum->IsIn(&stSphere))
				{
					p->UpdateAndRender();
				}
			}

			for each (auto p in m_vecSkinnedMesh)
			{
				ST_SPHERE stSphere = p->GetBoundingSphere();
				stSphere.p += p->GetPosition();
				if (m_pFrustum->IsIn(&stSphere))
				{
					p->UpdateAndRender();
				}
			}
		}
	}
	if (m_pMap) m_pMap->Render();

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &(m_pCrtCtrl->GetWorldTM()));

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


void cMainGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		switch (wParam) {
		case VK_SPACE:
		{

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
	D3DXVECTOR3 vDir(1, -1, 1);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);
}

void cMainGame::OnClick(cUIButton* pSender)
{

}

cActionMove * cMainGame::CreateActionMove(D3DXVECTOR3 from, D3DXVECTOR3 to, cGameObject* target)
{
	cActionMove* actionMove = new cActionMove(from, to);
	float distance = D3DXVec3Length(&(from - to));
	actionMove->SetActionTime(distance / 5.0f);
	actionMove->SetTarget(target);
	//오토 릴리즈
	actionMove->AutoRelease();
	return actionMove;
}

void cMainGame::setRender()
{
	m_bRender = true;
	m_bStageClear = false;
}


