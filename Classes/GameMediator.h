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

	void setDeadCount(int deadCount);
	void setMaxGameLevel();
	void gotoNextGameLevel();
	void gotoNextGameRoom();

	// getter function
	vector<GameLevelData*>* getGameLevelData() { return &m_vGameLevelData; }
	vector<int>* getLevelMinDeadCount() { return &m_vLevelMinDeadCount; }
	int getDeadCountAll(int level);

private:
	inline void addCustomEventLisenter(const string suffix, int* pScore);

public:
	CC_SYNTHESIZE(int, m_nGameLevelCount, GameLevelCount);
	CC_SYNTHESIZE(int, m_nCurGameLevel, CurGameLevel);
	CC_SYNTHESIZE(int, m_nMaxGameLevel, MaxGameLevel);
	CC_SYNTHESIZE(int, m_nCurGameRoom, CurGameRoom);
	CC_SYNTHESIZE(int, m_nTotalDeadCount, TotalDeadCount);

private:
	vector<GameLevelData*>	m_vGameLevelData;
	vector<int>	m_vLevelMinDeadCount;
};


