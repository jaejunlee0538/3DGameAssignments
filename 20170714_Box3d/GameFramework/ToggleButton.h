#pragma once
#include "UIBase.h"
#include <Sprite.h>
#include <functional>
namespace SGA {
	class ToggleButton :
		public UIBase
	{
	public:
		enum ShapeTypes {
			SHAPE_CIRCLE,
			SHAPE_RECTANGLE
		};

		enum ButtonStates {
			BUTTON_STATE_IDLE,
			BUTTON_STATE_HOVER,
			BUTTON_STATE_CLICK,
			BUTTON_STATE_DISABLED
		};

		typedef std::function<void(bool toggleState, void* userData)> CALLBACK_FUNC_PARAM;
		//typedef void(*CALLBACK_FUNC_PARAM)(bool currentState, void *);

		ToggleButton();
		virtual ~ToggleButton();
		HRESULT init(float posX, float posY, ShapeTypes shape,
			SGA::tstring spriteState1, SGA::tstring spriteState1Hover, SGA::tstring spriteState1Click, SGA::tstring spriteState1Disabled,
			SGA::tstring spriteState2, SGA::tstring spriteState2Hover, SGA::tstring spriteState2Click, SGA::tstring spriteState2Disabled);

		void setPressingBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setPressingCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setPressingEndCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setHoverBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setHoveringCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setHoverEndCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);

		void setToggleState(int state);

		virtual void enableButton(bool enable);
		virtual void release();
		virtual void render();
		virtual void update();

		virtual int getWidth() const;
		virtual int getHeight() const;
	private:
		bool isMouseInButton();
	private:
		class ButtonCallback {
			CALLBACK_FUNC_PARAM cb;
			void* data;
		public:
			ButtonCallback()
				:cb(NULL), data(NULL) {
			}
			void set(CALLBACK_FUNC_PARAM cb, void* data) {
				this->cb = cb;
				this->data = data;
			}
			inline void call(bool currentState) {
				if (cb) {
					cb(currentState, data);
				}
			}
		};

		ButtonStates _state;
		int _stateIdx;
		ShapeTypes _shape;

		const SGA::Sprite* _sprite;
		const SGA::Sprite* _spriteIdle[2];
		const SGA::Sprite* _spriteHover[2];
		const SGA::Sprite* _spriteClick[2];
		const SGA::Sprite* _spriteDisabled[2];

		ButtonCallback _cbPressBegin;
		ButtonCallback _cbPressing;
		ButtonCallback _cbPressEnd;
		ButtonCallback _cbHoverBegin;
		ButtonCallback _cbHovering;
		ButtonCallback _cbHoverEnd;
	};

}