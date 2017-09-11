#pragma once
#include "GameObject.h"
class BoxObject :
	public GameObject
{
public:
	BoxObject();
	~BoxObject();
	void init(D3DXVECTOR3 scale);
	virtual void render() const;
	virtual void update(bool isDirty);
private:
	D3DMATERIAL9 m_mtl;
	LPD3DXMESH m_pMesh;
};

