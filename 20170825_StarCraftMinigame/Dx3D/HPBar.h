#pragma once
class HPBar
{
public:
	HPBar();
	~HPBar();

	void Init(float length, float height, float maxHP);
	void UpdatePosition(D3DXVECTOR3 position);
	void UpdateCameraPosition(D3DXVECTOR3 position);
	void UpdateCurrentHP(float currentHP);

	void Render();
private:
	D3DXMATRIX GetBkgBarMatrix() const;
	D3DXMATRIX GetHPBarMatrix() const;
private:
	D3DXVECTOR3 m_cameraPosition;

	D3DXQUATERNION m_quaternion;
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_scales;

	float m_maxHP;
	float m_currentHP;
	LPDIRECT3DVERTEXBUFFER9 m_pVBBkg;
	LPDIRECT3DVERTEXBUFFER9 m_pVBHP;
};

