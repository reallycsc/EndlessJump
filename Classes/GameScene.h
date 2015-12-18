#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "CommonHeader.h"
#include "GameLevelData.h"
#include "Player.h"
#include "Enemy.h"

class GameScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

private:
	void onEnter() override;
	virtual bool onTouchBegan(Touch* touch, Event* event) override;
	virtual void onTouchMoved(Touch* touch, Event* event) override;
	virtual void onTouchEnded(Touch* touch, Event* event) override;
	bool onContactBegin(const PhysicsContact& contact);

	void update(float dt) override;

	void setPhyWorld(PhysicsWorld* world) { m_world = world; }
	void addPlayer();

	void addMaximumEnemy_Debug();

	
private:
	PhysicsWorld* m_world;

	GameLevelData*	m_pLevelData;
	Player*	m_pPlayer;
	Vector<Enemy*>	m_vEnemys;

	list<RoomData>::const_iterator	m_iterCurRoom;
};

#endif // __GAME_SCENE_H__
