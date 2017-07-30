#pragma once
#include "CameraIface.h"
namespace SGA {
	class FixedCamera :public CameraIface
	{
	public:
		FixedCamera();
		virtual ~FixedCamera();
		void init(int width, int height, float posX, float posY);
	};
}
