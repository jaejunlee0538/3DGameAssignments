#pragma once
#include <functional>

struct Particle {
	float			m_fInitLifeTime;
	float			m_fInitSize;
	D3DXVECTOR3		m_vec3Velocity;
};

class SparkParticle
{
public:
	SparkParticle();
	~SparkParticle();
	
	void Init(size_t nMaxParticles, D3DXVECTOR3 pos);
	void ResetParams();
	void Update();
	void Render();
	bool IsStarted() const;
	void Start();
	void Stop();
	bool IsPaused() const;
	void Pause();
	void Resume();
	bool IsEmissionEnded() const;
	bool IsFinished() const;

	void SetMaxNumParticles(int maxNumParticles);
	
	bool IsFull() const {
		return (m_iDeadParticleIndex + 1) % m_iMaxNumParticles == m_iActiveParticleIndex;
	}

	bool IsEmpty() const {
		return m_iActiveParticleIndex < 0;
	}

	bool HasActiveParticle() const {
		return m_iActiveParticleIndex >= 0;
	}

	int GetNumActive() const {
		if (m_iActiveParticleIndex < 0)
			return 0;
		assert(m_iActiveParticleIndex != m_iDeadParticleIndex);
		if (m_iActiveParticleIndex < m_iDeadParticleIndex) {
			return m_iDeadParticleIndex - m_iActiveParticleIndex;
		}
		else {
			return (m_iMaxNumParticles - m_iActiveParticleIndex) + m_iDeadParticleIndex + 1;
		}
	}

	int GetNumInactive() const {
		//assert(m_iActiveParticleIndex != m_iDeadParticleIndex);
		return m_iMaxNumParticles - GetNumActive() - 1;
	}

	D3DXVECTOR3 GetEmitDirection() const;

	D3DXVECTOR3	 m_vec3Position;

	//파티클마다 작용하는 중력
	bool		 m_bGravity;
	D3DXVECTOR3	 m_vec3Gravity;
	
	//초기 파티클 속력
	float		 m_fInitEmitSpeed; 
	D3DXVECTOR4	 m_vec4InitColor;
	float		 m_fInitSize;

	//댐핑
	bool		m_bDamping;
	float		m_fDampingFactor;
	
	//
	bool		 m_bColorOverTime;
	D3DXVECTOR4	 m_vec4ColorLifeTime0;
	D3DXVECTOR4	 m_vec4ColorLifeTime1;

	bool		 m_bColorBySpeed;
	D3DXVECTOR4	 m_vec4ColorForSpeed0;
	D3DXVECTOR4	 m_vec4ColorForSpeed1;
	float		 m_fSpeed0;
	float		 m_fSpeed1;
	
	bool	m_bSizeOverTime;
	float	m_fSizeScaleOverTime0;
	float	m_fSizeScaleOverTime1;

	float	m_fInitLifeTime;
		
	int		m_iDeadParticleIndex;
	int		m_iActiveParticleIndex;

	float	m_fEmitDuration;
	float	m_fStartTime;
	float	m_fEmissionEndTime;
	float	m_fCurrentTime;

	int		m_iNParticlesPerSec;
	int		m_iMaxNumParticles;

	bool	m_bLoop;
	bool	m_bStarted;
	bool	m_bPaused;

	float	m_fLastParticleGenTime;

	LPDIRECT3DTEXTURE9	m_pTexture;

	std::function<float()>		m_funcInitSizeNoise;
	std::function<D3DXVECTOR3(float time)>	m_funcPositionNoise;
	std::function<D3DXVECTOR3(float time)>	m_funcDirectionNoise;

	std::function<void(void)>				m_cbFuncEmissionFinished;
	std::vector<Particle>			m_vecParticleInfos;
	std::vector<ST_PCPS_VERTEX>		m_vecParticles;
	LPDIRECT3DVERTEXBUFFER9			m_pVBParticles;
};

