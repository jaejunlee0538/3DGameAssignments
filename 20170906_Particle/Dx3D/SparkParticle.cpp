#include "stdafx.h"
#include "SparkParticle.h"
#include "cTimeManager.h"

SparkParticle::SparkParticle()
	:m_pVBParticles(NULL), m_pTexture(NULL)
{

}

SparkParticle::~SparkParticle()
{
	SAFE_RELEASE(m_pVBParticles);
}

void SparkParticle::Init(size_t nMaxParticles, D3DXVECTOR3 pos)
{
	ResetParams();
	m_vec3Position = pos;
	SetMaxNumParticles(nMaxParticles);
}

void SparkParticle::ResetParams()
{
	m_bGravity = false;
	m_vec3Gravity = D3DXVECTOR3(0, -10, 0);

	m_fInitEmitSpeed = 5.0f;

	m_vec4InitColor = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	m_fInitSize = 0.025f;

	m_bDamping = false;
	m_fDampingFactor = 0.0f;

	m_bColorOverTime = false;
	m_vec4ColorLifeTime0 = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vec4ColorLifeTime1 = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	m_bColorBySpeed = false;
	m_vec4ColorForSpeed0 = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vec4ColorForSpeed1 = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_fSpeed0 = 0.0f;
	m_fSpeed1 = 2.0f;

	m_bSizeOverTime = false;
	m_fSizeScaleOverTime0 = 1.0f;
	m_fSizeScaleOverTime1 = 0.0f;

	m_fInitLifeTime = 2.0f;

	m_iDeadParticleIndex = 0;
	m_iActiveParticleIndex = -1;

	m_fEmitDuration = 5.0f;
	m_iNParticlesPerSec = 50;
	//m_iMaxNumParticles는 변경하지 않음
	m_fStartTime = 0.0f;
	m_fEmissionEndTime = 0.0f;
	m_fCurrentTime = 0.0f;

	m_bLoop = false;
	m_bStarted = false;
	m_bPaused = false;
	m_fLastParticleGenTime = g_pTimeManager->GetTime();
}

void SparkParticle::Update()
{
	if (!m_bStarted || m_bPaused)
		return;
	m_fCurrentTime = g_pTimeManager->GetTime();

	float nextGenTime = m_fLastParticleGenTime + 1.0f / m_iNParticlesPerSec;
	float deltaTime = g_pTimeManager->GetDeltaTime();
	
	if (!IsEmissionEnded()) {
		int numNewParticles = m_iNParticlesPerSec * deltaTime;

		if (numNewParticles <= 0) {
			if (nextGenTime <= m_fCurrentTime) {
				numNewParticles = 1;
			}
		}

		if (numNewParticles > GetNumInactive()) {
			//생성하려는 파티클 개수가 파티클을 생성하기 위해 남은 공간보다 적은경우.
			numNewParticles = GetNumInactive();
			//LogDebugMessage("numNewParticles = GetNumInactive()\n", numNewParticles);
		}
		
		//파티클 생성
		if (numNewParticles) {
			assert(m_iDeadParticleIndex != m_iActiveParticleIndex);
			//파티클 생성
			int iEnd = (m_iDeadParticleIndex + numNewParticles) % m_iMaxNumParticles;
			for (int i = m_iDeadParticleIndex; i != iEnd;) {
				m_vecParticleInfos[i].m_fInitLifeTime = m_fInitLifeTime;
				D3DXVECTOR3 emitDir = GetEmitDirection();
				if (m_funcDirectionNoise) {
					emitDir += m_funcDirectionNoise(m_fCurrentTime - m_fStartTime);
				}
				m_vecParticleInfos[i].m_vec3Velocity = emitDir * m_fInitEmitSpeed;
				m_vecParticleInfos[i].m_fInitSize = m_fInitSize;
				if (m_funcInitSizeNoise) {
					m_vecParticleInfos[i].m_fInitSize += m_funcInitSizeNoise();
				}
				m_vecParticles[i].c = D3DXCOLOR(m_vec4InitColor);
				m_vecParticles[i].p = m_vec3Position;
				if (m_funcPositionNoise) {
					m_vecParticles[i].p += m_funcPositionNoise(m_fCurrentTime - m_fStartTime);
				}
				m_vecParticles[i].ps = m_vecParticleInfos[i].m_fInitSize;
				if (++i >= m_iMaxNumParticles)
					i = 0;
			}
			if (IsEmpty()) {
				//첫번째 파티클을 생성할 때 실행됨
				m_iActiveParticleIndex = 0;
			}
			assert(iEnd != m_iActiveParticleIndex);
			m_iDeadParticleIndex = iEnd;
			m_fLastParticleGenTime = m_fCurrentTime;
		}
	}
	else {
		if (GetNumActive() == 0) {
			//생성된 모든 파티클들이 사라짐.
			//파티클 시스템 종료
			Stop();
			if (m_cbFuncEmissionFinished) {
				m_cbFuncEmissionFinished();
			}
			return;
		}
	}

	if (m_iActiveParticleIndex >= 0) {
		for (int i = m_iActiveParticleIndex; i != m_iDeadParticleIndex;) {
			if (m_vecParticleInfos[i].m_fInitLifeTime < 0.0f) {
				//파티클 삭제
				m_iActiveParticleIndex = i + 1;

			}
			else {
				//파티클 업데이트
				//댐핑 적용
				if (m_bDamping) {
					m_vecParticleInfos[i].m_vec3Velocity =
						m_vecParticleInfos[i].m_vec3Velocity +
						m_vecParticleInfos[i].m_vec3Velocity * m_fDampingFactor;
				}

				float phaseOverTime = (m_fInitLifeTime - m_vecParticleInfos[i].m_fInitLifeTime) / m_fInitLifeTime;
				float speed = D3DXVec3Length(&m_vecParticleInfos[i].m_vec3Velocity);
				float phaseOverSpeed = (speed - m_fSpeed0) / (m_fSpeed1 - m_fSpeed0);

				//파티클 색깔 업데이트
				D3DXVECTOR4 color = m_vec4InitColor;
				if (m_bColorOverTime) {
					D3DXVECTOR4 color1;
					D3DXVec4Lerp(&color1, &m_vec4ColorLifeTime0, &m_vec4ColorLifeTime1, phaseOverTime);
					color = D3DXVECTOR4(color.x*color1.x, color.y*color1.y, color.z*color1.z, color.w*color1.w);
				}
				if (m_bColorBySpeed) {
					D3DXVECTOR4 color1;
					D3DXVec4Lerp(&color1, &m_vec4ColorForSpeed0, &m_vec4ColorForSpeed1, phaseOverSpeed);
					color = D3DXVECTOR4(color.x*color1.x, color.y*color1.y, color.z*color1.z, color.w*color1.w);
				}
				m_vecParticles[i].c = D3DXCOLOR(color);

				//파티클 사이즈 업데이트
				if (m_bSizeOverTime) {
					m_vecParticles[i].ps = m_vecParticleInfos[i].m_fInitSize * (m_fSizeScaleOverTime0 + (m_fSizeScaleOverTime1 - m_fSizeScaleOverTime0) * phaseOverTime);
				}

				//중력 업데이트
				if (m_bGravity) {
					m_vecParticleInfos[i].m_vec3Velocity = 
						m_vecParticleInfos[i].m_vec3Velocity + 
						m_vec3Gravity * deltaTime;
				}
				//위치 업데이트
				m_vecParticles[i].p = m_vecParticles[i].p + m_vecParticleInfos[i].m_vec3Velocity * deltaTime;

				//Lifetime 빼기
				m_vecParticleInfos[i].m_fInitLifeTime -= deltaTime;
			}
			if (++i >= m_iMaxNumParticles)
				i = 0;
			if (m_iActiveParticleIndex >= m_iMaxNumParticles) {
				m_iActiveParticleIndex = 0;
			}
		}
		if (m_iActiveParticleIndex == m_iDeadParticleIndex) {
			//모든 파티클들이 소멸되었음
			m_iActiveParticleIndex = -1;
			m_iDeadParticleIndex = 0;
		}
	}
}

void SparkParticle::Render()
{
	if (!m_bStarted)
		return;
	if (HasActiveParticle()) {
		ST_PCPS_VERTEX* pVertex = nullptr;
		m_pVBParticles->Lock(0, 0, (LPVOID*)&pVertex, 0);
		memcpy(pVertex, m_vecParticles.data(), sizeof(ST_PCPS_VERTEX)*m_vecParticles.size());
		m_pVBParticles->Unlock();

		D3DXMATRIX matT;
		D3DXMatrixTranslation(&matT, m_vec3Position.x, m_vec3Position.y, m_vec3Position.z);
		
		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		if (m_pTexture) {
			g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
		}
		g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
		g_pD3DDevice->SetTexture(0, m_pTexture);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matT);
		g_pD3DDevice->SetStreamSource(0, m_pVBParticles, 0, sizeof(ST_PCPS_VERTEX));	
		g_pD3DDevice->SetFVF(ST_PCPS_VERTEX::FVF);
		if (m_iActiveParticleIndex < m_iDeadParticleIndex) {
			g_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,
				m_iActiveParticleIndex,
				m_iDeadParticleIndex - m_iActiveParticleIndex);
		}
		else {
			g_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,
				m_iActiveParticleIndex,
				m_iMaxNumParticles - m_iActiveParticleIndex);
			if (m_iDeadParticleIndex > 0) {
				g_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,
					0,
					m_iDeadParticleIndex);
			}
		}
		g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

		g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	}
}

bool SparkParticle::IsStarted() const
{
	return m_bStarted;
}

void SparkParticle::Start()
{
	m_bStarted = true;
	m_fStartTime = g_pTimeManager->GetTime();
	m_fEmissionEndTime = m_fStartTime + m_fEmitDuration;
}

void SparkParticle::Stop()
{
	m_bStarted = false;
	m_iDeadParticleIndex = 0;
	m_iActiveParticleIndex = -1;
}

bool SparkParticle::IsPaused() const
{
	return m_bPaused;
}

void SparkParticle::Pause()
{
	m_bPaused = true;
}

void SparkParticle::Resume()
{
	m_bPaused = false;
	m_fEmissionEndTime = g_pTimeManager->GetTime() + m_fEmitDuration - (m_fCurrentTime - m_fStartTime);
}

bool SparkParticle::IsEmissionEnded() const
{
	return m_bLoop==false && m_fCurrentTime >= m_fEmissionEndTime;
}

void SparkParticle::SetMaxNumParticles(int maxNumParticles)
{
	m_iMaxNumParticles = maxNumParticles;
	m_vecParticleInfos.resize(maxNumParticles);
	m_vecParticles.resize(maxNumParticles);
	SAFE_RELEASE(m_pVBParticles);
	g_pD3DDevice->CreateVertexBuffer(
		sizeof(ST_PCPS_VERTEX)*maxNumParticles,
		D3DUSAGE_WRITEONLY,
		ST_PCPS_VERTEX::FVF,
		D3DPOOL_MANAGED,
		&m_pVBParticles,
		0);
	if (maxNumParticles < m_iActiveParticleIndex)
		m_iActiveParticleIndex = maxNumParticles;
	if (maxNumParticles < m_iDeadParticleIndex) 
		m_iDeadParticleIndex = maxNumParticles;
}

D3DXVECTOR3 SparkParticle::GetEmitDirection() const
{
	return D3DXVECTOR3(0, 1 ,0);
}