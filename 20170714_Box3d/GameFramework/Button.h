#pragma once
#include <Windows.h>
#include <Sprite.h>
#include <functional>
#include "UIBase.h"
namespace SGA {
	class Button : public UIBase
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

		typedef std::function<void(void*)> CALLBACK_FUNC_PARAM;
		//typedef void (*CALLBACK_FUNC_PARAM)(void *);

		Button();
		virtual ~Button();
		HRESULT init(float posX, float posY, ShapeTypes shape,SGA::tstring spriteShared);
		HRESULT init(float posX, float posY, ShapeTypes shape,
			SGA::tstring spriteIDLE, SGA::tstring spriteHover,
			SGA::tstring spriteClick, SGA::tstring spriteDisabled);

		void setPressingBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setPressingCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setPressingEndCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setHoverBeginCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setHoveringCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);
		void setHoverEndCallback(CALLBACK_FUNC_PARAM cbFunc, void* userData);

		virtual void enableButton(bool enable);
		virtual void release();
		virtual void render();
		virtual void render(HDC hdc);
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
			inline void call() {
				if (cb) {
					cb(data);
				}
			}
		};

		ButtonStates _state;
		ShapeTypes _shape;
		const SGA::Sprite* _spriteIdle;
		const SGA::Sprite* _spriteHover;
		const SGA::Sprite* _spriteClick;
		const SGA::Sprite* _spriteDisabled;

		ButtonCallback _cbPressBegin;
		ButtonCallback _cbPressing;
		ButtonCallback _cbPressEnd;
		ButtonCallback _cbHoverBegin;
		ButtonCallback _cbHovering;
		ButtonCallback _cbHoverEnd;
	};
}