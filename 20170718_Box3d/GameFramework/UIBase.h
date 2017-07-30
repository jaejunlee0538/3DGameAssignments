#pragma once
#include <string>
#include <Windows.h>
#include <vector>
#include <tstring.h>
namespace SGA {
	class SpriteIface;
	class Layer;

	class UIBase
	{
	public:
		UIBase();
		virtual ~UIBase();

		virtual void setRenderCamera(SGA::tstring layerName);
		virtual HRESULT init() { return S_OK; }
		virtual void release();
		virtual void update() {}
		virtual void render() {}
		virtual void render(HDC hdc){}
		virtual void setTopLeft(float x, float y);
		virtual void setPosition(float x, float y);
		virtual float getPositionX() const { return _x; }
		virtual float getPositionY() const { return _y; }
		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
	protected:
		/*
		UI가 다른 UI객체를 소유하는 경우 소유자와 child UI가 동일한 레이어에
		렌더되는 것을 보장하기 위해서는 addChild를 호출해야 한다.
		*/
		void addChild(UIBase* child);
		/*
		이 멤버함수를 이용하여 _targetCamera 레이어에 sprite를 렌더링할 수 있다.
		*/
		void renderOnLayer(const SGA::SpriteIface* sprite, float x, float y);

		void textoutOnLayer(SGA::tstring str, float x, float y, UINT TA, COLORREF color);
		void renderRECTOnLayer(RECT rect, COLORREF color, bool transparent);
	protected:
		SGA::tstring _targetCamera;
		std::vector<UIBase*> _childUI;
		float _x, _y;
	private:
		static SGA::tstring _defaultRenderCamera;
	};

}