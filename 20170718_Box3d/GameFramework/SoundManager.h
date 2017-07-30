#pragma once

#include <SingletonBase.h>
#include <string>
#include <map>
#include <Windows.h>
#include <vector>
//fmod hpp추가!!
#include "inc\fmod.hpp"
#include "tstring.h"
#include <functional>
//lib링크...
#pragma comment (lib, "GameFramework/lib/fmodex_vc.lib")

#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 10
#define MAX_SOUND_NAME_LENGTH	50
#define TOTALSOUNDBUFFER (EXTRACHANNELBUFFER + SOUNDBUFFER)
namespace SGA {
	class SoundManager : public SGA::SingletonBase <SoundManager>
	{
	private:
		typedef std::map<SGA::tstring, FMOD::Sound*> arrSounds;
		typedef std::map<SGA::tstring, FMOD::Sound*>::iterator arrSoundsIter;
		typedef std::map<SGA::tstring, FMOD::Channel*> arrChannels;
		typedef std::map<SGA::tstring, FMOD::Channel*>::iterator arrChannelsIter;
		typedef std::function<void(tstring keyName, void* data)> SOUND_FINISHED_CALLBACK;
		class SoundFinishedCallback {
			SOUND_FINISHED_CALLBACK cb;
			void* data;
		public:
			SoundFinishedCallback()
				:cb(NULL), data(NULL) {
			}
			void set(SOUND_FINISHED_CALLBACK cb, void* data) {
				this->cb = cb;
				this->data = data;
			}
			inline void call(tstring soundName) {
				if (cb) {
					cb(soundName, data);
				}
			}
		};
	public:
		SoundManager(void);
		~SoundManager(void);

		HRESULT init(void);
		void release(void);

		//flac, mp3, wave, midi, ogg, m4a, aac, aif, aiff, wma
		void addSound(tstring keyName, tstring soundName, bool background, bool loop);
		void play(tstring keyName);

		//이하는 숙제...
		void play(tstring keyName, float volume);
		void setPaused(tstring keyName);
		void resume(tstring keyName);
		void stop(tstring keyName);
		void stopAll();

		//일시 정지 되었는가?
		bool isSoundPaused(tstring keyName);

		//현재 재생중이냐?
		bool isSoundPlaying(tstring keyName);

		//fmod 시스템 갱신...
		void update(void);

		std::vector<tstring> getSoundList() const;

		void loopTag(tstring keyName);
		tstring getTag(tstring keyName, tstring tagName);

		void setVolume(tstring keyName, float volume);

		int getSoundLengthMS(tstring keyName);
		int getPlayPositionMS(tstring keyName) const;
		void setPlayPositionMS(tstring keyName, unsigned int pos);

		//사운드가 끝까지 재생되었을 때 호출할 콜백함수를 추가한다.
		void addSoundFinishedCallback(SOUND_FINISHED_CALLBACK cb, void* data);
	private:
		//사용하고 있지 않은 채널 목록의 인덱스를 반환한다. 여유 공간이 없으면 -1을 반환한다.
		int findFreeChannel() const;
		//channelIdx의 채널이 재생 중이라면 true를 반환한다. 해당 인덱스에 채널이 할당되어 있지 않으면 false를 반환.
		bool isPlaying(int channelIdx);
		//channelIdx에 사운드와 채널이 할당되어 있으면 true를 반환한다.
		bool isActiveChannel(int channelIdx) const;
		//channelIdx의 채널의 재생을 멈춘다.
		void stop(int channelIdx);
		//현재 재생 중인 채널에서 keyName 사운드를 재생하고 있는 채널의 인덱스를 반환한다. 탐색에 실패하면 -1을 반환한다.
		int findPlayingSound(tstring keyName) const;
		//channelIdx가 재생 중이면 재생을 멈추고 channelIdx의 채널 목록을 NULL로 초기화한다.
		void clearChannel(int channelIdx);
		//channel이 있는 채널 목록을 찾아 clear한다. clearChannel(int)와 동일하기 동작한다.
		void clearChannel(FMOD::Channel* channel);
		//채널 목록에서 channel에 해당하는 목록의 인덱스를 반환한다. 탐색에 실패하면 -1을 반환한다.
		int getChannelIdx(FMOD::Channel* channel);
		//keyName을 가진 FMOD::Sound 객체의 포인터를 반환한다.
		FMOD::Sound* findSound(tstring keyName);
		//keyName을 가진 FMOD::Sound 객체의 포인터를 반환한다.
		const FMOD::Sound* findSound(tstring keyName) const;

		static FMOD_RESULT F_CALLBACK callbackChannelEvent(FMOD_CHANNEL *channel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2);
	private:
		FMOD::System* _system;

		FMOD::Sound** _sound;
		FMOD::Channel** _channel;
		TCHAR** _playSoundName;

		arrSounds _mTotalSounds;
		std::vector<SoundFinishedCallback> _soundFinishedCallback;
	};
#define GET_SOUND_MANAGER()	SGA::SoundManager::getSingleton()
}