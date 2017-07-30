#pragma once
#include <GameNode.h>
#include <vector>
#include "Math3D.h"
class PlayScene :public SGA::GameNode
{
public:
	PlayScene();
	virtual ~PlayScene();
	HRESULT init(void);
	void update(void);
	void release(void);
	void render(void);
private:
	HPEN m_Pen;
	std::vector<cVector3> m_BoxPoints;
	cVector3 m_LookDir;
	cVector3 m_BoxPos;
	cVector3 m_CamPos;
};

