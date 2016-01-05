#pragma once
#include "CommonHeader.h"

enum DrawNodeTag
{
	TAG_BLOCK = 1,
	TAG_TEXT_ID,
	TAG_SELECTED_BLOCK,
	TAG_PLAYER_BLOCK,
};

enum enemyType
{
	TYPE_NORMAL = 1,
	TYPE_ROTATE,
	TYPE_MOVE,
};

enum actionTag
{
	TAG_ACTION_ROTATE,
	TAG_ACTION_MOVE,
};

class Enemy :
	public Node
{
public:
	Enemy(void);
	~Enemy(void);

	static Enemy* create(const Size &size = Size(50, 50), const Color3B &color = Color3B::BLUE, const int &id = -1);
	bool init(const Size &size, const Color3B &color, const int &id);

	static Enemy* createRotate(const Size &size = Size(50, 50), const Color3B &color = Color3B::BLUE, const int &id = -1, 
		const float &duration = 1.0f);
	static Enemy* createMove(const Size &size = Size(50, 50), const Color3B &color = Color3B::BLUE, const int &id = -1, 
		const Point &start = Point::ZERO, const Point &dest = Point::ZERO, const float &duration = 1.0f);

	void updateId(const int &id);
	void updateSize(const Size &size);
	void updateColor(const Color3B &color);
	void updateRotateDuration(const float &duration);
	void updateStartPoint(const Point &start);
	void updateDestPoint(const Point &dest);
	void updateMoveDuration(const float &duration);

	void select();
	void unSelect();

	void addPlayerBlockForLevelMake(Size playerSize);
	void removePlayerBlockForLevelMake();

public:
	CC_SYNTHESIZE(int, m_nType, Type);

	CC_SYNTHESIZE(float, m_fRotateDuration, RotateDuration);

	CC_SYNTHESIZE(Point, m_StartPoint, StartPoint);
	CC_SYNTHESIZE(Point, m_DestPoint, DestPoint);
	CC_SYNTHESIZE(float, m_fMoveDuration, MoveDuration);

	CC_SYNTHESIZE(bool, m_bIsSelected, IsSelected);
	CC_SYNTHESIZE(bool, m_bIsPlayerAdded, IsPlayerAdded);

private:
	Size	m_size;
	Color4F	m_color;
};

