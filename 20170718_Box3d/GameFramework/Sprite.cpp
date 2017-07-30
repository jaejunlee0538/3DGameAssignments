#include "Sprite.h"
namespace SGA {
	Sprite::Sprite(const BitmapImage* imgSrc,
		int clipPosX, int clipPosY, int clipWidth, int clipHeight,
		float px, float py)
		:_imageSource(imgSrc)
	{
		_clip.clipPosX = clipPosX;
		_clip.clipPosY = clipPosY;
		_clip.h = clipHeight;
		_clip.w = clipWidth;
		_clip.pivotX = clipWidth * px;
		_clip.pivotY = clipHeight * py;
		_clipXFactor = 1.0f;
		_clipYFactor = 1.0f;
		setFlipX(false);
		if (imgSrc->getTransparentMode() == BitmapImage::TRANSPARENT_MODE_ALPHA) {
			enableAlpha(true);
		}
		else {
			enableAlpha(false);
		}
		setAlpha(255);
		setAngle(0.0f);
	}

	Sprite::Sprite(const Sprite & other)
	{
		_clip = other._clip;
		_imageSource = other._imageSource;
		_flipX = other._flipX;
		_clipXFactor = other._clipXFactor;
		_clipYFactor = other._clipYFactor;
		_alpha = other._alpha;
		_isAlphaEnabled = other._isAlphaEnabled;
		_angleRad = other._angleRad;
	}

	Sprite::~Sprite()
	{

	}

	Sprite * Sprite::clone() const
	{
		return new Sprite(*this);
	}

	void Sprite::render(HDC hdc, int drawPosX, int drawPosY) const
	{
		if (_clipXFactor == 1.0f && _clipYFactor == 1.0f) {
			entireRender(hdc, drawPosX, drawPosY);
		}
		else {
			clipRender(hdc, drawPosX, drawPosY, _clipXFactor, _clipYFactor);
		}
	}

	void Sprite::reset()
	{
		setFlipX(false);
		setClipXFactor(1.0f);
		setClipYFactor(1.0f);
	}

	void Sprite::setAngle(float angleRad)
	{
		_angleRad = angleRad;
	}

	//true로 설정하면 스프라이트가 피벗 포인트를 기준으로 좌우 반전되어 출력된다.

	void Sprite::setFlipX(bool flipped) {
		_flipX = flipped;
	}

	//피벗 포인트를 기준으로 좌우 동일한 비율로 clip 폭을 조절한다.
	//유효 범위 : [0, 1.0]

	void Sprite::enableAlpha(bool enable)
	{
		_isAlphaEnabled = enable;
	}

	void Sprite::setAlpha(BYTE alpha)
	{
		_alpha = alpha;
	}

	void Sprite::setClipXFactor(float factor) {
		if (factor < 0.0f) factor = 0.0f;
		if (factor > 1.0f)factor = 1.0f;
		_clipXFactor = factor;
	}

	//피벗 포인트를 기준으로 좌우 동일한 비율로 clip 높이을 조절한다.
	//유효 범위 : [0, 1.0]

	void Sprite::setClipYFactor(float factor) {
		if (factor < 0.0f) factor = 0.0f;
		if (factor > 1.0f)factor = 1.0f;
		_clipYFactor = factor;
	}

	//ClipXFactor가 반영된 출력 스프라이트의 폭을 반환한다.

	int Sprite::getWidth() const {
		return _clip.w*_clipXFactor;
	}

	//ClipYFactor가 반영된 출력 스프라이트의 높이를 반환한다.

	int Sprite::getHeight() const {
		return _clip.h*_clipYFactor;
	}

	int Sprite::getLeft() const {
		if (_flipX) {
			return -(_clip.w -_clip.pivotX)*_clipXFactor;
		}
		else {
			return -_clip.pivotX*_clipXFactor;
		}
	}
	int Sprite::getRight() const {
		return getWidth() + getLeft();
	}

	int Sprite::getTop() const {
		return -_clip.pivotY*_clipYFactor;
	}

	int Sprite::getBottom() const {
		return getHeight() + getTop();
	}

	void Sprite::entireRender(HDC hdc, int drawPosX, int drawPosY) const
	{
		int pivotX, pivotY;
		int w, h;
		int x, y;
		pivotX = _flipX ? (_clip.w - _clip.pivotX) : _clip.pivotX;
		pivotY = _clip.pivotY;
		w = _flipX ? -_clip.w : _clip.w;
		h = _clip.h;
		x = drawPosX - pivotX;
		y = drawPosY - pivotY;

		if (_angleRad == 0.0f) {
			if (_isAlphaEnabled) {
				_imageSource->renderAlpha(hdc, x, y, _clip.clipPosX, _clip.clipPosY, w, h, _alpha);
			}
			else {
				_imageSource->render(hdc, x, y, _clip.clipPosX, _clip.clipPosY, w, h);
			}
		}
		else {
			_imageSource->renderRot(hdc, x, y, _clip.clipPosX, _clip.clipPosY, w, h, _angleRad, pivotX, pivotY);
		}
	}

	void Sprite::clipRender(HDC hdc, int drawPosX, int drawPosY, float srcWidthClip, float srcHeightClip) const
	{
		//TODO:
		if (_flipX == false) {
			int pivotX = _clip.pivotX * srcWidthClip;
			int pivotY = _clip.pivotY * srcHeightClip;
			_imageSource->render(hdc,
				drawPosX - pivotX,
				drawPosY - pivotY,
				_clip.clipPosX + (_clip.pivotX - pivotX), _clip.clipPosY + (_clip.pivotY - pivotY),
				_clip.w * srcWidthClip, _clip.h * srcHeightClip);
		}
		else {
			int srcW = _clip.w * srcWidthClip;
			int srcH = _clip.h * srcHeightClip;
			int pivotX = _clip.pivotX * srcWidthClip;
			int pivotY = _clip.pivotY * srcHeightClip;
			_imageSource->render(hdc,
				drawPosX - (srcW - pivotX),
				drawPosY - pivotY,
				_clip.clipPosX + (_clip.pivotX - pivotX), _clip.clipPosY + (_clip.pivotY - pivotY),
				-srcW, srcH);
		}
	}
	/*
	void Sprite::render(HDC hdc, int drawPosX, int drawPosY, float scale) const
	{
		_imageSource->render(hdc,
			drawPosX - _clip.pivotX*scale,
			drawPosY - _clip.pivotY*scale,
			_clip.w*scale, _clip.h*scale,
			_clip.clipPosX, _clip.clipPosY,
			_clip.w, _clip.h);
	}

	void Sprite::renderRot(HDC hdc, int drawPosX, int drawPosY, float angle) const
	{
		POINT pivot;
		pivot.x = _clip.pivotX;
		pivot.y = _clip.pivotY;
		_imageSource->renderRot(hdc, drawPosX - _clip.pivotX, drawPosY - _clip.pivotY,
			_clip.clipPosX, _clip.clipPosY,
			_clip.w, _clip.h, angle, &pivot);
	}*/

}