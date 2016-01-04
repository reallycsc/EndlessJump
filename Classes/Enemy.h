#pragma once
#include "CommonHeader.h"

enum DrawNodeTag
{
	TAG_BLOCK = 1,
	TAG_SELECTED_BLOCK,
	TAG_PLAYER_BLOCK,
};

enum enemyType
{
	TYPE_NORMAL = 1,
	TYPE_ROTATE,
};

enum actionTag
{
	TAG_ACTION_ROTATE,
};

class Enemy :
	public Node
{
public:
	Enemy(void);
	~Enemy(void);

	static Enemy* create(const Size &size = Size(50, 50), const Color3B &color = Color3B::BLUE, const int &id = -1);
	bool init(const Size &size, const Color3B &color, const int &id);

	static Enemy* createWithType(const int type = TYPE_NORMAL, const Size &size = Size(50, 50), const Color3B &color = Color3B::BLUE, const int &id = -1,
		const float rotateDuration = 1.0f);

	void updateSize(const Size &size);
	void updateColor(const Color3B& color);
	void updateRotateDuration(const float duration);

	void select();
	void unSelect();

	void addPlayerBlockForLevelMake(Size playerSize);
	void removePlayerBlockForLevelMake();

public:
	CC_SYNTHESIZE(int, m_nType, Type);

	CC_SYNTHESIZE(float, m_fRotateDuration, RotateDuration);

	CC_SYNTHESIZE(bool, m_bIsSelected, IsSelected);
	CC_SYNTHESIZE(bool, m_bIsPlayerAdded, IsPlayerAdded);

private:
	Size	m_size;
	Color4F	m_color;
};

