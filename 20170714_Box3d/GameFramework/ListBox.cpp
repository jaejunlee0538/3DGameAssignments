#include "ListBox.h"
#include <CommonMacroHeader.h>
#define VERTICAL_OFFSET	25
namespace SGA {
	ListBox::ListBox()
	{
	}


	ListBox::~ListBox()
	{
	}

	HRESULT ListBox::init(int x, int y, int w, int h)
	{
		setPosition(x, y);
		_w = w;
		_h = h;
		_selectIdx = 0;
		return S_OK;
	}

	void ListBox::release()
	{
	}

	void ListBox::update()
	{
	}

	void ListBox::render()
	{
		float x, y;
		float ySel;
		x = getPositionX();
		y = getPositionY() - getHeight() / 2;
		for (int i = 0; i < _elements.size(); ++i) {
			if (i == _selectIdx) {
				ySel = y;
			}
			textoutOnLayer(_elements[i], x - getWidth() / 2, y, TA_LEFT, RGB(255, 255, 255));
			y += VERTICAL_OFFSET;
		}
		RECT rect = SGA::makeRectCenter<RECT, int>(x, ySel + VERTICAL_OFFSET / 2, getWidth(), VERTICAL_OFFSET);
		renderRECTOnLayer(rect, RGB(255, 0, 0), true);
	}

	void ListBox::setSelectCallback(CALLBACK_FUNC_PARAM cb, void * data)
	{
		_cbSelect.set(cb, data);
	}

	SGA::tstring ListBox::getString(int selIdx) const
	{
		if (selIdx < 0 || selIdx >= _elements.size()) {
			return SGA::tstring();
		}
		return _elements[selIdx];
	}

	int ListBox::getSelectIdx() const
	{
		return _selectIdx;
	}

	void ListBox::insert(SGA::tstring element)
	{
		_elements.push_back(element);
	}

	void ListBox::remove(SGA::tstring element)
	{
		for (auto it = _elements.begin(); it != _elements.end(); ) {
			if (*it == element) {
				it = _elements.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void ListBox::removeAll()
	{
		_elements.clear();
	}

	void ListBox::select(int idx)
	{
		if (idx < 0 || idx >= _elements.size()) {
			return;
		}
		_selectIdx = idx;
		_cbSelect.call(_selectIdx);
	}

	void ListBox::select(SGA::tstring element)
	{
		for (size_t i = 0; i < _elements.size(); ++i) {
			if (_elements[i] == element) {
				select(i);
			}
		}
	}

	int ListBox::getWidth() const
	{
		return _w;
	}

	int ListBox::getHeight() const
	{
		return _h;
	}
}