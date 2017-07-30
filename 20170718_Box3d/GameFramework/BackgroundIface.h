#pragma once
namespace SGA {
	class CameraIface;
	class BackgroundIface
	{
	public:
		BackgroundIface();
		virtual ~BackgroundIface();
		virtual void render(CameraIface * camera, float priority) = 0;
	};
}