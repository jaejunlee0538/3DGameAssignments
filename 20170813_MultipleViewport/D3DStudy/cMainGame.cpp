#include "stdafx.h"
#include "cMainGame.h"
#include "KeyManager.h"
#include <time.h>
#include "MeshObject.h"
#include "Axis.h"
#include "PlayerControlObject.h"
#define CHARACTER_RUN_TIME	10.0f
namespace SGA {
	IDirect3DTexture9 * g_textureIronman;
	cMainGame::cMainGame()
		:m_pD3D(NULL), m_pD3DDevice(NULL),_time(0.0f), _animationClip(NULL), loadedObject(NULL), m_pSwapChain(NULL)
	{
	}

	cMainGame::~cMainGame()
	{
		GET_TEXTURE_MGR()->release();
		SAFE_RELEASE(m_pSwapChain);
		SAFE_RELEASE(m_pD3D);
		SAFE_DELETE(_animationClip);
		SAFE_DELETE(loadedObject);
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
		LogDebugMessage("최대 지원 렌더 타겟 갯수 : %d\n", stCaps.NumSimultaneousRTs);

		D3DPRESENT_PARAMETERS stD3DPP;
		ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
		stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		stD3DPP.Windowed = TRUE;
		stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
		stD3DPP.EnableAutoDepthStencil = TRUE;
		stD3DPP.AutoDepthStencilFormat = D3DFMT_D16;
		//stD3DPP.M
		m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			g_hWnd,
			nVertexProcessing,
			&stD3DPP,
			&m_pD3DDevice);
		g_pD3DDevice = m_pD3DDevice;

		////////////////////////두번째 뷰포트//////////////////////////////////////
		D3DPRESENT_PARAMETERS stD3DPP2;
		ZeroMemory(&stD3DPP2, sizeof(D3DPRESENT_PARAMETERS));
		stD3DPP2.SwapEffect = D3DSWAPEFFECT_DISCARD;
		stD3DPP2.Windowed = TRUE;
		stD3DPP2.EnableAutoDepthStencil = TRUE;
		stD3DPP2.AutoDepthStencilFormat = D3DFMT_D16;
		stD3DPP2.BackBufferFormat = D3DFMT_UNKNOWN;
		stD3DPP2.BackBufferCount = 1;
		stD3DPP2.BackBufferWidth = 200;
		stD3DPP2.BackBufferHeight = 200;

		m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			g_hWnd,
			nVertexProcessing,
			&stD3DPP2,
			&m_pD3DDevice2);
		D3DVIEWPORT9 viewport;
		viewport.Height = 200;
		viewport.Width = 200;
		viewport.MaxZ = 1.0f;
		viewport.MinZ = 0.0f;
		viewport.X = 0;
		viewport.Y = 0;
		m_pD3DDevice2->SetViewport(&viewport);
		
		////////////////////////////////////////////////////////////////
		GET_TEXTURE_MGR()->init(m_pD3DDevice, NULL);

		RECT screenRect;
		GetClientRect(g_hWnd, &screenRect);
		m_camera.Setup(D3DXToRadian(55), screenRect.right / (float)screenRect.bottom, 1, 1000);
		m_camera.SetAngle(D3DXToRadian(45), D3DXToRadian(180), 0);
		m_camera.SetDistanceToTarget(6);
		m_camera.SetLookTargetPosition(D3DXVECTOR3(0, 0, 0));
		
		m_camera2.Setup(D3DXToRadian(55), 1.0, 1, 1000);
		m_camera2.SetAngle(0, 0, 0);
		m_camera2.SetDistanceToTarget(6);
		m_camera2.SetLookTargetPosition(D3DXVECTOR3(0, 0, 0));

		m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_camera.GetLookAtMatrix());
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_camera.GetProjectionMatrix());
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

		m_pD3DDevice2->SetTransform(D3DTS_VIEW, &m_camera2.GetLookAtMatrix());
		m_pD3DDevice2->SetTransform(D3DTS_PROJECTION, &m_camera2.GetProjectionMatrix());
		m_pD3DDevice2->SetRenderState(D3DRS_LIGHTING, true);
		m_pD3DDevice2->SetRenderState(D3DRS_SPECULARENABLE, true);
		m_pD3DDevice2->SetRenderState(D3DRS_NORMALIZENORMALS, true);

		m_sunDir = D3DXVECTOR3(1, -1, 1);
		D3DXVec3Normalize(&m_sunDir, &m_sunDir);
		Lights::initDirectionalLight(m_lightSun, Colors::WHITE, Colors::WHITE, Colors::WHITE*0.4f, m_sunDir);
		m_pD3DDevice->SetLight(0, &m_lightSun);
		m_pD3DDevice->LightEnable(0, true);

		_grid.init(30, 30, 1, 1);

		ASEParser aseParser;
		MeshObject * meshObject = nullptr;
		aseParser.load("woman/woman_01_all.ASE", &meshObject, &_animationClip);
		_timeScale = (aseParser._tickPerFrame * aseParser._frameSpeed) / 1000.0f;
		_startFrame = aseParser._firstFrame * aseParser._tickPerFrame;
		_endFrame = aseParser._lastFrame * aseParser._tickPerFrame;

		loadedObject = new PlayerControlObject();
		loadedObject->setPosition(0, 0, 0);
		loadedObject->insertChild(meshObject);
		loadedObject->insertChild(new Axis(1.0f));
		loadedObject->insertChild("Bip01",new Axis(1.f));
	}

	void cMainGame::Update()
	{
		int tickCount = _startFrame + (static_cast<DWORD>(_timeScale*GetTickCount()) % (_endFrame - _startFrame));
		
		auto input = InputManager::getSingleton()->GenerateUserInput();
		m_camera.Update(input);
		
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &(m_camera.GetLookAtMatrix()));
		m_pD3DDevice2->SetTransform(D3DTS_VIEW, &(m_camera2.GetLookAtMatrix()));

		if (GET_KEY_MANAGER()->isStayKeyDown(VK_LEFT)) {
			loadedObject->rotateLocal(0.05, 0.05, 0);
		}
		else if (GET_KEY_MANAGER()->isStayKeyDown(VK_RIGHT)) {
			loadedObject->rotateLocal(-0.05, -0.05, 0);
		}

		AnimationSnapshots animSnapshot;
		_animationClip->interpolate(tickCount, animSnapshot);
		loadedObject->setKeyFrameAnimation(animSnapshot);
		loadedObject->update(false);

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
		_grid.render(m_pD3DDevice);

		loadedObject->render(m_pD3DDevice);
		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
		//////////////////////////////////////////

		m_pD3DDevice2->Clear(NULL,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(0, 0, 0),
			1.0f, 0);

		m_pD3DDevice2->BeginScene();
		_grid.render(m_pD3DDevice2);

		loadedObject->render(m_pD3DDevice2);
		m_pD3DDevice2->EndScene();
		m_pD3DDevice2->Present(NULL, NULL, NULL, NULL);
		//////////////////////////////////////////



	}
}