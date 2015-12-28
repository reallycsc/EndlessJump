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

inline bool sortJumpPointsForAutoTrying(const float &p1, const float &p2)  
{
	return p1 < p2; // ascending
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
	virtual bool onTouchBegan(Touch* touch, Event* event);
	virtual void onTouchMoved(Touch* touch, Event* event);
	virtual void onTouchEnded(Touch* touch, Event* event);

	void onTextFieldEvent(Ref *pSender, TextField::EventType type);
	void onSliderEvent(Ref *pSender, Slider::EventType type);
	bool onContactBegin(const PhysicsContact& contact);

	void update(float dt);

	void buttonCallback_AddBlock(Ref* pSender);
	void buttonCallback_RemoveBlock(Ref* pSender);
	void buttonCallback_CalcFoothold(Ref* pSender);
	void buttonCallback_Try(Ref* pSender);
	void buttonCallback_Save(Ref* pSender);
	void buttonCallback_Load(Ref* pSender);
	void buttonCallback_Export(Ref* pSender);
	void buttonCallback_Import(Ref* pSender);

	void addRoom();
	void addEnemy();
	void addPlayer();
	void addPlayerForTrying();
	void updateLevelOrEnemys(int tag);
	void updateCurEnemy(int tag);
	void updatePlayer(int tag);
	void calcFoothold();

	void loadDataFrom(int level, int room);
	void saveDataTo(int level, int room);

	inline void getLevelNode(Node* root, int tag, bool isWithSlider = true);
	static inline void initStruct(TextFieldSliderBindInt* structTmp, int min, int number, int max);
	inline void updateBlockTextFieldNumber(int tag, int number);
	inline void updateBlockTextFieldMax(int tag, int max);
	inline void updateBlockByLevel(int tag, int number);
	static inline void drawJumpLineByStart(DrawNode* draw, Vec2 start, Vec2 ctlOffset, Vec2 endOffset, Color4F color);
	static inline void drawJumpLineByEnd(DrawNode* draw, Vec2 end, Vec2 ctlOffset, Vec2 endOffset, Color4F color);
	inline void AddEstimateFrameText(Node* parent, char* buf, float p1, float p2,
		int frameRate, float playerWidth, float speed, float posY, Color4B color) const;
private:
	map<int, tagTextFieldSliderBindInt> m_mTextFieldStructs;
	CSCClass::DropDownList* m_pDropDownListLevel;
	CSCClass::DropDownList* m_pDropDownListRoom;

	Vector<Enemy*>	m_vEnemys;
	Player*	m_pPlayer;
	vector<JumpPoints>	m_vJumpPoints;
	vector<float>	m_vJumpPointsForAutoTrying;

	int m_nCurEnemyId;
	int m_nCurJumpPointId;

	bool m_bIsTrying;
	bool m_bIsTouchEnemy;
	Vec2 m_touchOffset;
};

#endif // __LEVELMAKE_SCENE_H__
