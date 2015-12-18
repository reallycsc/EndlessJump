#pragma once
#include "CommonHeader.h"

typedef struct tagEnemyData
{
	Size	size;
	Vec2	position;
	Color3B	color;
}EnemyData;

typedef struct tagRoomData
{
	Size	size;
	Vec2	position;
	Color3B	color;
	vector<EnemyData>	enemysData;
}RoomData;

class GameLevelData : public Ref
{
public:
	GameLevelData(void);
	~GameLevelData(void);

	static GameLevelData* create() { return GameLevelData::create(NULL); }
	static GameLevelData* create(GameLevelData* gamedata);

	bool init(GameLevelData* gamedata);

	static void generateRandomLevel(int w, int h);

	// getter function
	list<RoomData>* getRoomsData() { return &m_lRoomsData; }

private:
	Size generateRandomEnemySize() const;

public:
	CC_SYNTHESIZE(int, m_nLevel, Level);

	CC_SYNTHESIZE(Color4F, m_playerColor, PlayerColor);
	CC_SYNTHESIZE(float, m_fPlayerSpeed, PlayerSpeed);
	CC_SYNTHESIZE(float, m_fPlayerJumpDuration, PlayerJumpDuration);

private:
	list<RoomData>	m_lRoomsData;
};

