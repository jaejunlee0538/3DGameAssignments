#include "CameraManager.h"
#include <BackgroundIface.h>

namespace SGA {
	CameraManager::CameraManager()
	{
	}

	CameraManager::~CameraManager()
	{
		release();
	}

	HRESULT CameraManager::init()
	{
		return S_OK;
	}

	void CameraManager::render(const SpriteIface * sprite, float x, float y, float priority)
	{
		for (auto it = _camera.begin(); it != _camera.end(); ++it) {
			it->second->render(sprite, x, y, priority);
		}
	}

	void CameraManager::render(const SpriteIface * sprite, float x, float y, float priority, tstring targetCamera)
	{
		findCamera(targetCamera)->render(sprite, x, y, priority);
	}

	void CameraManager::render(BackgroundIface * background, float priority, tstring targetCamera)
	{
		background->render(findCamera(targetCamera), priority);
	}

	void CameraManager::renderText(tstring text, COLORREF color, int textAlign, float x, float y, float priority, tstring targetCamera)
	{
		findCamera(targetCamera)->renderText(text, color, textAlign, x, y, priority);
	}

	void CameraManager::clearFilms()
	{
		for (auto it = _camera.begin(); it != _camera.end(); ++it) {
			it->second->clearFilm();
		}
	}

	void CameraManager::output(HDC hdc)
	{
		for (auto it = _camera.begin(); it != _camera.end(); ++it) {
			it->second->output(hdc);
		}
	}

	void CameraManager::update()
	{
		for (auto it = _camera.begin(); it != _camera.end(); ++it) {
			it->second->update();
		}
	}

	void CameraManager::release()
	{
		for (auto it = _camera.begin(); it != _camera.end(); ++it) {
			delete it->second;
		}
		_camera.clear();
	}

	void CameraManager::addCamera(CameraIface * camera, tstring name)
	{
		auto it = _camera.find(name);
		if (it != _camera.end()) {
			delete it->second;
			it->second = camera;
		}
		else {
			_camera.insert(std::make_pair(name, camera));
		}
	}

	CameraIface * CameraManager::findCamera(tstring name)
	{
		auto it = _camera.find(name);
		if (it != _camera.end()) {
			return it->second;
		}
		return NULL;
	}
}