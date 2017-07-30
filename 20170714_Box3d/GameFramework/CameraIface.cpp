#include "CameraIface.h"
#include <numeric>
#include <CommonMacroHeader.h>
namespace SGA {
	namespace __private {

		RenderTask::RenderTask(float priority, float x, float y)
			:_x(x), _y(y), _priority(priority)
		{

		}

		DelayedSprite::DelayedSprite(SpriteIface * sprite, float priority, float x, float y)
			: _sprite(sprite), RenderTask(priority, x, y) {

		}

		DelayedSprite::~DelayedSprite() {
			_sprite->release();
			delete _sprite;
		}

		void DelayedSprite::render(HDC hdc, float offsetX, float offsetY) const {
			_sprite->render(hdc, _x + offsetX, _y + offsetY);
		}

		TextRenderTaks::TextRenderTaks(tstring text, COLORREF color, int textAlign, float priority, float x, float y)
			:_text(text),_color(color), _textAlign(textAlign), RenderTask(priority, x, y)
		{
		}

		void TextRenderTaks::render(HDC hdc, float offsetX, float offsetY) const
		{
			int oldBK = SetBkMode(hdc, TRANSPARENT);
			COLORREF oldColor = SetTextColor(hdc, _color);
			UINT oldTA = SetTextAlign(hdc, _textAlign);
			TextOut(hdc, _x + offsetX, _y + offsetY, _text.c_str(), _text.size());
			SetTextAlign(hdc, oldTA);
			SetTextColor(hdc, oldColor);
			SetBkMode(hdc, oldBK);
		}
	}

	namespace Camera {
#undef max
#undef min
		const float TOP_RENDER_PRIORITY = std::numeric_limits<float>::max();
		const float BOTTOM_RENDER_PRIORITY = std::numeric_limits<float>::min();
	}

	CameraIface::CameraIface() {
		_renderPos.x = _renderPos.y = 0;
	}

	CameraIface::~CameraIface() {

	}

	void CameraIface::init(int width, int height, float posX, float posY) {
		setPosition(posX, posY);
		_width = width;
		_height = height;
	}

	void CameraIface::setFilmPosition(float x, float y) {
		_renderPos.x = x;
		_renderPos.y = y;
	}

	void CameraIface::clearFilm()
	{
		_spritesToRender.clear();
	}

	void CameraIface::output(HDC hdc) {
		for (auto it = _spritesToRender.begin(); it != _spritesToRender.end(); ++it) {
			(*it)->render(hdc, _renderPos.x, _renderPos.y);
		}
	}

	void CameraIface::render(const SpriteIface * sprite, float posX, float posY, float priority) {
		POINTFLOAT pos = transformWorldToScreen(posX, posY);
		_spritesToRender.insert(new __private::DelayedSprite(sprite->clone(), priority, pos.x, pos.y));
	}

	void CameraIface::renderText(tstring text, COLORREF color, int textAlign, float posX, float posY, float priority)
	{
		POINTFLOAT pos = transformWorldToScreen(posX, posY);
		_spritesToRender.insert(new __private::TextRenderTaks(text, color, textAlign, priority, posX, posY));
	}

	POINTFLOAT CameraIface::transformWorldToScreen(float x, float y) const {
		POINTFLOAT transformed;
		transformed.x = x - _pos.x + _width / 2;
		transformed.y = y - _pos.y + _height / 2;
		return transformed;
	}

	void CameraIface::setPosition(float x, float y) {
		_pos.x = x;
		_pos.y = y;
	}
	
	RECT CameraIface::getCameraRECT() const
	{
		return SGA::makeRectCenter<RECT, int>(getPositionX(), getPositionY(), getWidth(), getHeight());
	}
}