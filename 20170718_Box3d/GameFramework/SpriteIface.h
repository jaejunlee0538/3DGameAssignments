#pragma once
#include <Windows.h>
namespace SGA {
	class SpriteIface {
	public:
		virtual ~SpriteIface(){}
		virtual void release(){}
		virtual void update(){}
		virtual void render(HDC hdc, int drawPosX, int drawPosY) const= 0;

		virtual SpriteIface* clone() const = 0;
		virtual void reset(){}
		//true로 설정하면 스프라이트가 피벗 포인트를 기준으로 좌우 반전되어 출력된다.
		virtual void setFlipX(bool flipped) = 0;
		virtual bool getFlipX() const = 0;
		//피벗 포인트를 기준으로 좌우 동일한 비율로 clip 폭을 조절한다.
		//유효 범위 : [0, 1.0]
		virtual void setClipXFactor(float factor) = 0;
		//피벗 포인트를 기준으로 좌우 동일한 비율로 clip 높이을 조절한다.
		//유효 범위 : [0, 1.0]
		virtual void setClipYFactor(float factor) = 0;
		//ClipXFactor가 반영된 출력 스프라이트의 폭을 반환한다.
		virtual int getWidth() const = 0;
		//ClipYFactor가 반영된 출력 스프라이트의 높이를 반환한다.
		virtual int getHeight() const = 0;
		//스프라이트의 가장 왼쪽 픽셀 좌표를 반환한다(피벗 포인트 기준)
		virtual int getLeft() const = 0;
		//스프라이트의 가장 오른쪽 픽셀 좌표를 반환한다(피벗 포인트 기준)
		virtual int getRight() const = 0;
		//스프라이트의 가장 위 픽셀 좌표를 반환한다(피벗 포인트 기준)
		virtual int getTop() const = 0;
		//스프라이트의 가장 아래 픽셀 좌표를 반환한다(피벗 포인트 기준)
		virtual int getBottom() const = 0;

	/*	virtual void render(HDC hdc, int drawPosX, int drawPosY, int w, int h) const= 0;
		virtual void render(HDC hdc, int drawPosX, int drawPosY, float scale) const =0;
		virtual void renderRot(HDC hdc, int drawPosX, int drawPosY, float angle) const = 0;*/
		//virtual void render(HDC hdc, int drawPosX, int drawPosY) const;
		//virtual void render(HDC hdc, int drawPosX, int drawPosY, int w, int h) const;
		///*
		//스프라이트를 scale배하여 drawPosX,drawPosY에 출력한다.
		//*/
		//virtual void render(HDC hdc, int drawPosX, int drawPosY, float scale) const;
		//virtual void renderRot(HDC hdc, int drawPosX, int drawPosY, float angle) const;
	};
}