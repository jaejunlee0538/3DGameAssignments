#pragma once

#include <SingletonBase.h>
#include <string>
#include <map>
#include <Windows.h>
#include <vector>
//fmod hpp�߰�!!
#include "inc\fmod.hpp"
#include "tstring.h"
#include <functional>
//lib��ũ...
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

		//���ϴ� ����...
		void play(tstring keyName, float volume);
		void setPaused(tstring keyName);
		void resume(tstring keyName);
		void stop(tstring keyName);
		void stopAll();

		//�Ͻ� ���� �Ǿ��°�?
		bool isSoundPaused(tstring keyName);

		//���� ������̳�?
		bool isSoundPlaying(tstring keyName);

		//fmod �ý��� ����...
		void update(void);

		std::vector<tstring> getSoundList() const;

		void loopTag(tstring keyName);
		tstring getTag(tstring keyName, tstring tagName);

		void setVolume(tstring keyName, float volume);

		int getSoundLengthMS(tstring keyName);
		int getPlayPositionMS(tstring keyName) const;
		void setPlayPositionMS(tstring keyName, unsigned int pos);

		//���尡 ������ ����Ǿ��� �� ȣ���� �ݹ��Լ��� �߰��Ѵ�.
		void addSoundFinishedCallback(SOUND_FINISHED_CALLBACK cb, void* data);
	private:
		//����ϰ� ���� ���� ä�� ����� �ε����� ��ȯ�Ѵ�. ���� ������ ������ -1�� ��ȯ�Ѵ�.
		int findFreeChannel() const;
		//channelIdx�� ä���� ��� ���̶�� true�� ��ȯ�Ѵ�. �ش� �ε����� ä���� �Ҵ�Ǿ� ���� ������ false�� ��ȯ.
		bool isPlaying(int channelIdx);
		//channelIdx�� ����� ä���� �Ҵ�Ǿ� ������ true�� ��ȯ�Ѵ�.
		bool isActiveChannel(int channelIdx) const;
		//channelIdx�� ä���� ����� �����.
		void stop(int channelIdx);
		//���� ��� ���� ä�ο��� keyName ���带 ����ϰ� �ִ� ä���� �ε����� ��ȯ�Ѵ�. Ž���� �����ϸ� -1�� ��ȯ�Ѵ�.
		int findPlayingSound(tstring keyName) const;
		//channelIdx�� ��� ���̸� ����� ���߰� channelIdx�� ä�� ����� NULL�� �ʱ�ȭ�Ѵ�.
		void clearChannel(int channelIdx);
		//channel�� �ִ� ä�� ����� ã�� clear�Ѵ�. clearChannel(int)�� �����ϱ� �����Ѵ�.
		void clearChannel(FMOD::Channel* channel);
		//ä�� ��Ͽ��� channel�� �ش��ϴ� ����� �ε����� ��ȯ�Ѵ�. Ž���� �����ϸ� -1�� ��ȯ�Ѵ�.
		int getChannelIdx(FMOD::Channel* channel);
		//keyName�� ���� FMOD::Sound ��ü�� �����͸� ��ȯ�Ѵ�.
		FMOD::Sound* findSound(tstring keyName);
		//keyName�� ���� FMOD::Sound ��ü�� �����͸� ��ȯ�Ѵ�.
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