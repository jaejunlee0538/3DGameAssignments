#pragma once

class cPyramid;

class cLerp : public cObject
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	cPyramid*				m_pPyramid;

	cLerp(void);
public:
	
	~cLerp(void);

	static cLerp* Create()
	{
		return (cLerp*)(new cLerp)->AutoRelease();
	}

	void Setup();
	void Update();
	void Render();
};

