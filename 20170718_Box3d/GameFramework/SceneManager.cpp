#include "StdAfx.h"
#include "SceneManager.h"
#include "GameNode.h"
namespace SGA {
	//로딩 쓰레드 함수
	DWORD CALLBACK loadingThread(LPVOID prc)
	{
		//교체해야 할 init 함수 실행
		SceneManager::_readyScene->init();

		//현재씬을 교체될 씬으로!!
		SceneManager::_currentScene = SceneManager::_readyScene;

		//해제...
		SceneManager::_loadingScene->release();
		SceneManager::_loadingScene = NULL;
		SceneManager::_readyScene = NULL;

		return NULL;
	}

	GameNode* SceneManager::_currentScene = NULL;
	GameNode* SceneManager::_loadingScene = NULL;
	GameNode* SceneManager::_readyScene = NULL;

	HRESULT SceneManager::init(void)
	{
		_currentScene = NULL;
		_loadingScene = NULL;
		_readyScene = NULL;

		return S_OK;
	}

	void SceneManager::release(void)
	{
		//맵을 모두 돌면서 삭제..
		mapSceneIter iter = _mSceneList.begin();

		for (; iter != _mSceneList.end();)
		{
			//지울 수 있으면 반복자 증가 x
			if (iter->second != NULL)
			{
				iter->second->release();
				if (iter->second) {
					delete iter->second;
				}
				iter = _mSceneList.erase(iter);
			}
			else //반복자 증가!
			{
				++iter;
			}
		}

		_currentScene = NULL;
		_mSceneList.clear();
	}

	void SceneManager::update(void)
	{
		if (_currentScene) _currentScene->update();
	}

	void SceneManager::render(void)
	{
		if (_currentScene) _currentScene->render();
	}

	//씬 추가한다
	GameNode* SceneManager::addScene(tstring sceneName, GameNode* scene)
	{
		if (!scene) return NULL;
		_mSceneList.insert(make_pair(sceneName, scene));
		return scene;
	}

	//씬 교체...
	HRESULT SceneManager::changeScene(tstring sceneName)
	{
		//교체해야 할 씬을 찾는다
		mapSceneIter find = _mSceneList.find(sceneName);

		//교체 씬이 없으면 펄스..
		if (find == _mSceneList.end())
			return E_FAIL;

		if (_currentScene) {
			_currentScene->release(); 
			_currentScene = NULL;
		}

		//교체 씬 초기화
		if (SUCCEEDED(find->second->init()))
		{
			_currentScene = find->second;

			return S_OK;
		}

		return E_FAIL;
	}
	GameNode * SceneManager::getCurrentScene()
	{
		assert(_currentScene != NULL);
		return _currentScene;
	}
}