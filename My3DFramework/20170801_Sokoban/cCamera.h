#pragma once
class cCamera
{
public:
	cCamera();
	~cCamera();

	void Setup(float yFoV, float aspect, float zNear, float zFar);

	void SetAngle(float angleX, float angleY, float angleZ);
	void SetDistanceToTarget(float dist);
	void SetLookTargetPosition(D3DXVECTOR3 targetPos);
	void Update(SGA::UserInputData& input);
	void CalculateLookAtMatrix();
	const D3DXMATRIX& GetLookAtMatrix() const { return m_matLookAt; }
	const D3DXMATRIX& GetProjectionMatrix() const { return m_matProj; }
private:
	D3DXMATRIX m_matLookAt;
	D3DXMATRIX m_matProj;
	float m_distToTarget;
	D3DXVECTOR3 m_targetPos;
	D3DXVECTOR3 m_camPos;
	D3DXVECTOR3 m_camAngles;
};

