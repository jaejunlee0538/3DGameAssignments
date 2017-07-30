#include "stdafx.h"
#include "cMainGame.h"
#include "KeyManager.h"
#include <time.h>
#define U(idx) (idx / 16.0f)
#define V(idx) (idx / 8.0f)
namespace SGA {
	IDirect3DTexture9 * g_textureIronman;
	cMainGame::cMainGame()
		:m_pD3D(NULL), m_pD3DDevice(NULL)
	{
		srand(time(0));
		_currentTime = 60 * 60;
	}

	cMainGame::~cMainGame()
	{
		SAFE_RELEASE(m_pD3D);
		SAFE_RELEASE(m_pD3DDevice);
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
		LogDebugMessage(TEXT("MaxLight : %d\n"), stCaps.MaxActiveLights);

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

		m_textureManager.init(m_pD3DDevice, NULL);

		m_grid.init(30, 30, 1, 1);
		m_piramid[0].init(0.5, 0.5, 3, D3DCOLOR_XRGB(255, 0, 0));
		m_piramid[0].SetPosition(3, 0, 0);
		m_piramid[0].SetAngle(0, 0, D3DXToRadian(90));
		m_piramid[1].init(0.5, 0.5, 3, D3DCOLOR_XRGB(0, 255, 0));
		m_piramid[1].SetPosition(0, 3, 0);
		m_piramid[1].SetAngle(0, 0, D3DXToRadian(180));
		m_piramid[2].init(0.5, 0.5, 3, D3DCOLOR_XRGB(0, 0, 255));
		m_piramid[2].SetPosition(0, 0, 3);
		m_piramid[2].SetAngle(D3DXToRadian(-90), 0, 0);

		m_character.setup(m_textureManager.createTexture(TEXT("Iron_Man_Skin_2.png")));
		_playerController.setPlayer(&m_character);

		m_floor.init(100, 2, 100, D3DXVECTOR3(0, 0, 0),
			m_textureManager.createTexture(TEXT("WoodPlanksWorn19_COL_VAR2_3K.jpg")), 
			{/*전*/U(2),V(2), U(4), V(4), /*후*/U(6), V(2), U(8), V(4),/*좌*/ U(0), V(2), U(2), V(4),/*우*/ U(4), V(2), U(6), V(4),/*상*/ U(2),V(0), U(4), V(2),/*하*/U(4), V(0), U(6), V(2) });
		m_floor.setPosition(0, -1, 0);
		m_floor.setRotation(0, 0, 0);
		Materials::initMaterial(_materialFloor, Colors::WHITE*0.1f, Colors::WHITE, Colors::WHITE, Colors::BLACK, 5);

		m_camera.SetAngle(D3DXToRadian(45), D3DXToRadian(180), 0);
		m_camera.SetDistanceToTarget(20);
		m_camera.SetLookTargetPosition(D3DXVECTOR3(0, 0, 0));

		RECT screenRect;
		GetClientRect(g_hWnd, &screenRect);

		D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(55), screenRect.right / (float)screenRect.bottom, 1, 1000);

		m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);
		m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		m_character.setState(SGA::MinecraftCharacter::STATE_IDLE);

		m_sunDir = D3DXVECTOR3(-1, -1, 0);
		D3DXVec3Normalize(&m_sunDir, &m_sunDir);
		Lights::initDirectionalLight(m_lightSun, Colors::WHITE*0.05f, Colors::WHITE*0.2f, Colors::WHITE*0.6f, m_sunDir);
		m_pD3DDevice->SetLight(0, &m_lightSun);
	}

	void cMainGame::Update()
	{
		if (_currentTime > 0) {
			_currentTime--;
			SGA::UserInputData input = GET_INPUT_MANAGER()->GenerateUserInput();
			if (GET_KEY_MANAGER()->isOnceKeyDown(VK_SHIFT)) {
				GenerateCube();
			}
			/*if (_currentTime % 60 == 0) {
				GenerateCube();
			}
*/
			TCHAR msg[256];
			_stprintf_s(msg, 256, TEXT("남은 시간 : %.2f 초"), _currentTime*1.0f / 60.0f);
			SetWindowText(g_hWnd, msg);

			_playerController.update(input);

			m_character.update(false);
			m_floor.update(false);
			m_camera.SetLookTargetPosition(m_character.getPosition());
			m_camera.Update(input);
			g_pD3DDevice->SetTransform(D3DTS_VIEW, &(m_camera.GetLookAtMatrix()));

			for (auto it = _boxObstacles.begin(); it != _boxObstacles.end(); ++it) {
				(*it)->update();
			}
			m_character.checkCollision(_boxObstacles);
			for (auto it = _boxObstacles.begin(); it != _boxObstacles.end();) {
				if ((*it)->getCurrentHP() == 0) {
					it = _boxObstacles.erase(it);
					_currentTime += 1.5 * 60;
				}
				else {
					++it;
				}
			}
		}
		else {
			TCHAR msg[256];
			_stprintf_s(msg, 256, TEXT("Game Over"));
			SetWindowText(g_hWnd, msg);
		}
		//////////////////////태양 회전///////////////////////////////
		D3DXMATRIX tmp;
		D3DXVec3TransformNormal(&m_sunDir, &m_sunDir, D3DXMatrixRotationY(&tmp, 0.005));
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
		m_pD3DDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		m_pD3DDevice->BeginScene();

		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		m_grid.Render();
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
		m_pD3DDevice->LightEnable(0, true);
		m_piramid[0].Render();
		m_piramid[1].Render();
		m_piramid[2].Render();

		m_character.render();
		for (auto it = _boxObstacles.begin(); it != _boxObstacles.end(); ++it) {
			(*it)->render();
		}
		m_pD3DDevice->SetMaterial(&_materialFloor);
		m_floor.render();

		m_pD3DDevice->EndScene();

		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}

	void cMainGame::GenerateCube()
	{
		std::shared_ptr<cObstacle> box(new cObstacle());
		box->init(5,5,5, D3DXVECTOR3(0, -2.5, 0), { m_textureManager.createTexture(TEXT("Box1.png")),m_textureManager.createTexture(TEXT("Box2.png")),m_textureManager.createTexture(TEXT("Box3.png")) });
		box->setRotation(0, 0, 0);
		box->setPosition(rand() % 100 - 50, 30, rand() % 100 - 50);
		_boxObstacles.push_back(box);
	}
}