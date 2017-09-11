#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cFrameHierarchyAllocator.h"
#include "cSkinnedModel.h"

cMainGame::cMainGame(void)
	: m_pGrid(NULL)
	, m_pCamera(NULL),
	m_pZealot(NULL),
	m_pFont(NULL),
	m_pGroundBox(NULL),
	m_pEffectDrawShadowMap(NULL),
	m_pEffectDrawScene(NULL)
{
}

cMainGame::~cMainGame(void)
{
	SAFE_RELEASE(m_pFont);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pZealot);
	SAFE_RELEASE(m_pGroundBox);
	SAFE_RELEASE(m_pBall);
	SAFE_RELEASE(m_pEffectDrawShadowMap);
	SAFE_RELEASE(m_pEffectDrawScene);

	//전역 객체 삭제
	g_pObjectPool->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

bool cMainGame::Setup()
{
	InitFont();
	InitCamera();
	InitLight();
	InitGrid();
	InitZealot();
	InitGround();
	InitBall();

	ID3DXBuffer* errorBuffer = NULL;

	D3DXCreateEffectFromFile(
		g_pD3DDevice,
		"Effects/DrawScene.fx",
		0,
		0,
		D3DXSHADER_DEBUG,
		0,
		&m_pEffectDrawScene,
		&errorBuffer
		);
	if (errorBuffer) {
		LogDebugMessage("%s\n", (char*)errorBuffer->GetBufferPointer());
	}
	if (m_pEffectDrawScene==NULL) {
		return false;
	}
	return true;
}

void cMainGame::Update()
{
	g_pTimeManager->Update();
	
	m_pCamera->Update({ 0,0,0 });
	if (m_pZealot) {
		m_pZealot->Update();
	}
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
	
	//if (m_pZealot) {
	//	m_pZealot->Render();
	//	DrawTextAnimationList();
	//}
	D3DXHANDLE hViewMatrix;
	hViewMatrix = m_pEffectDrawScene->GetParameterByName(0, "gViewMatrix");
	m_pEffectDrawScene->SetMatrix(hViewMatrix, &m_pCamera->GetViewMatrix());

	D3DXHANDLE hProjMatrix;
	hProjMatrix = m_pEffectDrawScene->GetParameterByName(0, "gProjMatrix");
	m_pEffectDrawScene->SetMatrix(hProjMatrix, &m_pCamera->GetProjMatrix());

	D3DXHANDLE hEyePosW;
	hEyePosW = m_pEffectDrawScene->GetParameterByName(0, "gEyePosW");
	m_pEffectDrawScene->SetVector(hEyePosW, &m_pCamera->GetPositionVec4());

	D3DXHANDLE hDirectionLight;
	hDirectionLight = m_pEffectDrawScene->GetParameterByName(0, "gDirectionalLights");
	m_pEffectDrawScene->SetRawValue(hDirectionLight, m_dirLights, 0, sizeof(EffectTypes::DirectionalLight) * 3);

	D3DXHANDLE hNumDirectionLights;
	hNumDirectionLights = m_pEffectDrawScene->GetParameterByName(0, "gNumDirectionalLights");
	m_pEffectDrawScene->SetInt(hNumDirectionLights, m_numDirLights);

	if (m_pGroundBox) {
		D3DXMATRIXA16	matWorld;
		D3DXVECTOR4 vec4;
		D3DXMatrixIdentity(&matWorld);

		D3DXHANDLE hWorldMatrix;
		hWorldMatrix = m_pEffectDrawScene->GetParameterByName(0, "gWorldMatrix");
		m_pEffectDrawScene->SetMatrix(hWorldMatrix, &matWorld);

		D3DXHANDLE hMaterial;
		hMaterial = m_pEffectDrawScene->GetParameterByName(0, "gMaterial");
		m_pEffectDrawScene->SetRawValue(hMaterial, &m_mtlGround, 0, sizeof(EffectTypes::Material));

		D3DXHANDLE hTech = m_pEffectDrawScene->GetTechniqueByName("BasicLight");
		m_pEffectDrawScene->SetTechnique(hTech);

		UINT numPass;
		m_pEffectDrawScene->Begin(&numPass, 0);
		for (UINT iPass = 0; iPass < numPass; ++iPass) {
			m_pEffectDrawScene->BeginPass(iPass);
			m_pGroundBox->DrawSubset(0);
			m_pEffectDrawScene->EndPass();
		}
		m_pEffectDrawScene->End();
	}
	if (m_pBall) {
		D3DXMATRIXA16	matWorld;
		D3DXVECTOR4 vec4;
		D3DXMatrixTranslation(&matWorld, 1,1,1);

		D3DXHANDLE hWorldMatrix;
		hWorldMatrix = m_pEffectDrawScene->GetParameterByName(0, "gWorldMatrix");
		m_pEffectDrawScene->SetMatrix(hWorldMatrix, &matWorld);

		D3DXHANDLE hMaterial;
		hMaterial = m_pEffectDrawScene->GetParameterByName(0, "gMaterial");
		m_pEffectDrawScene->SetRawValue(hMaterial, &m_mtlBall, 0, sizeof(EffectTypes::Material));

		D3DXHANDLE hTech = m_pEffectDrawScene->GetTechniqueByName("BasicLight");
		m_pEffectDrawScene->SetTechnique(hTech);

		UINT numPass;
		m_pEffectDrawScene->Begin(&numPass, 0);
		for (UINT iPass = 0; iPass < numPass; ++iPass) {
			m_pEffectDrawScene->BeginPass(iPass);
			m_pBall->DrawSubset(0);
			m_pEffectDrawScene->EndPass();
		}
		m_pEffectDrawScene->End();
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
		if (m_pZealot) {
			if (wParam >= '1' && wParam <= '9') {
				int idx = wParam - '1';
				if (idx < m_pZealot->GetNumAnimations()) {
					m_pZealot->SetAnimation(idx);
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


void cMainGame::InitBall()
{
	LPD3DXBUFFER materialBuffer = NULL;
	DWORD numMaterials = 0;
	LPD3DXMESH pMesh = NULL;
	if (FAILED(D3DXLoadMeshFromXA(
		"Zealot/Ball.X",
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		NULL,
		&materialBuffer,
		NULL,
		&numMaterials,
		&pMesh))) {
		::MessageBox(g_hWnd, "Load GroundBox Failed", 0, 0);
		return;
	}
	
	D3DXMATERIAL material = *(D3DXMATERIAL*)materialBuffer->GetBufferPointer();
	material.MatD3D.Ambient = material.MatD3D.Diffuse;
	m_mtlBall.InitFromD3DMaterial(material.MatD3D);
	materialBuffer->Release();

	m_pBall = pMesh;
}

void cMainGame::InitGround()
{
	LPD3DXBUFFER materialBuffer = NULL;
	DWORD numMaterials = 0;
	LPD3DXMESH pMesh = NULL;
	if (FAILED(D3DXLoadMeshFromXA(
		"Zealot/GroundBox.X",
		D3DXMESH_MANAGED,
		g_pD3DDevice,
		NULL,
		&materialBuffer,
		NULL,
		&numMaterials,
		&pMesh))) {
		::MessageBox(g_hWnd,"Load GroundBox Failed" , 0, 0);
		return;
	}

	D3DXMATERIAL material = *(D3DXMATERIAL*)materialBuffer->GetBufferPointer();
	material.MatD3D.Ambient = material.MatD3D.Diffuse;
	m_mtlGround.InitFromD3DMaterial(material.MatD3D);
	materialBuffer->Release();

	m_pGroundBox = pMesh;
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
	SAFE_DELETE(m_pZealot);
	m_pZealot = new cSkinnedModel();
	if (!m_pZealot->LoadFromX("Zealot/zealot.X")) {
		::MessageBoxA(g_hWnd, "XFile 로드 실패", "로드 에러", MB_OK);
		SAFE_DELETE(m_pZealot);
	}
}

void cMainGame::InitCamera()
{
	m_pCamera = new cCamera;
	
	m_pCamera->Setup(D3DXVECTOR3(0,0,0));
}

void cMainGame::InitGrid()
{
	m_pGrid = new cGrid();
	m_pGrid->Setup();
	m_pGrid->SetOrigin(0, -0.2, 0);
}

void cMainGame::InitLight()
{
	m_numDirLights = 1;
	m_dirLights[0].Ambient = D3DXVECTOR4(0.05f, 0.05f, 0.05f, 1.0f);
	m_dirLights[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_dirLights[0].Specular = D3DXVECTOR4(1.0f,1.0f,1.0f, 1.0f);

	D3DXVECTOR3 lightPos(400, 500, 300);
	D3DXVECTOR3 lightDir;
	D3DXVec3Normalize(&lightDir, &-lightPos);
	m_dirLights[0].Direction = lightDir;
}

void cMainGame::DrawTextAnimationList()
{
	char buffer[512];
	int offset = 0;
	int numAnims = m_pZealot->GetNumAnimations();
	for(size_t i=0;i<numAnims;++i){
		std::string name;
		m_pZealot->GetAnimationName(i, name);
		offset += sprintf_s(&buffer[offset], 512 - offset, "%u : %s\n", i + 1, name.c_str());
	}
	RECT rect;
	rect.left = rect.right = 10;
	rect.top = rect.bottom = 10;
	m_pFont->DrawTextA(NULL, buffer, offset, &rect, DT_NOCLIP|DT_LEFT, D3DCOLOR_XRGB(255,255,255));
}