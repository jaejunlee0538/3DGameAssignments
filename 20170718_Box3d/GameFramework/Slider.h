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
		minVal, maxVal, initVal : �ּҰ�, �ִ밪, �ʱⰪ

		railSprite : �����̴��� ������ ������ ��������Ʈ
		sliderSprite : �����̴� ��������Ʈ(�簢��)
		*/
		virtual HRESULT init(float minVal, float maxVal, float initVal, SGA::tstring railSprite, SGA::tstring sliderSprite);
		virtual void release();
		virtual void update();
		virtual void render();
		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual void setCaption(SGA::tstring caption);
		/*
		���콺�� �����̴��� ������ ������ ȣ��Ǵ� �ݹ��Լ�.
		�ݹ��Լ��� ù��° �Ű������� ���� Slider���̰� �ι��� �Ű������� ����� ���� �������̴�.
		*/
		void setSliderMoveCallback(CALLBACK_FUNC_PARAM cb, void* data);

		void setMinVal(float minVal) { _minVal = minVal; }
		void setMaxVal(float maxVal) { _maxVal = maxVal; }

		/*
		���� �����̴� ���� �����Ѵ�. �� �Լ��� ���� �� ������ �ݹ��Լ��� ȣ������ �ʴ´�.
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