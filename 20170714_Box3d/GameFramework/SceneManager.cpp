#include "StdAfx.h"
#include "SceneManager.h"
#include "GameNode.h"
namespace SGA {
	//�ε� ������ �Լ�
	DWORD CALLBACK loadingThread(LPVOID prc)
	{
		//��ü�ؾ� �� init �Լ� ����
		SceneManager::_readyScene->init();

		//������� ��ü�� ������!!
		SceneManager::_currentScene = SceneManager::_readyScene;

		//����...
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
		//���� ��� ���鼭 ����..
		mapSceneIter iter = _mSceneList.begin();

		for (; iter != _mSceneList.end();)
		{
			//���� �� ������ �ݺ��� ���� x
			if (iter->second != NULL)
			{
				iter->second->release();
				if (iter->second) {
					delete iter->second;
				}
				iter = _mSceneList.erase(iter);
			}
			else //�ݺ��� ����!
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

	//�� �߰��Ѵ�
	GameNode* SceneManager::addScene(tstring sceneName, GameNode* scene)
	{
		if (!scene) return NULL;
		_mSceneList.insert(make_pair(sceneName, scene));
		return scene;
	}

	//�� ��ü...
	HRESULT SceneManager::changeScene(tstring sceneName)
	{
		//��ü�ؾ� �� ���� ã�´�
		mapSceneIter find = _mSceneList.find(sceneName);

		//��ü ���� ������ �޽�..
		if (find == _mSceneList.end())
			return E_FAIL;

		if (_currentScene) {
			_currentScene->release(); 
			_currentScene = NULL;
		}

		//��ü �� �ʱ�ȭ
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