#include "Slider.h"
#include <SpriteManager.h>
#include <KeyManager.h>
#include <CollisionCheckFunc.h>
#include <CommonMacroHeader.h>
namespace SGA {
	Slider::Slider()
		:_railSprite(NULL), _sliderSprite(NULL)
	{
	}

	Slider::~Slider()
	{
	}

	HRESULT Slider::init(float minVal, float maxVal, float initVal, SGA::tstring railSprite, SGA::tstring sliderSprite)
	{
		UIBase::init();
		_minVal = minVal;
		_maxVal = maxVal;
		_currentVal = initVal;

		_railSprite = GET_SPRITE_MANAGER()->findSprite(railSprite);
		_sliderSprite = GET_SPRITE_MANAGER()->findSprite(sliderSprite);
		_xBegin = -_railSprite->getWidth() / 2;
		_xRailLength = _railSprite->getWidth();
		return S_OK;
	}

	void Slider::release()
	{
	}

	void Slider::update()
	{
		if (_controlling == false && SGA::isPointInRect(getSliderRECT(), GET_KEY_MANAGER()->getCurrentMousePos())) {
			if (GET_KEY_MANAGER()->isOnceKeyDown(VK_LBUTTON)) {
				_controlling = true;
				_ptPrev = _pivot = GET_KEY_MANAGER()->getCurrentMousePos();
				_xBeforePivot = getSliderXPos();
			}
		}

		if (_controlling) {
			if (GET_KEY_MANAGER()->isStayKeyDown(VK_LBUTTON)) {
				POINT pt = GET_KEY_MANAGER()->getCurrentMousePos();
				if (_ptPrev.x - pt.x != 0) {
					int dx;
					dx = pt.x - _pivot.x;
					setSliderXPos(_xBeforePivot + dx);
				}
				_ptPrev = pt;
			}
			else {
				_controlling = false;
			}
		}
	}

	void Slider::render()
	{
		UIBase::renderOnLayer(_railSprite, _x, _y);
		UIBase::renderOnLayer(_sliderSprite, _x + getSliderXPos(), _y);
		if (_caption.empty() == false) {
			UIBase::textoutOnLayer(_caption, _x - getWidth() / 2 - 5, _y - 10, TA_RIGHT, RGB(255, 255, 255));
		}
	}

	int Slider::getWidth() const
	{
		return _railSprite->getWidth();
	}

	int Slider::getHeight() const
	{
		return _railSprite->getHeight();
	}

	void Slider::setCaption(SGA::tstring caption)
	{
		_caption = caption;
	}

	void Slider::setSliderMoveCallback(CALLBACK_FUNC_PARAM cb, void * data)
	{
		_sliderMoveCallback.set(cb, data);
	}

	void Slider::setCurrentValue(float val)
	{
		_currentVal = val;
	}

	float Slider::getCurrentValue() const
	{
		return _currentVal;
	}

	void Slider::setSliderXPos(float xPos)
	{
		if (xPos < _xBegin) {
			xPos = _xBegin;
		}
		if (xPos > _xBegin + _xRailLength) {
			xPos = _xBegin + _xRailLength;
		}
		float val = _minVal + (_maxVal - _minVal)*(xPos - _xBegin) / _xRailLength;
		setCurrentValue(val);
		_sliderMoveCallback.call(getCurrentValue());
	}

	float Slider::getSliderXPos() const
	{
		return _xBegin + _xRailLength*(_currentVal - _minVal) / (_maxVal - _minVal);
	}

	RECT Slider::getSliderRECT() const
	{
		return  SGA::makeRectCenter<RECT, int>(getPositionX() + getSliderXPos(),
			getPositionY(), _sliderSprite->getWidth(), _sliderSprite->getHeight());
	}
}