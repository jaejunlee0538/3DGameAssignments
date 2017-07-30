#include "Button.h"
#include <SpriteManager.h>
#include <string>
#include <KeyManager.h>
#include <ClockManager.h>
#include <CollisionCheckFunc.h>
#include <CommonMacroHeader.h>
#include <CameraManager.h>
namespace SGA {
	Button::Button()
	{

	}

	Button::~Button()
	{
	}

	HRESULT Button::init(float posX, float posY, ShapeTypes shape, SGA::tstring spriteShared)
	{
		return init(posX, posY, shape, spriteShared, spriteShared, spriteShared, spriteShared);
	}

	HRESULT Button::init(float posX, float posY, ShapeTypes shape, SGA::tstring spriteIDLE, SGA::tstring spriteHover, SGA::tstring spriteClick, SGA::tstring spriteDisabled)
	{
		setPosition(posX, posY);
		_shape = shape;
		_spriteIdle = GET_SPRITE_MANAGER()->findSprite(spriteIDLE);
		_spriteHover = GET_SPRITE_MANAGER()->findSprite(spriteHover);
		_spriteClick = GET_SPRITE_MANAGER()->findSprite(spriteClick);
		_spriteDisabled = GET_SPRITE_MANAGER()->findSprite(spriteDisabled);
		_state = BUTTON_STATE_IDLE;
		return S_OK;
	}

	void Button::setPressingBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbPressBegin.set(cbFunc, userData);
	}

	void Button::setPressingCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbPressing.set(cbFunc, userData);
	}

	void Button::setPressingEndCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbPressEnd.set(cbFunc, userData);
	}

	void Button::setHoverBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbHoverBegin.set(cbFunc, userData);
	}

	void Button::setHoveringCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbHovering.set(cbFunc, userData);
	}

	void Button::setHoverEndCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbHoverEnd.set(cbFunc, userData);
	}

	void Button::enableButton(bool enable)
	{
		if (enable == false) {
			_state = BUTTON_STATE_DISABLED;
		}
		else {
			_state = BUTTON_STATE_IDLE;
		}
	}

	void Button::release()
	{
	}

	void Button::render()
	{
		const SGA::Sprite* sprite = NULL;
		switch (_state) {
		case BUTTON_STATE_IDLE:
			sprite = _spriteIdle;
			break;
		case BUTTON_STATE_HOVER:
			sprite = _spriteHover;
			break;
		case BUTTON_STATE_CLICK:
			sprite = _spriteClick;
			break;
		case BUTTON_STATE_DISABLED:
			sprite = _spriteDisabled;
			break;
		}
		renderOnLayer(sprite, _x, _y);
	}

	void Button::render(HDC hdc)
	{
		const SGA::Sprite* sprite = NULL;
		switch (_state) {
		case BUTTON_STATE_IDLE:
			sprite = _spriteIdle;
			break;
		case BUTTON_STATE_HOVER:
			sprite = _spriteHover;
			break;
		case BUTTON_STATE_CLICK:
			sprite = _spriteClick;
			break;
		case BUTTON_STATE_DISABLED:
			sprite = _spriteDisabled;
			break;
		}
		sprite->render(hdc, _x, _y);
	}

	void Button::update()
	{
		switch (_state) {
		case BUTTON_STATE_IDLE:
			if (isMouseInButton()) {
				if (GET_KEY_MANAGER()->isStayKeyUp(VK_LBUTTON)) {//TODO : KEY_MANAGER 의존성 제거
					_state = BUTTON_STATE_HOVER;
					_cbHoverBegin.call();
				}
			}
			break;
		case BUTTON_STATE_HOVER:
			if (isMouseInButton()) {
				if (GET_KEY_MANAGER()->isStayKeyDown(VK_LBUTTON)) {
					_state = BUTTON_STATE_CLICK;
					_cbPressBegin.call();
				}
				else {
					_cbHovering.call();
				}
			}
			else {
				_state = BUTTON_STATE_IDLE;
				_cbHoverEnd.call();
			}
			break;
		case BUTTON_STATE_CLICK:
			if (GET_KEY_MANAGER()->isStayKeyUp(VK_LBUTTON)) {
				//Clicked
				if (isMouseInButton()) {
					_state = BUTTON_STATE_HOVER;
				}
				else {
					_state = BUTTON_STATE_IDLE;
				}
				_cbPressEnd.call();
			}
			else {
				//Clicking
				_cbPressing.call();
			}
			break;
		case BUTTON_STATE_DISABLED:
			break;
		}
	}

	int Button::getWidth() const
	{
		return _spriteIdle->getWidth();
	}

	int Button::getHeight() const
	{
		return _spriteIdle->getHeight();
	}

	bool Button::isMouseInButton()
	{
		RECT collisionRECT;
		const SGA::Sprite* sprite = _spriteIdle;
		collisionRECT = SGA::makeRectCenter<RECT, int>(_x, _y, sprite->getWidth(), sprite->getHeight());
		switch (_shape) {
		case SHAPE_CIRCLE:
			return  SGA::isPointInCircle(collisionRECT, GET_KEY_MANAGER()->getCurrentMousePos());
			break;
		case SHAPE_RECTANGLE:
			return  SGA::isPointInRect(collisionRECT, GET_KEY_MANAGER()->getCurrentMousePos());
			break;
		default:
			assert(false);
			return false;
		}
	}
}