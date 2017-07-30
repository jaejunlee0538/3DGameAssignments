#include "UIBase.h"
#include <SpriteIface.h>
#include <assert.h>
#include <CameraManager.h>
namespace SGA {
	SGA::tstring UIBase::_defaultRenderCamera = TEXT("UI");

	UIBase::UIBase()
		:_targetCamera(_defaultRenderCamera)
	{
		setPosition(0, 0);
	}


	UIBase::~UIBase()
	{
	}

	void UIBase::setRenderCamera(SGA::tstring layerName) {
		_targetCamera = layerName;
		for (auto it = _childUI.begin(); it != _childUI.end(); ++it) {
			(*it)->setRenderCamera(_targetCamera);
		}
	}

	void UIBase::release() {
		_childUI.clear();
	}

	inline void UIBase::setTopLeft(float x, float y)
	{
		setPosition(x + getWidth() / 2, y + getHeight() / 2);
	}

	void UIBase::setPosition(float x, float y)
	{
		_x = x;
		_y = y;
	}

	void UIBase::addChild(UIBase * child)
	{
		_childUI.push_back(child);
		child->setRenderCamera(_targetCamera);
	}

	void UIBase::renderOnLayer(const SGA::SpriteIface * sprite, float x, float y)
	{
		GET_CAMERA_MANAGER()->render(sprite, x, y, Camera::TOP_RENDER_PRIORITY, _targetCamera);
	}

	void UIBase::textoutOnLayer(SGA::tstring str, float x, float y, UINT TA, COLORREF color)
	{
		GET_CAMERA_MANAGER()->renderText(str, color, TA, x, y, Camera::TOP_RENDER_PRIORITY,_targetCamera);
	}

	void UIBase::renderRECTOnLayer(RECT rect, COLORREF color, bool transparent)
	{
		/*SGA::Layer* layer = GET_LAYER_MANAGER()->findLayer(_targetCamera);
		assert(layer != NULL);
		if (transparent) {
			HBRUSH oldBrush = (HBRUSH)SelectObject(layer->getDC(), GetStockObject(NULL_BRUSH));
			COLORREF oldColor = SetDCPenColor(layer->getDC(), color);
			Rectangle(layer->getDC(), rect.left, rect.top, rect.right, rect.bottom);
			SelectObject(layer->getDC(), oldBrush);
			SetDCPenColor(layer->getDC(), oldColor);
		}
		else {
			COLORREF oldColor = SetDCBrushColor(layer->getDC(), color);
			Rectangle(layer->getDC(), rect.left, rect.top, rect.right, rect.bottom);
			SetDCBrushColor(layer->getDC(), oldColor);
		}*/
	}
}