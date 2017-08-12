#include "StdAfx.h"
#include "cFrame.h"
#include "cMtlTex.h"

cFrame::cFrame(void)
	: m_pMtlTex(NULL),
	m_pVB(NULL),
	m_pMesh(NULL)
{
	D3DXMatrixIdentity(&mfinal);
	D3DXMatrixIdentity(&localmtl);
	D3DXMatrixInverse(&mfinalInv, 0, &mfinal);
	m_animTimeFrameLength = 0;
	m_animTimeOffset = 0;
	m_animTimeScale = 0;
}

cFrame::~cFrame(void)
{
	SAFE_RELEASE(m_pMtlTex);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pMesh);

	for (size_t i = 0; i < m_vecMaterials.size(); ++i) {
		SAFE_RELEASE(m_vecMaterials[i]);
	}
}

void cFrame::RenderUP()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mfinal);
	if(m_pMtlTex)
	{
		g_pD3DDevice->SetMaterial(&(m_pMtlTex->GetMtl()));
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX));
	}

	for each(auto p in m_vecChild)
	{
		p->RenderUP();
	}
}

void cFrame::Render() {
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mfinal);
	if (m_pMtlTex) {
		g_pD3DDevice->SetMaterial(&(m_pMtlTex->GetMtl()));
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->SetStreamSource(0, m_pVB, 0, sizeof(ST_PNT_VERTEX));
		g_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,
			0, m_vecVertex.size() / 3);
	}
	for each(auto p in m_vecChild){
		p->Render();
	}
}

void cFrame::RenderMesh()
{
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &mfinal);
	if (m_pMesh) {
		for (size_t i = 0; i < m_vecMaterials.size(); ++i) {
			if (m_vecMaterials[i]) {
				g_pD3DDevice->SetTexture(0, m_vecMaterials[i]->GetTexture());
				g_pD3DDevice->SetMaterial(&m_vecMaterials[i]->GetMtl());
				m_pMesh->DrawSubset(i);
			}
		}
	}
	for each(auto p in m_vecChild) {
		p->Render();
	}
}

void cFrame::UpdateVertexBuffer()
{
	SAFE_RELEASE(m_pVB);
	if (m_pVB == NULL) {
		g_pD3DDevice->CreateVertexBuffer(
			m_vecVertex.size() * sizeof(ST_PNT_VERTEX),
			D3DUSAGE_WRITEONLY,
			ST_PNT_VERTEX::FVF,
			D3DPOOL_MANAGED,
			&m_pVB,
			NULL);
	}

	ST_PNT_VERTEX *vertices;
	m_pVB->Lock(0, 0, (void**)&vertices, 0);
	for (size_t i = 0; i < m_vecVertex.size(); ++i) {
		vertices[i] = m_vecVertex[i];
	}
	m_pVB->Unlock();
}

void cFrame::SetMesh(ID3DXMesh * pMesh, std::vector<cMtlTex*> materials) {
	m_pMesh = pMesh;

	std::vector<cMtlTex*> tmp = m_vecMaterials;
	m_vecMaterials = materials;

	for (size_t i = 0; i < m_vecMaterials.size(); ++i) {
		if (m_vecMaterials[i]) {
			m_vecMaterials[i]->AddRef();
		}
	}

	//Deffered release
	for (size_t i = 0; i < tmp.size(); ++i) {
		tmp[i]->Release();
	}
}

void cFrame::SetMesh(ID3DXMesh * pMesh)
{
	m_pMesh = pMesh;
}

void cFrame::AddChild( cFrame* pChild ){
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy(){
	for each(auto p in m_vecChild){	
		p->Destroy();
	}
	delete this;
}

void cFrame::SetTransForm(D3DXMATRIX currentmtl){
	D3DXMatrixIdentity(&localmtl);
	localmtl = currentmtl;
}

void cFrame::SetPosition(float x, float y, float z){
	localmtl._41 = x;
	localmtl._42 = y;
	localmtl._43 = z;
}

void cFrame::Update(DWORD currentTime, D3DXMATRIX* parent){
	//애니메이션 업데이트
	//
	DWORD animTime = currentTime; 
	if (m_animTimeFrameLength>0) {
		animTime = (DWORD)(currentTime*m_animTimeScale) % m_animTimeFrameLength + m_animTimeOffset;
	}
	D3DXVECTOR3 pos;
	interpolateTranslation(animTime, pos);

	D3DXQUATERNION quat;
	interpolateRotation(animTime, quat);

	D3DXMATRIX mpos;
	D3DXMatrixTranslation(&mpos, pos.x, pos.y, pos.z);

	D3DXMATRIX mquat;
	D3DXMatrixRotationQuaternion(&mquat, &quat);
	
	mfinal = mquat*mpos;
	if(parent)
		mfinal = (mfinal*(*parent));
	D3DXMatrixInverse(&mfinalInv, 0, &mfinal);
	for each(auto pChild in m_vecChild) {
		pChild->Update(currentTime, &mfinal);
	}
}

void cFrame::roateLocal(D3DXVECTOR3 deltaAngle)
{
	D3DXMATRIX rotMat;
	D3DXMatrixRotationYawPitchRoll(&rotMat, deltaAngle.y, deltaAngle.x, deltaAngle.z);
	localmtl = rotMat * localmtl;
}

void cFrame::TransformVertices(D3DXMATRIX m)
{
	for (auto& v : m_vecVertex) {
		D3DXVec3TransformCoord(&v.p, &v.p, &m);
		D3DXVec3TransformNormal(&v.n, &v.n, &m);
	}
}

bool cFrame::IsCollideWithRay(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, float & distance)
{
	//광선을 메시의 로컬 좌표계로 변환한다.
	D3DXVec3TransformCoord(&rayPos, &rayPos, &mfinalInv);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &mfinalInv);

	BOOL hitted;
	HRESULT ret = D3DXIntersect(m_pMesh, &rayPos, &rayDir,&hitted, NULL, NULL, NULL,&distance, NULL, NULL);
	assert(ret == D3D_OK);
	return hitted == TRUE;
}

void cFrame::interpolateTranslation(IN DWORD time, OUT D3DXVECTOR3 & xyz)
{
	if (m_keyFramesTranslation.empty()) {
		D3DXVECTOR3 pos;
		pos.x = localmtl._41;
		pos.y = localmtl._42;
		pos.z = localmtl._43;
		xyz = pos;
	}
	else 
	{
		if (time <= m_keyFramesTranslation.front().time) {
			xyz = m_keyFramesTranslation.front().translation;
		}
		else if (time >= m_keyFramesTranslation.back().time) {

			xyz = m_keyFramesTranslation.back().translation;
		}
		else
		{
			int cur;
			for (size_t i = 0; i < m_keyFramesTranslation.size(); ++i) {

				if (time <= m_keyFramesTranslation[i].time) {

					cur = i;
					break;
				}
			}
			int pre = cur - 1;


			float t = (float)(time - (m_keyFramesTranslation[pre].time))
				/ (m_keyFramesTranslation[cur].time - m_keyFramesTranslation[pre].time);
			D3DXVECTOR3 pos;
			D3DXVec3Lerp(&pos, &m_keyFramesTranslation[pre].translation, &m_keyFramesTranslation[cur].translation, t);
			xyz = pos;
		}
	}
}

void cFrame::interpolateRotation(IN DWORD time, OUT D3DXQUATERNION & quat)
{

	if (m_keyFramesRotation.empty()) {
		D3DXQUATERNION rot;
		D3DXQuaternionRotationMatrix(&rot, &localmtl);
		quat = rot;
	}
	else 
	{
		if (time <= m_keyFramesRotation.front().time) {
			quat = m_keyFramesRotation.front().quat;
		}
		else if (time >= m_keyFramesRotation.back().time) {

			quat = m_keyFramesRotation.back().quat;

		}
		else {
			int cur;
			for (size_t i = 0; i < m_keyFramesRotation.size(); ++i) {
				if (time <= m_keyFramesRotation[i].time) {
					cur = i;
					break;
				}
			}
			int pre = cur - 1;

			float t = (float)(time - (m_keyFramesRotation[pre].time))
				/ (m_keyFramesRotation[cur].time - m_keyFramesRotation[pre].time);
			D3DXQUATERNION rot;
			D3DXQuaternionSlerp(&rot, &m_keyFramesRotation[pre].quat, &m_keyFramesRotation[cur].quat, t);
			quat = rot;
		}
	}
}
