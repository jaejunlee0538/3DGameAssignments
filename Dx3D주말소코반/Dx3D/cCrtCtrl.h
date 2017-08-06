#pragma once
#include "LinearInterpolater.h"
class cCrtCtrl
{
private:
	enum LookDirections {
		DIR_UP = 0,
		DIR_RIGHT,
		DIR_DOWN,
		DIR_LEFT,

		NUM_DIRS
	};

	D3DXVECTOR3					m_vDir;
	D3DXVECTOR3					m_vPos;
	bool						m_bMoving;
	LookDirections				m_lookDir;
	LinearInterpolator			m_lerp;
protected:
	SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorld, WorldTM);

public:
	cCrtCtrl(void);
	~cCrtCtrl(void);

	void Update();
	D3DXVECTOR3* GetPosition();
	float GetAngle() const;
	void SetPosition(D3DXVECTOR3 pos) { m_vPos = pos; }

	LookDirections GetLookDirection() const { return m_lookDir; }
	void ClearMoving() { m_bMoving = false; }
	bool IsMoving() const {
		return m_bMoving;
	}
private:
	void TurnLeft();
	void TurnRight();
};

