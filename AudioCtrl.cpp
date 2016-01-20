#include "AudioCtrl.h"

NS_CSC_BEGIN
static AudioCtrl _sharedContext;

AudioCtrl* AudioCtrl::getInstance()
{
	static bool s_bFirstUse = true;
	if (s_bFirstUse)
	{
		_sharedContext.init();
		s_bFirstUse = false;
	}
	return &_sharedContext;
}

AudioCtrl::AudioCtrl(): m_pSimpleAudioEngine(nullptr), m_nCurBackgroundMusicIndex(0), m_bIsListPlaying(false), m_bIsRandom(false)
{
	m_vBackgroundMusics.clear();
}

AudioCtrl::~AudioCtrl()
{
	m_vBackgroundMusics.clear();
	Director::getInstance()->getScheduler()->unschedule("update", this);
}

bool AudioCtrl::init()
{
	bool bRet = false;
	do
	{
		m_pSimpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

		bRet = true;
	} while (false);
	return bRet;
}

void AudioCtrl::addBackgroundMusic(const char* pszFilePath)
{
	m_pSimpleAudioEngine->preloadBackgroundMusic(pszFilePath);
	m_vBackgroundMusics.push_back(pszFilePath);
}

void AudioCtrl::playBackgroundMusicList(bool isRandom)
{
	int listSize = m_vBackgroundMusics.size();
	CS_RETURN_IF(listSize == 0);

	// stop the background music which is playing
	if (m_pSimpleAudioEngine->isBackgroundMusicPlaying())
	{
		m_pSimpleAudioEngine->stopBackgroundMusic();
	}

	// start play in list
	m_bIsListPlaying = true;
	m_bIsRandom = isRandom;
	if (m_bIsRandom)
	{
		m_nCurBackgroundMusicIndex = random(0, listSize - 1);
	}
	else
	{
		m_nCurBackgroundMusicIndex = 0;
	}
	m_pSimpleAudioEngine->playBackgroundMusic(m_vBackgroundMusics.at(m_nCurBackgroundMusicIndex).c_str(), false);
	auto director = Director::getInstance();
	director->getScheduler()->schedule(CC_CALLBACK_1(AudioCtrl::update, this), this, director->getAnimationInterval(), CC_REPEAT_FOREVER, 0, false, "update");
}

void AudioCtrl::stopBackgroundMusic()
{
	m_bIsListPlaying = false;
	m_bIsRandom = false;
	m_pSimpleAudioEngine->stopBackgroundMusic();
	Director::getInstance()->getScheduler()->unschedule("update", this);
}

void AudioCtrl::update(float dt)
{
	if (m_bIsListPlaying && !m_pSimpleAudioEngine->isBackgroundMusicPlaying())
	{
		if (m_bIsRandom)
		{
			m_nCurBackgroundMusicIndex = random(0, static_cast<int>(m_vBackgroundMusics.size()));
		}
		else
		{
			m_nCurBackgroundMusicIndex++;
		}
		m_pSimpleAudioEngine->playBackgroundMusic(m_vBackgroundMusics.at(m_nCurBackgroundMusicIndex).c_str(), false);
	}
}
NS_CSC_END
