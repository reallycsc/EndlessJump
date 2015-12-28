#pragma once
#include "CommonHeader.h"

typedef struct tagEnemyData
{
	int		id;
	Size	size;
	Vec2	position;
}EnemyData;

typedef struct tagRoomData
{
	int		id;
	Size	size;
	Vec2	position;
	Color3B	color;
	float	player_speed;
	float	player_jumpTime;
	Color3B	player_color;
	Color3B	enemy_color;
	vector<EnemyData>	enemysData;
}RoomData;

class GameLevelData : public Ref
{
public:
	GameLevelData(void);
	~GameLevelData(void);

	static GameLevelData* create() { return GameLevelData::create(nullptr); }
	static GameLevelData* create(GameLevelData* gamedata);

	bool init(GameLevelData* gamedata);

	// seeter function
	bool setRoomDataWithFile(XMLElement* surface);
	// getter function
	vector<RoomData>* getRoomsData() { return &m_vRoomsData; }

public:
	CC_SYNTHESIZE(int, m_nLevel, Level);

private:
	vector<RoomData>	m_vRoomsData;
};

