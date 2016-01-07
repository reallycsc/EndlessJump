#ifndef __LEVELMAKE_SCENE_H__
#define __LEVELMAKE_SCENE_H__

#include "CommonHeader.h"
#include "Enemy.h"
#include "Player.h"
#include "CSCClass\CSCUI\DropDownList.h"

enum TextFieldTag
{
	TAG_LEVEL_MIN = 0,
	TAG_LEVEL_ID,
	TAG_LEVEL_NAME,
	TAG_LEVEL_MAX_DEADTIME,
	TAG_LEVEL_ROOM_ID,
	TAG_LEVEL_ROOM_WIDTH,
	TAG_LEVEL_ROOM_HEIGHT,
	TAG_LEVEL_ROOM_COLOR_R,
	TAG_LEVEL_ROOM_COLOR_G,
	TAG_LEVEL_ROOM_COLOR_B,
	TAG_LEVEL_MAX,

	TAG_ENEMY_MIN,
	TAG_ENEMY_COLOR_R,
	TAG_ENEMY_COLOR_G,
	TAG_ENEMY_COLOR_B,
	TAG_ENEMY_WIDTH,
	TAG_ENEMY_HEIGHT,
	TAG_ENEMY_POSITION_X,
	TAG_ENEMY_POSITION_Y,
	TAG_ENEMY_ROTATE_TIME10,
	TAG_ENEMY_DESTINATION_X,
	TAG_ENEMY_DESTINATION_Y,
	TAG_ENEMY_MOVE_TIME10,
	TAG_ENEMY_BLINK_TIME10,
	TAG_ENEMY_BLINKHIDE_TIME10,
	TAG_ENEMY_MAX,

	TAG_PLAYER_MIN,
	TAG_PLAYER_SPEED,
	TAG_PLAYER_JUMPTIME10,
	TAG_PLAYER_MAX,
};

enum NodeTag
{
	TAG_BACKGROUND = 1,
	TAG_FOOTHOLDLINE,
	TAG_TIMETEXT,
	TAG_ENEMY, // tag for all enemys. so this need to be last one
};

typedef struct tagTextFieldSliderBindInt
{
	TextField*	textField;
	Slider*	slider;
	int	number;
	int min;
	int max;
}TextFieldSliderBindInt;

typedef struct tagJumpPoints
{ // jump point for enemy from left to right due to position x: pEndStart->pStart->pEnd->pStartEnd
	float pEndStart;
	float pStart;
	float pEnd;
	float pStartEnd;
}JumpPoints;

// vector functions
inline bool sortJumpPoints(const JumpPoints &p1, const JumpPoints &p2)
{
	return p1.pEndStart < p2.pEndStart; // ascending
}

inline bool sortJumpPointsForAutoTrying(const pair<float, bool> &p1, const pair<float, bool> &p2)
{
	return p1.first < p2.first; // ascending
}

class LevelMakeScene : public Layer
{
public:
	LevelMakeScene();
	~LevelMakeScene();

    static Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LevelMakeScene);

private:
	void onEnter();
	void update(float dt);

	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);
	bool onContactBegin(const PhysicsContact& contact);

	void onTextFieldEvent(Ref *pSender, TextField::EventType type);
	void onSliderEvent(Ref *pSender, Slider::EventType type);
	void onDropDownList_BlockType(EventCustom* event);
	void onDropDownList_Level(EventCustom* event);
	void onDropDownList_Room(EventCustom* event);

	void buttonCallback_MainMenu(Ref* pSender);
	void buttonCallback_AddNewLevel(Ref* pSender);
	void buttonCallback_AddNewRoom(Ref* pSender);
	void buttonCallback_AddBlock(Ref* pSender);
	void buttonCallback_RemoveBlock(Ref* pSender);
	void buttonCallback_ShowHideDebug(Ref* pSender);
	void buttonCallback_Try(Ref* pSender);
	void buttonCallback_Save(Ref* pSender);
	void buttonCallback_Load(Ref* pSender);
	void buttonCallback_Export(Ref* pSender);
	void buttonCallback_Import(Ref* pSender);

	void addRoom();
	void addEnemy();
	void addPlayer();
	void addPlayerForTrying();
	void selectEnemy(Enemy* enemy);
	void updateLevelOrEnemys(int tag);
	void updateCurEnemy(int tag);
	void updatePlayer(int tag);
	void calcFoothold();

	void loadDataFrom(int level, int room);
	void saveDataTo(int level, int room);

	void setDropDownList(vector<GameLevelData*>* levelsData);
	bool isJumpLineConflict(const Vec2 &origin, const Vec2 &control, const Vec2 &destination);

	inline void getLevelNode(Node* root, int tag, bool isWithSlider = true);
	static inline void initStruct(TextFieldSliderBindInt* structTmp, int min, int number, int max);
	inline void updateBlockTextFieldNumber(int tag, int number);
	inline void updateBlockTextFieldMax(int tag, int max);
	inline void updateBlockByLevel(int tag, int number);
	inline void updateBlockType(int type);
	inline void drawJumpLine(DrawNode* draw, const Vec2 &origin, const Vec2 &control, const Vec2 &destination, const Color4F &color);
	inline void AddEstimateFrameText(Node* parent, float p1, float p2,
		int frameRate, float playerWidth, float speed, float posY, Color4B color) const;
	inline void setTextFieldStructEnable(int tag, bool isEnable);

public:
	CC_SYNTHESIZE(PhysicsWorld*, m_pWorld, World);

private:
	map<int, tagTextFieldSliderBindInt> m_mTextFieldStructs;
	TextField*	m_pTextFieldLevelName;
	string m_sLevelName;

	CSCClass::DropDownList* m_pDropDownListLevel;
	CSCClass::DropDownList* m_pDropDownListRoom;
	CSCClass::DropDownList* m_pDropDownListType;

	Vector<Enemy*>	m_vEnemys;
	Player*	m_pPlayer;

	vector<JumpPoints>	m_vJumpPoints;
	vector<pair<float, bool>>	m_vJumpPointsForAutoTrying;
	vector<Point>	m_vAirPoints;

	int m_nCurEnemyId;
	int m_nCurJumpPointId;

	bool m_bIsAutoTrying;
	bool m_bIsTouchEnemy;
	Vec2 m_touchOffset;
};

#endif // __LEVELMAKE_SCENE_H__
