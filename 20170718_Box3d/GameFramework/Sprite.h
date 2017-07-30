#pragma once
#include <Windows.h>
#include "BitmapImage.h"
#include "SpriteIface.h"
namespace SGA {
	/*
	아틀라스 이미지 _imageSource내의 스프라이트 조각을 나타내는 클래스.
	*/
	class Sprite : public SpriteIface
	{
		struct tagClippingInfo {
			int clipPosX, clipPosY;
			int pivotX, pivotY;
			int w, h;
		};
	public:
		Sprite(const BitmapImage* imgSrc,
			int clipPosX, int clipPosY,
			int clipWidth, int clipHeight,
			float px, float py);

		//스프라이트 복사 생성자
		//flip, clipFactor는 default값으로 초기화된다.
		Sprite(const Sprite& other);

		virtual ~Sprite();

		virtual Sprite* clone() const;

		//스프라이트의 pivotPoint가 drawPos와 일치하도록 그린다.
		virtual void render(HDC hdc, int drawPosX, int drawPosY) const;

		virtual void reset();

		virtual void setAngle(float angleRad);

		virtual void setFlipX(bool flipped);
		virtual bool getFlipX() const { return _flipX; }

		virtual void enableAlpha(bool enable);
		virtual void setAlpha(BYTE alpha);
		virtual void setClipXFactor(float factor);
		virtual void setClipYFactor(float factor);

		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual int getLeft() const;
		virtual int getRight() const;
		virtual int getTop() const;
		virtual int getBottom() const;
	private:
		//스프라이트 전체를 drawPosX, drawPosY에 그린다.
		void entireRender(HDC hdc, int drawPosX, int drawPosY) const;
		//원본 스프라이트의 클리핑 사각형을 srcWidthClip/srcHeightClip의 비율로 크기 변경하여 출력한다.
		//두 스케일값은 [0, 1.0] 범위에 있어야 한다.
		void clipRender(HDC hdc, int drawPosX, int drawPosY, float srcWidthClip, float srcHeightClip) const;
	private:
		//아틀라스 이미지내에서의 위치와 크기
		tagClippingInfo _clip;
		const BitmapImage* _imageSource;

		bool _flipX;
		float _clipXFactor, _clipYFactor;
		BYTE _alpha;
		bool _isAlphaEnabled;
		float _angleRad;
	};
}