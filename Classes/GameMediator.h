#pragma once
#include "CommonHeader.h"
#include "GameLevelData.h"

class GameMediator
{
public:
	GameMediator();
	virtual ~GameMediator();
	bool init();
	// singlar
    static GameMediator* getInstance();

	void initGame();

	bool loadGameLevelFile();
	bool saveGameLevelFile();

	void reloadAllConfigFiles();

	void changeLanguageTo(LanguageType &type);

	// getter function
	map<string, string>* getGameText() { return &m_mGameText; }
	vector<GameLevelData*>* getGameLevelData() { return &m_vGameLevelData; }
    
private:
	bool loadGameTextFile();

public:
	CC_SYNTHESIZE(LanguageType, m_languageType, LanguageType);

	CC_SYNTHESIZE(int, m_nCurGameLevel, CurGameLevel);
	CC_SYNTHESIZE(int, m_nGameLevelMax, GameLevelMax);

private:
	vector<GameLevelData*>	m_vGameLevelData;
	map<string, string>	m_mGameText;
};


