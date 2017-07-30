#pragma once
#include <GameNode.h>
#include <vector>
#include "Math3D.h"
#include "Box.h"
#include "Camera3D.h"
#include "Grid.h"
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
	Box _box;
	Grid _grid;
	Camera3D _camera;
};

