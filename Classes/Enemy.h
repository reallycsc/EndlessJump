#pragma once
#include "CommonHeader.h"
#include "ActionData.h"

enum DrawNodeTag
{
	TAG_BLOCK = 1,
	TAG_TEXT_ID,
	TAG_SELECTED_BLOCK,
	TAG_PLAYER_BLOCK,
};

enum actionTag
{
	TAG_ACTION_MOVE,
	TAG_ACTION_ROTATE,
	TAG_ACTION_BLINK,
};

class Enemy :
	public Node
{
public:
	Enemy(void);
	~Enemy(void);

	static Enemy* create(const Size &size = Size(50, 50), const Color3B &color = Color3B::BLUE, const int &id = -1, const int &room_id = 1);
	bool init(const Size &size, const Color3B &color, const int &id, const int &room_id);

	bool addAction(ActionData* action_data);
	bool removeAction(ActionData* action_data);
	void updateAction(ActionData* action_data);
	void restartActions();

	void updateId(const int &id);
	void updateSize(const Size &size);
	void updateColor(const Color3B &color);
	// TYPE_MOVE
	void updateStartPoint(const Point &start);

	void select();
	void unSelect();

	void addPlayerBlockForLevelMake(Size playerSize);
	void removePlayerBlockForLevelMake();

	// setter & getter function
	Point getStartPoint();
	void setActions(Vector<ActionData*>* actions);
	Vector<ActionData*>* getActions() { return &m_vActions; }

private:
	inline void prepareForAction(ActionData* action_data);

public:
	CC_SYNTHESIZE(int, m_nID, ID);
	CC_SYNTHESIZE(int, m_nRoomID, RoomID);
	
	CC_SYNTHESIZE(bool, m_bIsHaveMoveAction, IsHaveMoveAction);
	CC_SYNTHESIZE(bool, m_bIsSelected, IsSelected);
	CC_SYNTHESIZE(bool, m_bIsPlayerAdded, IsPlayerAdded);

private:
	Size	m_size;
	Color4F	m_color;
	Vector<ActionData*>	m_vActions;
};

