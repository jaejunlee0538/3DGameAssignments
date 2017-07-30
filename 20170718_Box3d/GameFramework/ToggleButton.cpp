#include "ToggleButton.h"
#include <SpriteManager.h>
#include <KeyManager.h>
#include <CommonMacroHeader.h>
#include <CollisionCheckFunc.h>
namespace SGA {
	ToggleButton::ToggleButton()
	{
	}


	ToggleButton::~ToggleButton()
	{
	}

	HRESULT ToggleButton::init(float posX, float posY, ShapeTypes shape,
		SGA::tstring spriteState1, SGA::tstring spriteState1Hover, SGA::tstring spriteState1Click, SGA::tstring spriteState1Disabled,
		SGA::tstring spriteState2, SGA::tstring spriteState2Hover, SGA::tstring spriteState2Click, SGA::tstring spriteState2Disabled)
	{
		setPosition(posX, posY);
		_shape = shape;
		_spriteIdle[0] = GET_SPRITE_MANAGER()->findSprite(spriteState1);
		_spriteHover[0] = GET_SPRITE_MANAGER()->findSprite(spriteState1Hover);
		_spriteClick[0] = GET_SPRITE_MANAGER()->findSprite(spriteState1Click);
		_spriteDisabled[0] = GET_SPRITE_MANAGER()->findSprite(spriteState1Disabled);
		_spriteIdle[1] = GET_SPRITE_MANAGER()->findSprite(spriteState2);
		_spriteHover[1] = GET_SPRITE_MANAGER()->findSprite(spriteState2Hover);
		_spriteClick[1] = GET_SPRITE_MANAGER()->findSprite(spriteState2Click);
		_spriteDisabled[1] = GET_SPRITE_MANAGER()->findSprite(spriteState2Disabled);
		_state = BUTTON_STATE_IDLE;
		_stateIdx = 0;
		_sprite = _spriteIdle[_stateIdx];
		return S_OK;
	}

	void ToggleButton::setPressingBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbPressBegin.set(cbFunc, userData);
	}

	void ToggleButton::setPressingCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbPressing.set(cbFunc, userData);
	}

	void ToggleButton::setPressingEndCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbPressEnd.set(cbFunc, userData);
	}

	void ToggleButton::setHoverBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbHoverBegin.set(cbFunc, userData);
	}

	void ToggleButton::setHoveringCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbHovering.set(cbFunc, userData);
	}

	void ToggleButton::setHoverEndCallback(CALLBACK_FUNC_PARAM cbFunc, void * userData)
	{
		_cbHoverEnd.set(cbFunc, userData);
	}

	void ToggleButton::setToggleState(int state)
	{
		if (state != 0) {
			_stateIdx = 1;
		}
		else {
			_stateIdx = 0;
		}
		_state = BUTTON_STATE_IDLE;
		_sprite = _spriteIdle[_stateIdx];
	}

	void ToggleButton::enableButton(bool enable)
	{
		if (enable == false) {
			_state = BUTTON_STATE_DISABLED;
			_sprite = _spriteDisabled[_stateIdx];
		}
		else {
			_state = BUTTON_STATE_IDLE;
			_sprite = _spriteIdle[_stateIdx];
		}
	}

	void ToggleButton::release()
	{
	}

	void ToggleButton::render()
	{
		renderOnLayer(_sprite, _x, _y);
	}

	void ToggleButton::update()
	{
		switch (_state) {
		case BUTTON_STATE_IDLE:
			if (isMouseInButton()) {
				if (GET_KEY_MANAGER()->isStayKeyUp(VK_LBUTTON)) {//TODO : KEY_MANAGER 의존성 제거
					_state = BUTTON_STATE_HOVER;
					_sprite = _spriteHover[_stateIdx];
					_cbHoverBegin.call(_stateIdx);
				}
			}
			break;
		case BUTTON_STATE_HOVER:
			if (isMouseInButton()) {
				if (GET_KEY_MANAGER()->isStayKeyDown(VK_LBUTTON)) {
					_state = BUTTON_STATE_CLICK;
					_sprite = _spriteClick[_stateIdx];
					_cbPressBegin.call(_stateIdx);
				}
				else {
					_cbHovering.call(_stateIdx);
				}
			}
			else {
				_state = BUTTON_STATE_IDLE;
				_sprite = _spriteIdle[_stateIdx];
				_cbHoverEnd.call(_stateIdx);
			}
			break;
		case BUTTON_STATE_CLICK:
			if (GET_KEY_MANAGER()->isStayKeyUp(VK_LBUTTON)) {
				//Clicked
				_stateIdx++;
				if (_stateIdx == 2) {
					_stateIdx = 0;
				}
				if (isMouseInButton()) {
					_state = BUTTON_STATE_HOVER;
					_sprite = _spriteHover[_stateIdx];
				}
				else {
					_state = BUTTON_STATE_IDLE;
					_sprite = _spriteIdle[_stateIdx];
				}
				_cbPressEnd.call(_stateIdx);
			}
			else {
				//Clicking
				_cbPressing.call(_stateIdx);
			}
			break;
		case BUTTON_STATE_DISABLED:
		{
			_sprite = _spriteDisabled[_stateIdx];
		}
		break;
		}
	}

	int ToggleButton::getWidth() const
	{
		return _sprite->getWidth();
	}

	int ToggleButton::getHeight() const
	{
		return _sprite->getHeight();
	}

	bool ToggleButton::isMouseInButton()
	{
		RECT collisionRECT;
		const SGA::Sprite* sprite = _sprite;
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