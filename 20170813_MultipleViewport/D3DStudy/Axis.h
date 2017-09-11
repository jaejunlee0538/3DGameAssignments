#pragma once
#include "GameObject.h"
namespace SGA {
	class Axis : public GameObject
	{
	public:
		Axis(float scale=1.0f);
		~Axis();
		void init(float scale);
		void render(LPDIRECT3DDEVICE9 device) const override;

	private:
		VertexPosDiff _pts[6];
	};
}
