#pragma once
#include "CommonHeader.h"

enum DrawNodeTag
{
	TAG_BLOCK = 1,
	TAG_SELECTED_BLOCK,
	TAG_PLAYER_BLOCK,
};

class Enemy :
	public Node
{
public:
	Enemy(void);
	~Enemy(void);

	static Enemy* create(const Size &size = Size(50, 50), const Color3B &color = Color3B::BLUE, const int &id = -1);
	bool init(const Size &size, const Color3B &color, const int &id);

	void updateSize(const Size &size);
	void updateColor(const Color3B& color);

	void select();
	void unSelect();

	void addPlayerBlockForLevelMake(Size playerSize);
	void removePlayerBlockForLevelMake();

public:
	CC_SYNTHESIZE(bool, m_bIsSelected, IsSelected);
	CC_SYNTHESIZE(bool, m_bIsPlayerAdded, IsPlayerAdded);

private:
	Size	m_size;
	Color4F	m_color;
};

