#include "stdafx.h"
#include "cMainGame.h"
#include <time.h>

cMainGame::cMainGame()
	:m_pD3D(NULL), m_pD3DDevice(NULL)
{
	srand(time(0));
}


cMainGame::~cMainGame()
{
	SAFE_RELEASE(m_pD3D)
	SAFE_RELEASE(m_pD3DDevice)
}

void cMainGame::Setup()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 stCaps;
	int nVertexProcessing;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);
	if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = TRUE;
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D16;

	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		g_hWnd,
		nVertexProcessing,
		&stD3DPP,
		&m_pD3DDevice);
	g_pD3DDevice = m_pD3DDevice;

	m_box.SetPosition(0, 0, 0);
	m_box.init(1, 1, 2);
	m_grid.init(30, 30, 1, 1);
	m_piramid[0].init(0.2,0.2, 3, D3DCOLOR_XRGB(255, 0, 0)); 
	m_piramid[0].SetPosition(3, 0, 0);
	m_piramid[0].SetAngle(0, 0, D3DXToRadian(90));
	m_piramid[1].init(0.2, 0.2, 3, D3DCOLOR_XRGB(0, 255, 0));
	m_piramid[1].SetPosition(0, 3, 0);
	m_piramid[1].SetAngle(0, 0, D3DXToRadian(180));
	m_piramid[2].init(0.2, 0.2, 3, D3DCOLOR_XRGB(0, 0, 255));
	m_piramid[2].SetPosition(0, 0, 3);
	m_piramid[2].SetAngle(D3DXToRadian(-90), 0, 0);

	m_camera.SetAngle(D3DXToRadian(45), 0, 0);
	m_camera.SetLookTargetPosition(m_box.GetPosition());
	
	RECT screenRect;
	GetClientRect(g_hWnd, &screenRect);
	
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(55), screenRect.right / (float)screenRect.bottom, 1, 1000);

	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void cMainGame::Update()
{
	SGA::UserInputData input = GET_INPUT_MANAGER()->GenerateUserInput();
	m_box.Update(input);
	m_camera.Update(input);
	m_camera.SetLookTargetPosition(m_box.GetPosition());
	g_pD3DDevice->SetTransform(D3DTS_VIEW,&( m_camera.GetLookAtMatrix()));
}

void cMainGame::Render()
{
	m_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f, 0);
	m_pD3DDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	m_pD3DDevice->BeginScene();
	
	m_box.Render();
	m_grid.Render();
	m_piramid[0].Render();
	m_piramid[1].Render();
	m_piramid[2].Render();


	m_pD3DDevice->EndScene();

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
