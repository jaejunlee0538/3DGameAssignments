#pragma once
#include "UIBase.h"
#include <Sprite.h>
#include <functional>
namespace SGA {
	class Slider :
		public UIBase
	{
	public:
		typedef std::function<void(float val, void*)> CALLBACK_FUNC_PARAM;
		//typedef void (*CALLBACK_FUNC_PARAM)(void *);

		Slider();
		virtual ~Slider();
		/*
		minVal, maxVal, initVal : 최소값, 최대값, 초기값

		railSprite : 슬라이더가 움직일 레일의 스프라이트
		sliderSprite : 슬라이더 스프라이트(사각형)
		*/
		virtual HRESULT init(float minVal, float maxVal, float initVal, SGA::tstring railSprite, SGA::tstring sliderSprite);
		virtual void release();
		virtual void update();
		virtual void render();
		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual void setCaption(SGA::tstring caption);
		/*
		마우스로 슬라이더를 움직일 때마다 호출되는 콜백함수.
		콜백함수의 첫번째 매개변수는 현재 Slider값이고 두번재 매개변수는 사용자 정의 데이터이다.
		*/
		void setSliderMoveCallback(CALLBACK_FUNC_PARAM cb, void* data);

		void setMinVal(float minVal) { _minVal = minVal; }
		void setMaxVal(float maxVal) { _maxVal = maxVal; }

		/*
		현재 슬라이더 값을 변경한다. 이 함수로 인한 값 변경은 콜백함수를 호출하지 않는다.
		*/
		void setCurrentValue(float val);
		float getCurrentValue() const;
	private:
		void setSliderXPos(float xPos);
		float getSliderXPos()const;
		RECT getSliderRECT() const;

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
			inline void call(float val) {
				if (cb) {
					cb(val, data);
				}
			}
		};
	private:
		float _maxVal;
		float _minVal;
		float _currentVal;
		int _xBegin;
		int _xRailLength;
		POINT _ptPrev;
		POINT _pivot;
		float _xBeforePivot;
		bool _controlling;
		const SGA::Sprite* _railSprite;
		const SGA::Sprite* _sliderSprite;

		SGA::tstring _caption;

		ButtonCallback _sliderMoveCallback;
	};

}