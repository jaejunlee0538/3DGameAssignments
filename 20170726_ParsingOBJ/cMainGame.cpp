#include "stdafx.h"
#include "cMainGame.h"
#include "KeyManager.h"
#include "OBJLoader.h"
#include <time.h>

namespace SGA {
	IDirect3DTexture9 * g_textureIronman;
	cMainGame::cMainGame()
		:m_pD3D(NULL), m_pD3DDevice(NULL)
	{
	}

	cMainGame::~cMainGame()
	{
		_meshBox.reset();
		_meshRustyBox.reset();
		_meshModel.reset();
		GET_TEXTURE_MGR()->release();
		SAFE_RELEASE(m_pD3D);
		//SAFE_RELEASE(m_pD3DDevice);
		if (m_pD3DDevice) {
			ULONG refCount = m_pD3DDevice->Release();
			assert(refCount == 0 && "COM 객체가 모두 반환되지 않았습니다.");
		}
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
		//D3DXVec3CatmullRom()
	
		m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			g_hWnd,
			nVertexProcessing,
			&stD3DPP,
			&m_pD3DDevice);
		g_pD3DDevice = m_pD3DDevice;

		GET_TEXTURE_MGR()->init(m_pD3DDevice, NULL);

		RECT screenRect;
		GetClientRect(g_hWnd, &screenRect);
		m_camera.Setup(D3DXToRadian(55), screenRect.right / (float)screenRect.bottom, 1, 1000);
		m_camera.SetAngle(D3DXToRadian(45), D3DXToRadian(180), 0);
		m_camera.SetDistanceToTarget(20);
		m_camera.SetLookTargetPosition(D3DXVECTOR3(0, 0, 0));
		
		m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_camera.GetLookAtMatrix());
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_camera.GetProjectionMatrix());
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

		m_sunDir = D3DXVECTOR3(1, -1, 1);
		D3DXVec3Normalize(&m_sunDir, &m_sunDir);
		Lights::initDirectionalLight(m_lightSun, Colors::WHITE, Colors::WHITE, Colors::WHITE*0.4f, m_sunDir);
		m_pD3DDevice->SetLight(0, &m_lightSun);
		m_pD3DDevice->LightEnable(0, true);

		_grid.init(30, 30, 1, 1);

		ModelLoad::OBJLoader objLoader;
		objLoader.load3DModel(TEXT("obj/box.obj"), _meshBox);
		objLoader.load3DModel(TEXT("obj/Map.obj"), _meshModel);
		objLoader.load3DModel(TEXT("obj/RustyBox.obj"), _meshRustyBox);
	}

	void cMainGame::Update()
	{
		auto input = InputManager::getSingleton()->GenerateUserInput();
		m_camera.Update(input);
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &(m_camera.GetLookAtMatrix()));
		//////////////////////태양 회전///////////////////////////////
		D3DXMATRIX tmp;
		D3DXVec3TransformNormal(&m_sunDir, &m_sunDir, D3DXMatrixRotationY(&tmp, 0.05));
		m_lightSun.Direction = m_sunDir;
		m_pD3DDevice->SetLight(0, &m_lightSun);
		/////////////////////////////////////////////////////
	}

	void cMainGame::Render()
	{
		m_pD3DDevice->Clear(NULL,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0, 0, 0),
			1.0f, 0);

		m_pD3DDevice->BeginScene();
		//그리드 렌더링
		
		//_grid.Render();
		
		////메시 오브젝트 렌더링
		////박스 렌더링
		D3DXMATRIX matrix1;
		D3DXMatrixTranslation(&matrix1, 10, 0, -10);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix1);
		_meshBox->render(m_pD3DDevice);

		//D3DXMatrixTranslation(&matrix1, 10, 0, 10);
		//m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix1);
		//_meshRustyBox->render(m_pD3DDevice);

		//구조물 렌더링
		D3DXMATRIX scaleMat, rotMat;
		D3DXMatrixScaling(&scaleMat, 0.01, 0.01, 0.01);
		D3DXMatrixRotationX(&rotMat, D3DXToRadian(-90));
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &(scaleMat*rotMat));
		_meshModel->render(m_pD3DDevice);
		//_meshModel->renderGroup(m_pD3DDevice, "Rectangle16");

		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}