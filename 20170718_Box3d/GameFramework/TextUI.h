#pragma once
#include <Windows.h>
#include <map>
#include <Sprite.h>
#include <vector>
#include "UIBase.h"
#include <tstring.h>
enum TextUIAligmnents {
	ALIGN_LEFT,
	ALIGN_RIGHT,
	ALIGN_CENTER
};
namespace SGA {
	class TextUI : public UIBase
	{
	public:
		typedef std::map<TCHAR, SGA::Sprite*> CharacterSpriteMap;
		TextUI();
		~TextUI();
		HRESULT init(float posX, float posY);
		void release();
		void render();
		void render(HDC dhc);
		void setText(tstring str);
		void setInt(int val);
		int getWidth() const;
		int getHeight() const;
	private:
		tstring _text;
		std::vector<const SGA::Sprite*> _sprites;
		CharacterSpriteMap _charSpritesMap;
		int _width, _height;
	};

}