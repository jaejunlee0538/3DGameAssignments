#include "ScrollBackground.h"
#include <SpriteManager.h>
#include <CameraManager.h>
#include <assert.h>
#include <ClockManager.h>
#include "Utils.h"
namespace SGA {
	ScrollBackground::ScrollBackground()
	{
		/*bool _loopX, _loopY;
		int _maxLoopCountX[2], _maxLoopCountY[2];*/
		
	}

	ScrollBackground::~ScrollBackground()
	{
	}

	void ScrollBackground::init(tstring spriteName, float left, float top)
	{
		_maxLoopCountX[0] = 0;
		_maxLoopCountX[1] = 1;
		_maxLoopCountY[0] = 0;
		_maxLoopCountY[1] = 1;
		_topLeft.x = left;
		_topLeft.y = top;
		_nX = 1;
		_nY = 1;
		_vx = 0;
		_vy = 0;
		const SGA::Sprite* bkSprite = GET_SPRITE_MANAGER()->findSprite(spriteName);
		_tiles.resize(1, std::vector<const SGA::Sprite*>(1));
		_tiles[0][0] = bkSprite;
		_w = _tiles[0][0]->getWidth();
		_h = _tiles[0][0]->getHeight();
	}

	void ScrollBackground::init(tstring spritePrefix, int nX, int nY, float left, float top)
	{
		_maxLoopCountX[0] = 0;
		_maxLoopCountX[1] = 1;
		_maxLoopCountY[0] = 0;
		_maxLoopCountY[1] = 1;

		_topLeft.x = left;
		_topLeft.y = top;
		_nX = nX;
		_nY = nY;
		_vx = 0;
		_vy = 0;
		std::vector<const SGA::Sprite*> sprites;
		GET_SPRITE_MANAGER()->findSpriteList(spritePrefix, sprites);
		assert(sprites.size() == nX*nY);
		_tiles.resize(nY, std::vector<const SGA::Sprite*>(nX));

		for (int iy = 0; iy < nY; ++iy) {
			for (int ix = 0; ix < nX; ++ix) {
				_tiles[iy][ix] = sprites[iy*nX + ix];
			}
		}
		_w = _tiles[0][0]->getWidth();
		_h = _tiles[0][0]->getHeight();
	}

	void ScrollBackground::render(CameraIface* camera, float priority)
	{
		float cx, cy;//ī�޶� �߽��� World��ǥ
		float cLeft, cTop;
		cx = camera->getPositionX();
		cy = camera->getPositionY();

		//ī�޶� ȭ���� Top/Left�� World��ǥ ���
		cLeft = cx - camera->getWidth() / 2;
		cTop = cy - camera->getHeight() / 2;

		POINT tileIdx;
		POINT nTileRender;
		int drawX, drawY;
		float dx, dy;
		dx = cLeft - _topLeft.x; //ù��° Ÿ���� Top/Left world��ǥ�κ��� ī�޶� topLeft�� ��� ��ǥ
		dy = cTop - _topLeft.y;
		if (dx >= 0)	tileIdx.x = dx / _w;
		else			tileIdx.x = dx / _w - 1;
		if (dy >= 0)	tileIdx.y = dy / _h;
		else			tileIdx.y = dy / _h - 1;

		//ȭ�� TopLeft�� �������� Ÿ���� �ε����� ����Ѵ�.
		tileIdx.x = limitXIdx(tileIdx.x);
		tileIdx.y = limitXIdx(tileIdx.y);

		//�������ؾ��ϴ� Ÿ�� ������ ����Ѵ�.
		nTileRender.x = tileIdx.x + camera->getWidth() / _w + 2;
		nTileRender.y = tileIdx.y + camera->getHeight() / _h + 2;
		nTileRender.x = limitXIdx(nTileRender.x);
		nTileRender.y = limitXIdx(nTileRender.y);
		nTileRender.x -= tileIdx.x;
		nTileRender.y -= tileIdx.y;

		//ȭ�� TopLeft�� �������� Ÿ���� World���� topLeft��ǥ ���
		drawX = _topLeft.x + tileIdx.x *_w;
		drawY = _topLeft.y + tileIdx.y *_h;

		tileIdx.x %= _nX;
		tileIdx.y %= _nY;

		if (tileIdx.x < 0)
			tileIdx.x += _nX;
		if (tileIdx.y < 0)
			tileIdx.y += _nY;
		
		POINT tileIdxEnd;
		tileIdxEnd.x = tileIdx.x + nTileRender.x;
		tileIdxEnd.y = tileIdx.y + nTileRender.y;

		for (int iy = tileIdx.y; iy < tileIdxEnd.y; ++iy) {
			int tmpX = drawX;
			for (int ix = tileIdx.x; ix < tileIdxEnd.x; ++ix, tmpX += _w) {
				camera->render(_tiles[iy%_nY][ix%_nX], tmpX, drawY, priority);
				//SGA::LogDebugMessage(TEXT("%d %d %d %d\n"), iy%_nY, ix%_nX, drawY, tmpX);
			}
			drawY += _h;
		}
		//SGA::LogDebugMessage(TEXT("------------\n"));
	}

	void ScrollBackground::update() {
		if (_vx != 0.0f || _vy != 0.0f) {
			float dt = GET_CLOCK_MANAGER()->getDeltaGameTime();
			_topLeft.x += dt*_vx;
			_topLeft.y += dt*_vy;
		}
	}

	void ScrollBackground::setSpeedX(float vx)
	{
		_vx = vx;
	}

	void ScrollBackground::setSpeedY(float vy)
	{
		_vy = vy;
	}

	void ScrollBackground::moveX(float dx)
	{
		_topLeft.x += dx;
	}

	void ScrollBackground::moveY(float dy)
	{
		_topLeft.y += dy;
	}

	int ScrollBackground::limitXIdx(int idx)
	{
		if (idx < _maxLoopCountX[0] * _nX) {
			idx = _maxLoopCountX[0] * _nX;
		}
		if (idx > _maxLoopCountX[1] * _nX) {
			idx = _maxLoopCountX[1] * _nX;
		}
		return idx;
	}

	int ScrollBackground::limitYIdx(int idx)
	{
		if (idx < _maxLoopCountY[0] * _nY) {
			idx = _maxLoopCountY[0] * _nY;
		}
		if (idx > _maxLoopCountY[1] * _nY) {
			idx = _maxLoopCountY[1] * _nY;
		}
		return idx;
	}
}