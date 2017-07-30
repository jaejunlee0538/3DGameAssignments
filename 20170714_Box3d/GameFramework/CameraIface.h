#pragma once
#include <Windows.h>
#include "SpriteIface.h"
#include "BitmapImage.h"
#include <set>
#include "tstring.h"
namespace SGA {
	namespace __private {
		class RenderTask {
		public:
			RenderTask(float priority, float x, float y);
			virtual ~RenderTask() {}
			virtual bool operator<(const RenderTask& other) const {
				return _priority < other._priority;
			}
			virtual void render(HDC hdc, float offsetX, float offsetY) const = 0;
		protected:
			float _priority;
			float _x, _y;
		};

		struct RenderTaskComp {
			bool operator() (const RenderTask* left, const RenderTask* right) const{
				return (*left) < (*right);
			}
		};

		class DelayedSprite : public RenderTask {
		public:
			DelayedSprite(SpriteIface* sprite, float priority, float x, float y);
			virtual ~DelayedSprite();
			void render(HDC hdc, float offsetX, float offsetY) const;
		private:
			SpriteIface* _sprite;
		};

		class TextRenderTaks :public RenderTask {
		public:
			TextRenderTaks(tstring text, COLORREF color, int textAlign, float priority, float x, float y);
			void render(HDC hdc, float offsetX, float offsetY) const;
		private:
			tstring _text;
			COLORREF _color;
			int _textAlign;
		};
	}

	namespace Camera {
		extern const float TOP_RENDER_PRIORITY;
		extern const float BOTTOM_RENDER_PRIORITY;
	}

	class CameraIface
	{
	public:
		CameraIface();
		virtual ~CameraIface();
		virtual void update() {}
		virtual void init(int width, int height, float posX, float posY);
		virtual void setFilmPosition(float x, float y);
		virtual void clearFilm();
		virtual void output(HDC hdc);
		//TODO : render함수는 sprite의 위치와 사이즈를 통해 실제 카메라에 보이는지 여부에 따라 렌더링을 수행할지 말지 결정한다.
		virtual void render(const SpriteIface* sprite, float posX, float posY, float priority);
		virtual void renderText(tstring text, COLORREF color, int textAlign, float posX, float posY, float priority);

		virtual POINTFLOAT transformWorldToScreen(float x, float y) const;

		virtual void setPosition(float x, float y);
		inline int getWidth() const{
			return _width;
		}
		inline int getHeight() const{
			return _height;
		}
		inline float getPositionX() const{
			return _pos.x;
		}
		inline float getPositionY() const{
			return _pos.y;
		}
		RECT getCameraRECT() const;
	protected:
		
		POINTFLOAT _pos;
		int _width;
		int _height;
		POINTFLOAT _renderPos;
		std::multiset<__private::RenderTask*, __private::RenderTaskComp> _spritesToRender;
	};
}