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
	아틀라스 이미지와 스프라이트 조각들을 생성/초기화/삭제/검색하는 기능을 제공한다.
	*/
	class SpriteManager : public SingletonBase<SpriteManager>
	{
	private:
		struct tagAtlasImage {
			BitmapImage* source;
		};
		//스프라이트 클립 목록
		typedef std::map<tstring, Sprite*> TileSpriteMap;
		//아틀라스 이미지 목록
		typedef std::map<tstring, tagAtlasImage> AtlasImageMap;
	public:
		SpriteManager();
		virtual ~SpriteManager();
		/*
		모든 스프라이트와 아틀라스 이미지를 삭제한다.
		*/
		void release();
		void loadFromJSON(const tstring& configFile);
		void addSprite(const tstring& imageFile, const tstring& spriteName, BitmapImage::TransparentModes transparentMode, float pivotX=0.5f, float pivotY=0.5f);
		void addSprite(const tstring& imageFile, const tstring& spriteName, int clipX, int clipY, int clipW, int clipH);
		void addSprite(const tstring& imageFile, const tstring& spriteName, int clipX, int clipY, int clipW, int clipH, int pivotX, int pivotY);
		void addFrameSprite(const tstring& imageFile, const tstring& spriteNamePrefix, int beginX, int beginY, int clipW, int clipH, int nFrameX, int nFrameY, bool horizontalFirst);
		/*
		namePrefix+"숫자"의 이름을 가진 모든 스프라이트를 sprites 컨테이너에 담아준다.
		*/
		void findSpriteList(tstring namePrefix, std::vector<const Sprite*>& sprites);
		void findSpritesStartWith(tstring namePrefix, std::vector<const Sprite*>& sprites);
		void findSpritesStartWith(tstring namePrefix, std::vector<std::pair<tstring, const Sprite*>>& sprites);
		/*
		key 스프라이트의 원본 객체 포인터를 반환한다.
		스프라이트 객체는 SpriteManager객체 소멸시에 삭제된다.
		*/
		Sprite* findSprite(const tstring& key);
		TileSpriteMap& getSpriteMap() {
			return _spriteMap;
		}
	private:
		/*
		imageFile 아틀라스 이미지를 로드한다.
		처음 로드되는 이미지라면 이미지 파일을 읽어 맵에 저장한다.
		이미 로드된 아틀라스 이미지라면 맵에 저장되어 있는 데이터를 반환한다.
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