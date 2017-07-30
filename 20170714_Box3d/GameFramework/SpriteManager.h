#pragma once
#include <map>
#include <Windows.h>
#include "Sprite.h"
#include "BitmapImage.h"
#include "SingletonBase.h"
#include <vector>
#include <tstring.h>
namespace SGA {
	/*
	��Ʋ�� �̹����� ��������Ʈ �������� ����/�ʱ�ȭ/����/�˻��ϴ� ����� �����Ѵ�.
	*/
	class SpriteManager : public SingletonBase<SpriteManager>
	{
	private:
		struct tagAtlasImage {
			BitmapImage* source;
		};
		//��������Ʈ Ŭ�� ���
		typedef std::map<tstring, Sprite*> TileSpriteMap;
		//��Ʋ�� �̹��� ���
		typedef std::map<tstring, tagAtlasImage> AtlasImageMap;
	public:
		SpriteManager();
		virtual ~SpriteManager();
		/*
		��� ��������Ʈ�� ��Ʋ�� �̹����� �����Ѵ�.
		*/
		void release();
		void loadFromJSON(const tstring& configFile);
		void addSprite(const tstring& imageFile, const tstring& spriteName, BitmapImage::TransparentModes transparentMode, float pivotX=0.5f, float pivotY=0.5f);
		void addSprite(const tstring& imageFile, const tstring& spriteName, int clipX, int clipY, int clipW, int clipH);
		void addSprite(const tstring& imageFile, const tstring& spriteName, int clipX, int clipY, int clipW, int clipH, int pivotX, int pivotY);
		void addFrameSprite(const tstring& imageFile, const tstring& spriteNamePrefix, int beginX, int beginY, int clipW, int clipH, int nFrameX, int nFrameY, bool horizontalFirst);
		/*
		namePrefix+"����"�� �̸��� ���� ��� ��������Ʈ�� sprites �����̳ʿ� ����ش�.
		*/
		void findSpriteList(tstring namePrefix, std::vector<const Sprite*>& sprites);
		void findSpritesStartWith(tstring namePrefix, std::vector<const Sprite*>& sprites);
		void findSpritesStartWith(tstring namePrefix, std::vector<std::pair<tstring, const Sprite*>>& sprites);
		/*
		key ��������Ʈ�� ���� ��ü �����͸� ��ȯ�Ѵ�.
		��������Ʈ ��ü�� SpriteManager��ü �Ҹ�ÿ� �����ȴ�.
		*/
		Sprite* findSprite(const tstring& key);
		TileSpriteMap& getSpriteMap() {
			return _spriteMap;
		}
	private:
		/*
		imageFile ��Ʋ�� �̹����� �ε��Ѵ�.
		ó�� �ε�Ǵ� �̹������ �̹��� ������ �о� �ʿ� �����Ѵ�.
		�̹� �ε�� ��Ʋ�� �̹������ �ʿ� ����Ǿ� �ִ� �����͸� ��ȯ�Ѵ�.
		*/
		tagAtlasImage loadAtlasImage(const tstring& imageFile, const tstring& colorFormat);
		tagAtlasImage loadAtlasImage(const tstring& imageFile, BitmapImage::TransparentModes transparentMode);
		void insertSprite(const tstring& key, Sprite* sprite);
	private:
		TileSpriteMap _spriteMap;
		AtlasImageMap _atlasImageMap;
	};
#define GET_SPRITE_MANAGER()	SGA::SpriteManager::getSingleton()
}