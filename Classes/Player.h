#pragma once
#include "CommonHeader.h"
#include "GameLevelData.h"

enum ActionTags {
	ACTIONTAG_JUMP = 0,
};

class Player :
	public Node
{
public:
	Player(void);
	~Player(void);

	static Player* create(const Color3B &color = Color3B::BLUE, GameLevelData* levelData = NULL);
	bool init(const Color3B &color = Color3B::BLUE, GameLevelData* levelData = NULL);

	void jump();
	void die();

public:
	CC_SYNTHESIZE(Color4F, m_playerColor, PlayerColor);
	CC_SYNTHESIZE(float, m_fSpeed, Speed);
	CC_SYNTHESIZE(float, m_fJumpHeight, JumpHeight);
	CC_SYNTHESIZE(float, m_fJumpDuration, JumpDuration);
};

