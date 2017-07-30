#pragma once
#include <Windows.h>
#include "BitmapImage.h"
#include "SpriteIface.h"
namespace SGA {
	/*
	��Ʋ�� �̹��� _imageSource���� ��������Ʈ ������ ��Ÿ���� Ŭ����.
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

		//��������Ʈ ���� ������
		//flip, clipFactor�� default������ �ʱ�ȭ�ȴ�.
		Sprite(const Sprite& other);

		virtual ~Sprite();

		virtual Sprite* clone() const;

		//��������Ʈ�� pivotPoint�� drawPos�� ��ġ�ϵ��� �׸���.
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
		//��������Ʈ ��ü�� drawPosX, drawPosY�� �׸���.
		void entireRender(HDC hdc, int drawPosX, int drawPosY) const;
		//���� ��������Ʈ�� Ŭ���� �簢���� srcWidthClip/srcHeightClip�� ������ ũ�� �����Ͽ� ����Ѵ�.
		//�� �����ϰ��� [0, 1.0] ������ �־�� �Ѵ�.
		void clipRender(HDC hdc, int drawPosX, int drawPosY, float srcWidthClip, float srcHeightClip) const;
	private:
		//��Ʋ�� �̹����������� ��ġ�� ũ��
		tagClippingInfo _clip;
		const BitmapImage* _imageSource;

		bool _flipX;
		float _clipXFactor, _clipYFactor;
		BYTE _alpha;
		bool _isAlphaEnabled;
		float _angleRad;
	};
}