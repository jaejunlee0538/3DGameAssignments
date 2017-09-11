#pragma once
#include "iMap.h"

class cObjMap : public iMap
{
private:
	vector<D3DXVECTOR3> m_vecSurface;

public:
	cObjMap(void);
	~cObjMap(void);

	virtual void Load(char* szFullPath, D3DXMATRIX* pmat = NULL);

	// iMap override
	virtual bool GetHeight(IN float x, OUT float& y, IN float z) override;
};

