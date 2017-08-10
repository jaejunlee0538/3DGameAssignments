#pragma once
class cFrame;
class cPlayer
{
private:
	cFrame* m_pRunning;
	cFrame* m_pIdle;

	DWORD			m_prevTime;
	D3DXMATRIX		m_matWorld;
	D3DXMATRIX		m_playerOffset;
	D3DXQUATERNION	m_rotation;
	
	float			m_lastHeightOfCollisionPoint;

	bool			m_bRunning;
	bool			m_bTurning;
	bool			m_bJumping;

	bool			m_bGravityOn;

	D3DXVECTOR3		m_prevPosition;

	SYNTHESIZE(float, m_rayHeight, RayHeight);
	SYNTHESIZE(D3DXVECTOR3, m_position, Position);
	SYNTHESIZE(D3DXVECTOR3, m_angularVelocity, AngularVelocity);
	SYNTHESIZE(D3DXVECTOR3, m_vecVelocity, Velocity);
public:
	cPlayer();
	~cPlayer();

	void Init();
	void Update(DWORD time);
	void Render();
	void RenderUP();
	void RenderMesh();

	void UpdateWorldMatrix();

	void SetWalkingSpeed(float velocity);

	void SetVelocityLocal(D3DXVECTOR3 velocityLocal);

	void Translate(D3DXVECTOR3 displace);
	void RotateLocal(D3DXVECTOR3 displaceAngle);

	void ResolveCollisionByLiftingUp(std::vector<cFrame*>& objects);
	void ResolveCollision(std::vector<cFrame*>& objects);
private:
	float GetCollisionDistance(std::vector<cFrame*>& objects, float center);
	void EnableGravity(bool enable);
	void Jump(float jumpSpeed);
private:
	float ShootCollisionRay(D3DXVECTOR3 rayPos, D3DXVECTOR3 rayDir, std::vector<cFrame*>& objects);
};

