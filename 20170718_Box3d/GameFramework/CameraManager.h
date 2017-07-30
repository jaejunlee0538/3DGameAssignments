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
		name �̸����� ī�޶� �߰��Ѵ�.
		*/
		void addCamera(CameraIface* camera, tstring name);
		
		/*
		World��ǥ x,y ��ġ�� �ִ� sprite�� ��� ī�޶�鿡 �������Ѵ�.
		*/
		void render(const SpriteIface* sprite, float x, float y, float priority);
		/*
		World��ǥ x,y ��ġ�� �ִ� sprite�� targetCamera ī�޶󿡸� �������Ѵ�.
		���� ��� �������� �ʴ� UI ī�޶� ȭ�� ��ǥ �߽ɿ� �߰��ϰ� UI������Ʈ���� targetCamera�� �����ϰ� �Ѵ�.
		*/
		void render(const SpriteIface* sprite, float x, float y, float priority, tstring targetCamera);

		void render(BackgroundIface* background, float priority, tstring targetCamera);
		void renderText(tstring text, COLORREF color, int textAlign, float x, float y, float priority, tstring targetCamera);

		/*
		RenderTask ����� �����Ѵ�.
		�� ���� ���� ó������ �ݵ�� ȣ���� �־�� �Ѵ�!
		*/
		void clearFilms();

		/*
		ī�޶�鿡 �������� ��������Ʈ���� hdc�� ����Ѵ�.
		*/
		void output(HDC hdc);

		/*
		ī�޶� �Ŵ����� �߰��� ��� ī�޶� ������Ʈ �Ѵ�.
		*/
		void update();
		CameraIface* findCamera(tstring name);
	private:
		std::map<tstring, CameraIface*> _camera;
	};
#define GET_CAMERA_MANAGER()	SGA::CameraManager::getSingleton()
}