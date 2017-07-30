#include "SoundManager.h"
#include <Utils.h>
#include <assert.h>
namespace SGA {
	SoundManager::SoundManager(void)
	{
		
	}

	SoundManager::~SoundManager(void)
	{
	}

	void SoundManager::stop(int channelIdx)
	{
		LogDebugMessage(TEXT("%s channel 재생 중지\n"), _playSoundName[channelIdx]);
		_channel[channelIdx]->stop();
	}

	bool SoundManager::isPlaying(int channelIdx)
	{
		if (channelIdx < 0 || channelIdx >= TOTALSOUNDBUFFER) {
			return false;
		}
		if (_channel[channelIdx] != false) {
			bool playing;
			_channel[channelIdx]->isPlaying(&playing);
			return playing;
		}
		return false;
	}

	bool SoundManager::isActiveChannel(int channelIdx) const
	{
		return _channel[channelIdx] != NULL && _sound[channelIdx] != NULL;
	}

	int SoundManager::findFreeChannel() const
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i) {
			if (_sound[i] == NULL) {
				return i;
			}
		}
		return -1;
	}

	int SoundManager::findPlayingSound(tstring keyName) const
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i) {
			if (isActiveChannel(i)) {
				if (keyName.compare(_playSoundName[i]) == 0) {
					//일치
					return i;
				}
			}
		}
		return -1;
	}

	void SoundManager::clearChannel(int channelIdx)
	{
		LogDebugMessage(TEXT("%s channel 정리\n"), _playSoundName[channelIdx]);
		bool isPlaying;
		_channel[channelIdx]->isPlaying(&isPlaying);
		if (isPlaying) {
			stop(channelIdx);
		}
		_channel[channelIdx] = NULL;
		_sound[channelIdx] = NULL;
		_playSoundName[channelIdx][0] = NULL;
	}

	void SoundManager::clearChannel(FMOD::Channel * channel)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i) {
			if (_channel[i] == channel) {
				clearChannel(i);
				break;
			}
		}
	}

	int SoundManager::getChannelIdx(FMOD::Channel * channel)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i) {
			if (_channel[i] == channel) {
				return i;
			}
		}
		return -1;
	}

	FMOD::Sound * SoundManager::findSound(tstring keyName)
	{
		arrSounds::iterator it = _mTotalSounds.find(keyName);
		if (it != _mTotalSounds.end()) {
			return it->second;
		}
		return nullptr;
	}

	const FMOD::Sound * SoundManager::findSound(tstring keyName) const{
		return const_cast<SoundManager*>(this)->findSound(keyName);
	}

	HRESULT SoundManager::init(void)
	{
		//사운드 시스템 생성한다
		System_Create(&_system);

		//채널수 설정
		_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

		//채널수와 사운드 갯수는 맞춰준다!!
		_sound = new FMOD::Sound*[TOTALSOUNDBUFFER];
		_channel = new FMOD::Channel*[TOTALSOUNDBUFFER];
		_playSoundName = new TCHAR*[TOTALSOUNDBUFFER];
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i) {
			_playSoundName[i] = new TCHAR[MAX_SOUND_NAME_LENGTH];
			_playSoundName[i][0] = NULL;
		}
		//초기화
		ZeroMemory(_sound, sizeof(FMOD::Sound*) * (TOTALSOUNDBUFFER));
		ZeroMemory(_channel, sizeof(FMOD::Channel*) * (TOTALSOUNDBUFFER));

		return S_OK;
	}

	void SoundManager::release(void)
	{
		if (_channel != NULL || _sound != NULL)
		{
			for (int i = 0; i < TOTALSOUNDBUFFER; i++)
			{
				if (_channel != NULL)
				{
					if (_channel[i]) _channel[i]->stop();
				}

				if (_sound != NULL)
				{
					if (_sound[i]) _sound[i]->release();
				}
			}
		}
		if (_playSoundName) {
			for (int i = 0; i < TOTALSOUNDBUFFER; i++){
				delete[] _playSoundName[i];
			}
		}

		//메모리 지운다
		SAFE_DELETE_ARRAY(_channel);
		SAFE_DELETE_ARRAY(_sound);
		SAFE_DELETE_ARRAY(_playSoundName);

		if (_system != NULL)
		{
			_system->release();
			_system->close();
		}

		_mTotalSounds.clear();
	}

	//flac, mp3, wave, midi, ogg, m4a, aac, aif, aiff, wma
	void SoundManager::addSound(tstring keyName, tstring soundName, bool background, bool loop)
	{
		FMOD::Sound* sound = NULL;
		//뺑뻉이 사운드냐?
		if (loop)
		{
			//bgm!!
			if (background)
			{
				//배경음악일때...
				std::string str = convertToString(soundName).c_str();
				_system->createStream(convertToString(soundName).c_str(), FMOD_LOOP_NORMAL, 0, &sound);
			}
			else
			{
				//일반 사운드면....
				_system->createSound(convertToString(soundName).c_str(), FMOD_LOOP_NORMAL, 0, &sound);
			}
		}
		else
		{
			_system->createSound(convertToString(soundName).c_str(), FMOD_DEFAULT, 0, &sound);
		}
		assert(sound != NULL);
		//맵에 사운드를 키값과 함께 넣어준다
		_mTotalSounds.insert(std::make_pair(keyName, sound));
	}

	void SoundManager::play(tstring keyName)
	{
		int freeChannel = findFreeChannel();
		if (freeChannel < 0) {
			//모든 채널 사용중
			return;
		}
		arrSounds::iterator it = _mTotalSounds.find(keyName);
		if (it != _mTotalSounds.end()) {
			_system->playSound(FMOD_CHANNEL_FREE, it->second, false, &_channel[freeChannel]);
			_sound[freeChannel] = it->second;
			_stprintf_s(_playSoundName[freeChannel], MAX_SOUND_NAME_LENGTH, TEXT("%s"), keyName.c_str());
			_channel[freeChannel]->setCallback(&SoundManager::callbackChannelEvent);
			LogDebugMessage(TEXT("%s channel 재생\n"), _playSoundName[freeChannel]);
		}
	}

	//이하는 숙제...
	void SoundManager::play(tstring keyName, float volume)
	{
		int freeChannel = findFreeChannel();
		if (freeChannel < 0) {
			//모든 채널 사용중
			return;
		}
		arrSounds::iterator it = _mTotalSounds.find(keyName);
		if (it != _mTotalSounds.end()) {
			_system->playSound(FMOD_CHANNEL_FREE, it->second, false, &_channel[freeChannel]);
			_channel[freeChannel]->setVolume(volume);
			_sound[freeChannel] = it->second;
			_stprintf_s(_playSoundName[freeChannel], MAX_SOUND_NAME_LENGTH, TEXT("%s"), keyName.c_str());
			_channel[freeChannel]->setCallback(&SoundManager::callbackChannelEvent);
			LogDebugMessage(TEXT("%s channel 재생\n"), _playSoundName[freeChannel]);
		}
		else {
			LogDebugMessage(TEXT("WARN : %s 재생 실패\n"), keyName.c_str());
		}
	}

	void SoundManager::setPaused(tstring keyName)
	{
		int channelIdx = findPlayingSound(keyName);
		if (channelIdx < 0) {
			return;
		}
		_channel[channelIdx]->setPaused(true);
	}

	void SoundManager::resume(tstring keyName)
	{
		int channelIdx = findPlayingSound(keyName);
		if (channelIdx < 0) {
			return;
		}
		_channel[channelIdx]->setPaused(false);
	}

	void SoundManager::stop(tstring keyName)
	{
		int channelIdx = findPlayingSound(keyName);
		if (channelIdx < 0) {
			return;
		}
		stop(channelIdx);
	}

	void SoundManager::stopAll()
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; ++i) {
			if (_sound[i] != NULL) {

				_channel[i]->stop();
			}
		}
	}

	//일시 정지 되었는가?
	bool SoundManager::isSoundPaused(tstring keyName)
	{
		int channelIdx = findPlayingSound(keyName);
		if (channelIdx < 0) {
			return false;
		}
		bool paused;
		_channel[channelIdx]->getPaused(&paused);
		return paused;
	}

	//현재 재생중이냐?
	bool SoundManager::isSoundPlaying(tstring keyName)
	{
		int channelIdx = findPlayingSound(keyName);
		if (channelIdx < 0) {
			return false;
		}
		bool playing;
		_channel[channelIdx]->isPlaying(&playing);
		return playing;
	}

	//fmod 시스템 갱신...
	void SoundManager::update(void)
	{
		_system->update();
	}

	std::vector<tstring> SoundManager::getSoundList() const
	{
		std::vector<tstring> soundList;
		soundList.reserve(_mTotalSounds.size());
		for (auto it = _mTotalSounds.begin(); it != _mTotalSounds.end(); ++it) {
			soundList.push_back(it->first);
		}
		return soundList;
	}

	void SoundManager::loopTag(tstring keyName)
	{
		FMOD::Sound* sound = findSound(keyName);
		FMOD_TAG tag;
		int nTags;
		sound->getNumTags(&nTags, 0);
		for (int i = 0; i < nTags; ++i) {
			sound->getTag(NULL, i, &tag);
			switch (tag.datatype) {
			case FMOD_TAGDATATYPE_STRING:
			{
				char buffer[200];
				memcpy(buffer, tag.data, tag.datalen);
				LogDebugMessage(TEXT("%s : %s\n"), 
					SGA::convertToTString(std::string(tag.name)).c_str(), 
					SGA::convertToTString(std::string(buffer)).c_str());
			}
				break;
			}
			
		}
	}

	tstring SoundManager::getTag(tstring keyName, tstring tagName)
	{
		FMOD::Sound* sound = findSound(keyName);
		if (sound == NULL)
			return tstring();
		FMOD_TAG tag;
		sound->getTag(convertToString(tagName).c_str(), 0, &tag);
		switch (tag.datatype) {
		case FMOD_TAGDATATYPE_INT:
			return to_tstring<int>(*((int*)tag.data));
		case FMOD_TAGDATATYPE_FLOAT:
			return to_tstring<float>(*((float*)tag.data));
		case FMOD_TAGDATATYPE_STRING_UTF8:
		case FMOD_TAGDATATYPE_STRING:
			return convertToTString(std::string((char*)tag.data, tag.datalen));
		case FMOD_TAGDATATYPE_BINARY:
			LogDebugMessage(TEXT("Warn : FMOD_TAGDATATYPE_BINARY\n"));
			return tstring();
		case FMOD_TAGDATATYPE_CDTOC:
			LogDebugMessage(TEXT("Warn : FMOD_TAGDATATYPE_CDTOC\n"));
			return tstring();
		case FMOD_TAGDATATYPE_STRING_UTF16BE:
			LogDebugMessage(TEXT("Warn : FMOD_TAGDATATYPE_STRING_UTF16BE\n"));
			return tstring();
		case FMOD_TAGDATATYPE_STRING_UTF16:
			LogDebugMessage(TEXT("Warn : FMOD_TAGDATATYPE_STRING_UTF16\n"));
			return tstring();
		default:
			LogDebugMessage(TEXT("Warn : Unknown SoundTag Type\n"));
			return tstring();
		}
	}

	void SoundManager::setVolume(tstring keyName, float volume)
	{
		int idx = findPlayingSound(keyName);
		if (idx < 0)
			return;
		_channel[idx]->setVolume(volume);
	}

	int SoundManager::getSoundLengthMS(tstring keyName)
	{
		FMOD::Sound* sound = findSound(keyName);
		if (sound == NULL)
			return -1;
		unsigned int len;
		sound->getLength(&len, FMOD_TIMEUNIT_MS);
		return len;
	}

	int SoundManager::getPlayPositionMS(tstring keyName) const
	{
		int idx = findPlayingSound(keyName);
		if (idx < 0)
			return -1;

		unsigned int pos;
		_channel[idx]->getPosition(&pos, FMOD_TIMEUNIT_MS);
		return pos;
	}

	void SoundManager::setPlayPositionMS(tstring keyName, unsigned int pos)
	{
		int idx = findPlayingSound(keyName);
		if (idx < 0)
			return;
		_channel[idx]->setPosition(pos, FMOD_TIMEUNIT_MS);
	}

	void SoundManager::addSoundFinishedCallback(SOUND_FINISHED_CALLBACK cb, void * data)
	{
		_soundFinishedCallback.push_back(SoundFinishedCallback());
		_soundFinishedCallback.back().set(cb, data);
	}

	FMOD_RESULT F_CALLBACK SoundManager::callbackChannelEvent(FMOD_CHANNEL * channel, FMOD_CHANNEL_CALLBACKTYPE type, void * commanddata1, void * commanddata2)
	{
		///*
		//FMOD_CHANNEL_CALLBACKTYPE_END,                  /* Called when a sound ends. */
		//FMOD_CHANNEL_CALLBACKTYPE_VIRTUALVOICE,         /* Called when a voice is swapped out or swapped in. */
		//	FMOD_CHANNEL_CALLBACKTYPE_SYNCPOINT,            /* Called when a syncpoint is encountered.  Can be from wav file markers. */
		//	FMOD_CHANNEL_CALLBACKTYPE_OCCLUSION,            /* Called when the channel has its geometry occlusion value calculated.  Can be used to clamp or change the value. */

		//	FMOD_CHANNEL_CALLBACKTYPE_MAX,                  /* Maximum number of callback types supported. */
		//	FMOD_CHANNEL_CALLBACKTYPE_FORCEINT = 65536      /* Makes sure this enum is signed 32bit. */
		//*/
		switch (type) {
		case FMOD_CHANNEL_CALLBACKTYPE_END:
		{
			int idx = GET_SOUND_MANAGER()->getChannelIdx((FMOD::Channel*)channel);
			if (idx >= 0) {
				unsigned int pos;
				((FMOD::Channel*)channel)->getPosition(&pos, FMOD_TIMEUNIT_MS);
				unsigned int len;
				GET_SOUND_MANAGER()->_sound[idx]->getLength(&len, FMOD_TIMEUNIT_MS);
				if (pos == len) {
					//사운드가 끝까지 재생된 후 종료될 때
					for (int i = 0; i < GET_SOUND_MANAGER()->_soundFinishedCallback.size(); ++i) {
						GET_SOUND_MANAGER()->_soundFinishedCallback[i].call(GET_SOUND_MANAGER()->_playSoundName[idx]);
					}
				}
			}
			GET_SOUND_MANAGER()->clearChannel((FMOD::Channel*)channel);
		}
			break;
		}
		return FMOD_OK;
	}
}