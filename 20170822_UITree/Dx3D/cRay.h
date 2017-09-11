#pragma once
class cRay
{
private:
	D3DXVECTOR3 m_vRayOrg;
	D3DXVECTOR3 m_vRayDir;

	enum eRaySpace
	{
		E_NONE,
		E_VIEW,
		E_WORLD,
	};
	
	eRaySpace m_eRaySpace;

public:
	cRay(void);
	~cRay(void);

	static cRay RayAtViewSpace(int nScreenX, int nScreenY);
	static cRay RayAtWorldSpace(int nScreenX, int nScreenY);
	bool IsPicked(const ST_SPHERE* pSphere);
};

