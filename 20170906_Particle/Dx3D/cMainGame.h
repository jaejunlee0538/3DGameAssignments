#pragma once

class cCamera;
class SparkParticle;
class cMainGame
{
private:
	cCamera*				m_pCamera;
	vector<ST_PC_VERTEX>	m_vecVertex;
	SparkParticle*			m_pSparkParticle;
	SparkParticle*			m_pFireParticle;
	SparkParticle*			m_pSmokeParticle;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void SetLight();

private:
	void InitSimpleParticle();
	void InitFire();
	void InitSmoke();
};

