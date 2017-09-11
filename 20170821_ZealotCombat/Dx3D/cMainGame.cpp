#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cFrameHierarchyAllocator.h"
#include "cSkinnedModel.h"

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL),
	m_pFont(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	for (size_t i = 0; i < 2; ++i) {
		SAFE_DELETE(m_pZealots[i]);
	}
	//���� ��ü ����
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	InitFont();
	InitCamera();
	InitLight();
	InitGrid();
	InitZealot();
	///////////////////////////////////////////////////////////
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	g_pD3DDevice->LightEnable(0, true);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	
	m_pCamera->Update({ 0,0,0 });
	m_pZealots[0]->Update();
	/*for (auto zealot : m_pZealots) {
		zealot->Update();
	}*/
	///////////////////////////////////////////////////////////
	g_pAutoReleasePool->Drain();
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0,0,0),
		1.0f, 0);

	g_pD3DDevice->BeginScene();
	m_pGrid->Render();
	for (auto zealot : m_pZealots) {
		zealot->Render();
	}

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

bool cMainGame::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_LBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
		
	}
	break;
	case WM_RBUTTONDOWN:
	{
		POINT pt;
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);
	}
	break;
	case WM_CHAR:
	{
		if (m_pZealots.empty() == false) {
			if (wParam >= '1' && wParam <= '9') {
				int idx = wParam - '1';
				if (idx < m_pZealots[0]->GetNumAnimations()) {
					m_pZealots[0]->SetAnimation(idx, 2.0);
				}
			}
		}
	}
	break;
	default:
		break;
	}
	if (m_pCamera && m_pCamera->MsgProc(hWnd, message, wParam, lParam)) {
		return true;
	}
	return false;
}


void cMainGame::InitFont()
{
	D3DXFONT_DESC fd;
	ZeroMemory(&fd, sizeof(D3DXFONT_DESC));
	fd.Height = 20;
	fd.Width = 10;
	fd.Weight = FW_NORMAL;
	fd.Italic = false;
	fd.CharSet = DEFAULT_CHARSET;
	fd.OutputPrecision = OUT_DEFAULT_PRECIS;
	fd.PitchAndFamily = FF_DONTCARE;
	strcpy_s(fd.FaceName, "Serif");	
	D3DXCreateFontIndirect(g_pD3DDevice, &fd, &m_pFont);
}

void cMainGame::InitZealot()
{
	for (size_t i = 0; i < m_pZealots.size(); ++i) {
		SAFE_DELETE(m_pZealots[i]);
	}
	m_pZealots.clear();

	cSkinnedModel* pZealot = new cSkinnedModel();
	if (!pZealot->LoadFromX("Zealot/zealot.X")) {
		::MessageBoxA(g_hWnd, "XFile �ε� ����", "�ε� ����", MB_OK);
		SAFE_DELETE(pZealot);
	}
	m_pZealots.push_back(pZealot);
	m_pZealots.push_back(pZealot->Clone());
	m_pZealots.back()->SetPosition(5, 0, 0);

	pZealot->EnableControl(true);
}

void cMainGame::InitCamera()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup(D3DXVECTOR3(0,1.5,0));
}

void cMainGame::InitGrid()
{
	m_pGrid = new cGrid();
	m_pGrid->Setup();
	m_pGrid->SetOrigin(0, -0.2, 0);
}

void cMainGame::InitLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Direction = m_pCamera->GetLookDir();
	stLight.Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	g_pD3DDevice->SetLight(0, &stLight);
}

void cMainGame::DrawTextAnimationList()
{
	//char buffer[512];
	//int offset = 0;
	//int numAnims = m_pZealot->GetNumAnimations();
	//for(size_t i=0;i<numAnims;++i){
	//	std::string name;
	//	m_pZealot->GetAnimationName(i, name);
	//	offset += sprintf_s(&buffer[offset], 512 - offset, "%u : %s\n", i + 1, name.c_str());
	//}
	//RECT rect;
	//rect.left = rect.right = 10;
	//rect.top = rect.bottom = 10;
	//m_pFont->DrawTextA(NULL, buffer, offset, &rect, DT_NOCLIP|DT_LEFT, D3DCOLOR_XRGB(255,255,255));
}