#pragma once
#include "CameraIface.h"
#include <string>
#include <map>
#include "SingletonBase.h"
#include <tstring.h>
#include <vector>
namespace SGA {

	class SpriteIface;
	class BackgroundIface;
	class CameraManager : public SingletonBase<CameraManager>
	{
	public:
		CameraManager();
		virtual ~CameraManager();
		HRESULT init();
		void release();

		/*
		name 이름으로 카메라를 추가한다.
		*/
		void addCamera(CameraIface* camera, tstring name);
		
		/*
		World좌표 x,y 위치에 있는 sprite를 모든 카메라들에 렌더링한다.
		*/
		void render(const SpriteIface* sprite, float x, float y, float priority);
		/*
		World좌표 x,y 위치에 있는 sprite를 targetCamera 카메라에만 렌더링한다.
		예를 들어 움직이지 않는 UI 카메라를 화면 좌표 중심에 추가하고 UI오브젝트들을 targetCamera에 렌더하게 한다.
		*/
		void render(const SpriteIface* sprite, float x, float y, float priority, tstring targetCamera);

		void render(BackgroundIface* background, float priority, tstring targetCamera);
		void renderText(tstring text, COLORREF color, int textAlign, float x, float y, float priority, tstring targetCamera);

		/*
		RenderTask 목록을 삭제한다.
		매 렌더 루프 처음에서 반드시 호출해 주어야 한다!
		*/
		void clearFilms();

		/*
		카메라들에 렌더링된 스프라이트들을 hdc에 출력한다.
		*/
		void output(HDC hdc);

		/*
		카메라 매니저에 추가된 모든 카메라를 업데이트 한다.
		*/
		void update();
		CameraIface* findCamera(tstring name);
	private:
		std::map<tstring, CameraIface*> _camera;
	};
#define GET_CAMERA_MANAGER()	SGA::CameraManager::getSingleton()
}