#include "TextUI.h"
#include <SpriteManager.h>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <algorithm>
namespace SGA {

	TextUI::TextUI()
	{
	}

	TextUI::~TextUI()
	{
	}

	HRESULT TextUI::init(float posX, float posY)
	{
		UIBase::init();
		
		if (_charSpritesMap.empty()) {
			tstring name = TEXT("Digit_?");
			for (int i = 0; i < 10; ++i) {
				name.back() = TEXT('0') + i;
				SGA::Sprite* sprite = GET_SPRITE_MANAGER()->findSprite(name);
				assert(sprite != NULL);
				_charSpritesMap[TEXT('0') + i] = sprite;
			}
		}
		setPosition(posX, posY);
		return S_OK;
	}

	void TextUI::release()
	{
		_sprites.clear();
		for (auto it = _charSpritesMap.begin(); it != _charSpritesMap.end(); ++it) {
			it->second->release();
			delete it->second;
		}
		_charSpritesMap.clear();
	}
	
	void TextUI::render()
	{
	}

	void TextUI::render(HDC hdc)
	{
		int x, y;
		x = getPositionX() - getWidth()/2;
		y = getPositionY();
		for (int i = 0; i < _sprites.size(); ++i) {
			_sprites[i]->render(hdc, x, y);
			x += _sprites[i]->getWidth() + 5;
		}
	}
#undef max
	void TextUI::setText(tstring str)
	{
		_sprites.resize(str.size());
		_width = 0;
		_height = 0;
		for (int i = 0; i < str.size(); ++i) {
			_sprites[i] = _charSpritesMap[str[i]];
			_width += _sprites[i]->getWidth();
			_height = std::max(_height, _sprites[i]->getHeight());
		}
		_text = str;
	}

	void TextUI::setInt(int val)
	{
		setText(SGA::to_tstring(val));
	}

	int TextUI::getWidth() const
	{
		return _width;
	}

	int TextUI::getHeight() const
	{
		return _height;
	}
}