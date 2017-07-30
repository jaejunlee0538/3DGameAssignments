#include "FixedCamera.h"

namespace SGA {
	FixedCamera::FixedCamera()
	{
	}

	FixedCamera::~FixedCamera()
	{
	}

	void FixedCamera::init(int width, int height, float posX, float posY)
	{
		CameraIface::init(width, height, posX, posY);
	}
}