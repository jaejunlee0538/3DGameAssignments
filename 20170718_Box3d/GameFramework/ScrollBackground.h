#pragma once
#include "BackgroundIface.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <tstring.h>
namespace SGA {
	class CameraIface;
	class Sprite;
	class CameraIface;
}
namespace SGA {
	class ScrollBackground :
		public BackgroundIface
	{
	public:
		ScrollBackground();
		virtual ~ScrollBackground();
		virtual void init(tstring spriteName, float left, float top);
		/*
		left, top : 배경 이미지의 top-left를 world의 어떤 좌표에 맞추어야 할지 결정한다.
		*/
		virtual void init(tstring spritePrefix, int nX, int nY, float left, float top);
		virtual void update();
		virtual void render(CameraIface* camera, float priority);
		int getTotalWidth()const { return _w*_nX; }
		int getTotalHeight() const { return _h*_nY; }

		void setSpeedX(float vx);
		void setSpeedY(float vy);
		void moveX(float dx);
		void moveY(float dy);

		void setMaxLoopXCount(int negDir, int posDir);
		void setMaxLoopYCount(int negDir, int posDir);
	private:
		int limitXIdx(int idx);
		int limitYIdx(int idx);
	protected:
		POINTFLOAT _topLeft;
		std::vector<std::vector<const SGA::Sprite*>> _tiles;
		int _nX, _nY;
		int _w, _h;
		float _vx, _vy;
		int _maxLoopCountX[2], _maxLoopCountY[2];
	};
}