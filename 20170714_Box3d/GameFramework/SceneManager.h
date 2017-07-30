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
		static GameNode* _currentScene; //현재씬
		static GameNode* _loadingScene; //로딩씬
		static GameNode* _readyScene; //교체 대기 중인 씬

		//궁냐동 : 남궁영빈의 홈페이지...
		mapSceneList _mSceneList; //씬리스트

		DWORD _loadThreadID;

	public:
		HRESULT init(void);
		void release(void);
		void update(void);
		void render(void);

		//씬 추가한다
		GameNode* addScene(tstring sceneName, GameNode* scene);

		//씬 교체...
		HRESULT changeScene(tstring sceneName);

		GameNode* getCurrentScene();

		//로딩 쓰레드 함수
		friend DWORD CALLBACK loadingThread(LPVOID prc);

		SceneManager(void) {};
		~SceneManager(void) {};
	};
}
#define GET_SCENE_MANAGER()	SGA::SceneManager::getSingleton()