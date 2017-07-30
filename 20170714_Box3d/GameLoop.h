#pragma once
#include <Windows.h>
#include <GameNode.h>

class GameLoop : public SGA::GameNode
{
public:
	GameLoop();

	~GameLoop();

	virtual HRESULT init();
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	void controlMessage(int message);

private:
};
