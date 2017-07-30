#pragma once
class cCamera
{
public:
	cCamera();
	~cCamera();

	void SetAngle(float angleX, float angleY, float angleZ);
	void SetLookTargetPosition(D3DXVECTOR3 targetPos);
	void Update(SGA::UserInputData& input);
	void CalculateLookAtMatrix();
	const D3DXMATRIX& GetLookAtMatrix() const { return m_matLookAt; }
private:
	D3DXMATRIX m_matLookAt;
	float m_distToTarget;
	D3DXVECTOR3 m_targetPos;
	D3DXVECTOR3 m_camPos;
	D3DXVECTOR3 m_camAngles;
};

