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
	int condition; // the condition of dead_count for show story
	vector<LineData>	line_data;
}StoryData;

enum UserData
{
	CURRENT_LEVEL = 1,
	MAX_LEVEL,
	TOTAL_DEAD,
	LEVEL_DEAD_START,
};

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
	inline void saveDataForKey(const int &key, const int &data);

	void gotoNextGameLevel();
	void gotoNextGameRoom();

	void encodeAllFiles();

	// getter function
	vector<GameLevelData*>* getGameLevelData() { return &m_vGameLevelData; }
	vector<int>* getLevelMinDeadCount() { return &m_vLevelMinDeadCount; }
	int getDeadCountAll(int level);
	StoryData* getLevelStoryLines(int level);
	map<string, vector<LineData>>* getEndStoryLines(int id);

private:
	inline void addCustomEventLisenter(const string &suffix, int* pScore, const int &key);
	inline int loadDataForKey(const int &key, const int &default_data);

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

	int m_nFirstStoryMaxGameLevel;
};


