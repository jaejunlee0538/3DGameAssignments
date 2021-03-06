#include "StdAfx.h"
#include "cFrame.h"
#include "cMtlTex.h"

cFrame::cFrame(void)
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&mfinal);
	D3DXMatrixIdentity(&localmtl);
}

cFrame::~cFrame(void)
{
	SAFE_RELEASE(m_pMtlTex);
}

void cFrame::Render()
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
		p->Render();
	}
}

void cFrame::AddChild( cFrame* pChild )
{
	m_vecChild.push_back(pChild);
}

void cFrame::Destroy()
{
	for each(auto p in m_vecChild)
	{
		
		p->Destroy();
	}
	delete this;
}

void cFrame::setTransForm(D3DXMATRIX currentmtl)
{

	D3DXMatrixIdentity(&localmtl);
	localmtl = currentmtl;
	
}

void cFrame::SetPosition(float x, float y, float z)
{
	localmtl._41 = x;
	localmtl._42 = y;
	localmtl._43 = z;
}


void cFrame::Update(DWORD currentTime, D3DXMATRIX* parent)
{
	//애니메이션 업데이트
	//
	D3DXVECTOR3 pos;
	interpolateTranslation(currentTime, pos);

	D3DXQUATERNION quat;
	interpolateRotation(currentTime, quat);

	D3DXMATRIX mpos;
	D3DXMatrixTranslation(&mpos, pos.x, pos.y, pos.z);

	D3DXMATRIX mquat;
	D3DXMatrixRotationQuaternion(&mquat, &quat);
	
	mfinal = mquat*mpos;
	if(parent)
		mfinal = (mfinal*(*parent));

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

void cFrame::ScaleVertices(float scale)
{
	for (auto& v : m_vecVertex) {
		v.p *= scale;
	}
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
