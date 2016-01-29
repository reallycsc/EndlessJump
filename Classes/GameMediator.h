#pragma once
#include "CommonHeader.h"
#include "GameLevelData.h"

typedef struct tagLineData
{
	string	text;
	int		size;
	Color3B	color;
	int		interval;
}LineData;

typedef struct tagStoryData
{
	int	end;
	vector<LineData>	line_data;
}StoryData;

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
	bool loadGameLevelStoryFile();

	void setDeadCount(int deadCount);
	void setMaxGameLevel();
	void saveUserConfig();

	void gotoNextGameLevel();
	void gotoNextGameRoom();

	// getter function
	vector<GameLevelData*>* getGameLevelData() { return &m_vGameLevelData; }
	vector<int>* getLevelMinDeadCount() { return &m_vLevelMinDeadCount; }
	int getDeadCountAll(int level);
	StoryData* getLevelStoryLines(int level);
	map<string, vector<LineData>>* getEndStoryLines(int id);

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
	map<int, StoryData> m_mLevelStorys;
	map<int,map<string, vector<LineData>>> m_mEndStorys;
};


