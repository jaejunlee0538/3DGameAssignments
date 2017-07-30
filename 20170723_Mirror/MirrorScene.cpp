#include "stdafx.h"
#include "MirrorScene.h"
#include "KeyManager.h"

MirrorScene::MirrorScene()
	:m_pD3D(NULL), m_pD3DDevice(NULL)
{
}

MirrorScene::~MirrorScene()
{

	SAFE_RELEASE(m_pMeshTeapot);
	SAFE_RELEASE(m_pVBTeapot);
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pD3DDevice);
	SAFE_RELEASE(m_vertexBufferPlate);
	SAFE_RELEASE(m_indexBufferPlate);
}

void MirrorScene::Setup()
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 d3dCaps;
	int nVertexProcessing;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps);
	if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(stD3DPP));
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	stD3DPP.Windowed = TRUE;

	m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		g_hWnd, nVertexProcessing,
		&stD3DPP, &m_pD3DDevice);
	g_pD3DDevice = m_pD3DDevice;

	m_worldUp = D3DXVECTOR3(0, 1, 0);

	m_teapotPos = D3DXVECTOR3(0, 0, 0);
	D3DXCreateTeapot(m_pD3DDevice, &m_pMeshTeapot, &m_pVBTeapot);

	RECT rect;
	GetClientRect(g_hWnd, &rect);
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(65), rect.right / (float)rect.bottom, 1, 1000);
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMATRIX matView;
	m_camPos = D3DXVECTOR3(8, 4, 1);
	D3DXMatrixLookAtLH(&matView, &m_camPos, &m_teapotPos, &m_worldUp);
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
	////////////////////////////////////////////////////////////////
	//거울 모델
	tagVertexXYZ verticesPlate[4] = {
		tagVertexXYZ(-2, 0, -2),
		tagVertexXYZ(-2, 0, 2),
		tagVertexXYZ(2, 0, 2),
		tagVertexXYZ(2, 0,-2)
	};
	int indicesPlate[6] = { 0, 1, 2, 0, 2, 3 };

	m_pD3DDevice->CreateVertexBuffer(
		4 * sizeof(tagVertexXYZ), 
		D3DUSAGE_WRITEONLY,
		tagVertexXYZ::FVF,
		D3DPOOL_MANAGED,
		&m_vertexBufferPlate,
		NULL
	);
	m_pD3DDevice->CreateIndexBuffer(
		6 * sizeof(int),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_indexBufferPlate,
		NULL
	);

	tagVertexXYZ* pVertex;
	m_vertexBufferPlate->Lock(0, 0, (void**)&pVertex, 0);
	memcpy(pVertex, verticesPlate, sizeof(tagVertexXYZ) * 4);
	m_vertexBufferPlate->Unlock();

	int *pIndex;
	m_indexBufferPlate->Lock(0, 0, (void**)&pIndex, 0);
	memcpy(pIndex, indicesPlate, sizeof(int) * 6);
	m_indexBufferPlate->Unlock();
	/////////////////////////////////////////////////////
	//거울 재질
	m_materialMirror.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_materialMirror.Ambient = D3DXCOLOR(0, 0, 0, 1.0f);
	m_materialMirror.Specular = D3DXCOLOR(0, 0, 0, 1.0f);
	m_materialMirror.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	m_materialMirror.Power = 4.0f;
	/////////////////////////////////////////////////////
	//주전자 재질 설정
	m_materialTeapot.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_materialTeapot.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_materialTeapot.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_materialTeapot.Emissive = D3DXCOLOR(0, 0, 0, 1.0f);
	m_materialTeapot.Power = 4.0f;
	/////////////////////////////////////////////////////
	//광원 설정
	ZeroMemory(&m_lightSun, sizeof(m_lightSun));
	m_lightSun.Type = D3DLIGHT_DIRECTIONAL;
	m_lightSun.Direction = D3DXVECTOR3(0.5, -0.15, 0.2);
	m_lightSun.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) * 0.25f;
	m_lightSun.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightSun.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pD3DDevice->SetLight(0, &m_lightSun);
	/////////////////////////////////////////////////////
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3DDevice->LightEnable(0, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}

void MirrorScene::Update()
{
	if (GET_KEY_MANAGER()->isStayKeyDown('A')) {
		D3DXVECTOR3 lightDir = m_lightSun.Direction;
		D3DXMATRIX rotMat;
		D3DXMatrixRotationY(&rotMat, 0.05);
		D3DXVec3TransformNormal(&lightDir, &lightDir, &rotMat);
		m_lightSun.Direction = lightDir;
		m_pD3DDevice->SetLight(0, &m_lightSun);
	}
	if (GET_KEY_MANAGER()->isStayKeyDown('D')) {
		D3DXVECTOR3 lightDir = m_lightSun.Direction;
		D3DXMATRIX rotMat;
		D3DXMatrixRotationY(&rotMat, -0.05);
		D3DXVec3TransformNormal(&lightDir, &lightDir, &rotMat);
		m_lightSun.Direction = lightDir;
		m_pD3DDevice->SetLight(0, &m_lightSun);
	}

	if (GET_KEY_MANAGER()->isStayKeyDown(VK_LEFT)) {
		m_teapotPos.z -= 0.1f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown(VK_RIGHT)) {
		m_teapotPos.z += 0.1f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown(VK_UP)) {
		m_teapotPos.x -= 0.1f;
	}
	if (GET_KEY_MANAGER()->isStayKeyDown(VK_DOWN)) {
		m_teapotPos.x += 0.1f;
	}
}

void MirrorScene::Render()
{
	m_pD3DDevice->Clear(0, NULL,
		D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	m_pD3DDevice->BeginScene();
	///////////////////////////////////////////////////////////
	//벽 렌더
	D3DXMATRIX matTranslation;
	D3DXMATRIX matRotation;
	D3DXMatrixRotationZ(&matRotation, D3DXToRadian(-90));
	D3DXMatrixTranslation(&matTranslation, -5, 0, 4);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &(matRotation*matTranslation));
	m_pD3DDevice->SetStreamSource(0, m_vertexBufferPlate, 0, sizeof(tagVertexXYZ));
	m_pD3DDevice->SetIndices(m_indexBufferPlate);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	D3DXMatrixRotationZ(&matRotation, D3DXToRadian(-90));
	D3DXMatrixTranslation(&matTranslation, -5, 0, -4);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &(matRotation*matTranslation));
	m_pD3DDevice->SetStreamSource(0, m_vertexBufferPlate, 0, sizeof(tagVertexXYZ));
	m_pD3DDevice->SetIndices(m_indexBufferPlate);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	///////////////////////////////////////////////////////////
	//주전자 렌더
	D3DXMATRIX matWorldTeapot;
	D3DXMatrixTranslation(&matWorldTeapot, m_teapotPos.x, m_teapotPos.y, m_teapotPos.z);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorldTeapot);
	m_pD3DDevice->SetMaterial(&m_materialTeapot);
	m_pMeshTeapot->DrawSubset(0);
	///////////////////////////////////////////////////////////
	//거울 오브젝트 렌더
	D3DXMatrixRotationZ(&matRotation, D3DXToRadian(-90));
	D3DXMatrixTranslation(&matTranslation, -5, 0, 0);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &(matRotation*matTranslation));
	m_pD3DDevice->SetStreamSource(0, m_vertexBufferPlate, 0, sizeof(tagVertexXYZ));
	m_pD3DDevice->SetIndices(m_indexBufferPlate);
	m_pD3DDevice->SetMaterial(&m_materialMirror);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, true);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILREF, 1);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//스텐실 버퍼 0으로 클리어
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, 0);

	//스텐실 버퍼에 거울을 그린다.
	D3DXMatrixRotationZ(&matRotation, D3DXToRadian(-90));
	D3DXMatrixTranslation(&matTranslation, -5, 0, 0);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &(matRotation*matTranslation));
	m_pD3DDevice->SetStreamSource(0, m_vertexBufferPlate, 0, sizeof(tagVertexXYZ));
	m_pD3DDevice->SetIndices(m_indexBufferPlate);
	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 6, 0, 2);

	//반사된 물체를 그리기 전 렌더 스테이트 설정
	m_pD3DDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0, 0);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	//반사 행렬 계산
	D3DXMATRIX matReflect;
	D3DXMatrixReflect(&matReflect, &D3DXPLANE(1, 0, 0, 5));

	//반사된 주전자 렌더
	matWorldTeapot = matWorldTeapot * matReflect;
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorldTeapot);
	m_pD3DDevice->SetMaterial(&m_materialTeapot);
	m_pMeshTeapot->DrawSubset(0);
	///////////////////////////////////////////////////////////

	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, false);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	///////////////////////////////////////////////////////////
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
