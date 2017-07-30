#pragma once

#include <SingletonBase.h>
#include <map>
#include <string>
#include <tstring.h>

//============================================
//======= ## 20170516 sceneManager ## ======== 
//============================================
namespace SGA {
	class GameNode;
	class SceneManager : public SGA::SingletonBase <SceneManager>
	{
	public:
		typedef std::map<tstring, GameNode*> mapSceneList;
		typedef std::map<tstring, GameNode*>::iterator mapSceneIter;

	private:
		static GameNode* _currentScene; //�����
		static GameNode* _loadingScene; //�ε���
		static GameNode* _readyScene; //��ü ��� ���� ��

		//�óĵ� : ���ÿ����� Ȩ������...
		mapSceneList _mSceneList; //������Ʈ

		DWORD _loadThreadID;

	public:
		HRESULT init(void);
		void release(void);
		void update(void);
		void render(void);

		//�� �߰��Ѵ�
		GameNode* addScene(tstring sceneName, GameNode* scene);

		//�� ��ü...
		HRESULT changeScene(tstring sceneName);

		GameNode* getCurrentScene();

		//�ε� ������ �Լ�
		friend DWORD CALLBACK loadingThread(LPVOID prc);

		SceneManager(void) {};
		~SceneManager(void) {};
	};
}
#define GET_SCENE_MANAGER()	SGA::SceneManager::getSingleton()