#include "stdafx.h"
#include "cMainGame.h"
#include "KeyManager.h"
#include <time.h>
#define CHARACTER_RUN_TIME	10.0f
namespace SGA {
	IDirect3DTexture9 * g_textureIronman;
	cMainGame::cMainGame()
		:m_pD3D(NULL), m_pD3DDevice(NULL), _character1(NULL), _character2(NULL), _time(0.0f)
	{
	}

	cMainGame::~cMainGame()
	{
		SAFE_DELETE(_character1);
		SAFE_DELETE(_character2);

		GET_TEXTURE_MGR()->release();
		SAFE_RELEASE(m_pD3D);

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
		
		_bezierCurve[0].setMode(BezierCurve::INTERPOLATE_BEZIER);
		_bezierCurve[1].setMode(BezierCurve::INTERPOLATE_BEZIER);
		_bezierCurve[2].setMode(BezierCurve::INTERPOLATE_BEZIER);

		_linearCurve.setMode(BezierCurve::INTERPOLATE_LINEAR);

		for (int i = 0; i < 6; ++i) {
			float x, z;
			float angle = D3DXToRadian(i * 60);
			x = 10.0f * cosf(angle);
			z = 10.0f * sinf(angle);
			_hexagon.push_back(VertexPosDiff(x, 0, z, D3DCOLOR_XRGB(255, 0, 0)));
		}
		_hexagon.push_back(VertexPosDiff(10, 0, 0, D3DCOLOR_XRGB(255, 0, 0)));
		_bezierCurve[0].pushBackPoint(_hexagon[0].p.x, 0, _hexagon[0].p.z);
		_bezierCurve[0].pushBackPoint(_hexagon[1].p.x, 0, _hexagon[1].p.z);
		_bezierCurve[0].pushBackPoint(_hexagon[2].p.x, 0, _hexagon[2].p.z);
		_bezierCurve[1].pushBackPoint(_hexagon[2].p.x, 0, _hexagon[2].p.z);
		_bezierCurve[1].pushBackPoint(_hexagon[3].p.x, 0, _hexagon[3].p.z);
		_bezierCurve[1].pushBackPoint(_hexagon[4].p.x, 0, _hexagon[4].p.z);
		_bezierCurve[2].pushBackPoint(_hexagon[4].p.x, 0, _hexagon[4].p.z);
		_bezierCurve[2].pushBackPoint(_hexagon[5].p.x, 0, _hexagon[5].p.z);
		_bezierCurve[2].pushBackPoint(_hexagon[0].p.x, 0, _hexagon[0].p.z);
		_linearCurve.pushBackPoint(_hexagon[0].p.x, 0, _hexagon[0].p.z);
		_linearCurve.pushBackPoint(_hexagon[1].p.x, 0, _hexagon[1].p.z);
		_linearCurve.pushBackPoint(_hexagon[2].p.x, 0, _hexagon[2].p.z);
		_linearCurve.pushBackPoint(_hexagon[3].p.x, 0, _hexagon[3].p.z);
		_linearCurve.pushBackPoint(_hexagon[4].p.x, 0, _hexagon[4].p.z);
		_linearCurve.pushBackPoint(_hexagon[5].p.x, 0, _hexagon[5].p.z);
		_linearCurve.pushBackPoint(_hexagon[0].p.x, 0, _hexagon[0].p.z);

		_character1 = new MinecraftCharacter();
		_character2 = new MinecraftCharacter();
		_character2->setPosition(4, 0, 0);
		_character1->setup(GET_TEXTURE_MGR()->createTexture(TEXT("Iron_Man_Skin_2.png")));
		_character2->setup(GET_TEXTURE_MGR()->createTexture(TEXT("aooni_3443904.png")));
	}

	void cMainGame::Update()
	{
		_time += 1.0f / 60.0f;
		if (_time >= CHARACTER_RUN_TIME) {
			_time -= CHARACTER_RUN_TIME;
		}
		auto input = InputManager::getSingleton()->GenerateUserInput();
		m_camera.Update(input);
		g_pD3DDevice->SetTransform(D3DTS_VIEW, &(m_camera.GetLookAtMatrix()));

		//if (GET_KEY_MANAGER()->isStayKeyDown('W')) {
		//	_character2->translateLocal(0, 0, 0.5);
		//}
		//if (GET_KEY_MANAGER()->isStayKeyDown('S')) {
		//	_character2->translateLocal(0, 0, -0.5);
		//}
		//if (GET_KEY_MANAGER()->isStayKeyDown('A')) {
		//	_character2->rotateLocal(0, -0.1, 0);
		//}
		//if (GET_KEY_MANAGER()->isStayKeyDown('D')) {
		//	_character2->rotateLocal(0, 0.1, 0);
		//}
		D3DXVECTOR3 pos;
		D3DXVECTOR3 dirBezier;
		if (_time < CHARACTER_RUN_TIME / 3) {
			pos = _bezierCurve[0].computeFromTime(CHARACTER_RUN_TIME/3, _time);
			dirBezier = _bezierCurve[0].getCurrentDirection();
		}
		else if (_time < 2*CHARACTER_RUN_TIME / 3) {
			pos = _bezierCurve[1].computeFromTime(CHARACTER_RUN_TIME/3, _time- CHARACTER_RUN_TIME / 3);
			dirBezier = _bezierCurve[1].getCurrentDirection();
		}
		else {
			pos = _bezierCurve[2].computeFromTime(CHARACTER_RUN_TIME/3, _time-2* CHARACTER_RUN_TIME / 3);
			dirBezier = _bezierCurve[2].getCurrentDirection();
		}
		auto pos2 = _linearCurve.computeFromTime(CHARACTER_RUN_TIME, _time);
		_character1->setPosition(pos.x, pos.y, pos.z);
		_character1->setRotation(0, atan2(dirBezier.x, dirBezier.z), 0);
		_character2->setPosition(pos2.x, pos2.y, pos2.z);
		_character2->setRotation(0, D3DX_PI/2 - atan2(_linearCurve.getCurrentDirection().z, _linearCurve.getCurrentDirection().x), 0);

		_character1->update(false);
		_character2->update(false);

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
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pD3DDevice->SetTransform(D3DTS_WORLD, &Identity4X4);
		m_pD3DDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 6, _hexagon.data(), sizeof(VertexPosDiff));
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		_character1->render();
		_character2->render();
		m_pD3DDevice->EndScene();
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}