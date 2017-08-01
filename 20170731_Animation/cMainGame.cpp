#include "stdafx.h"
#include "cMainGame.h"
#include "KeyManager.h"
#include <time.h>
#include "MeshObject.h"
#include "Axis.h"
#define CHARACTER_RUN_TIME	10.0f
namespace SGA {
	IDirect3DTexture9 * g_textureIronman;
	cMainGame::cMainGame()
		:m_pD3D(NULL), m_pD3DDevice(NULL),_time(0.0f), _animationClip(NULL), loadedObject(NULL)
	{
	}

	cMainGame::~cMainGame()
	{
		GET_TEXTURE_MGR()->release();
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

		GET_TEXTURE_MGR()->init(m_pD3DDevice, NULL);

		RECT screenRect;
		GetClientRect(g_hWnd, &screenRect);
		m_camera.Setup(D3DXToRadian(55), screenRect.right / (float)screenRect.bottom, 1, 1000);
		m_camera.SetAngle(D3DXToRadian(45), D3DXToRadian(180), 0);
		m_camera.SetDistanceToTarget(6);
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

		ASEParser aseParser;
		aseParser.load("woman/woman_01_all.ASE", &loadedObject, &_animationClip);
		_timeScale = (aseParser._tickPerFrame * aseParser._frameSpeed) / 1000.0f;
		_startFrame = aseParser._firstFrame * aseParser._tickPerFrame;
		_endFrame = aseParser._lastFrame * aseParser._tickPerFrame;

		auto* tmp = loadedObject;
		loadedObject = new MeshObject();
		loadedObject->insertChild(tmp);
		loadedObject->insertChild(new Axis(1.f));
		loadedObject->translate(2, 0, 2);
	}

	void cMainGame::Update()
	{
		int tickCount = _startFrame + (static_cast<DWORD>(_timeScale*GetTickCount()) % (_endFrame - _startFrame));
		
		auto input = InputManager::getSingleton()->GenerateUserInput();
		m_camera.Update(input);
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &(m_camera.GetLookAtMatrix()));

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
		_grid.render();

		loadedObject->render();
		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}