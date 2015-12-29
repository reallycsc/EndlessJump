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

	bool loadGameLevelFile();
	bool saveGameLevelFile();

	void saveGameData();

	// getter function
	vector<GameLevelData*>* getGameLevelData() { return &m_vGameLevelData; }

public:
	CC_SYNTHESIZE(int, m_nMaxGameLevel, MaxGameLevel);
	CC_SYNTHESIZE(int, m_nCurGameLevel, CurGameLevel);

private:
	vector<GameLevelData*>	m_vGameLevelData;
};


