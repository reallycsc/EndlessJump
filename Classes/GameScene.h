#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "CommonHeader.h"
#include "GameMediator.h"
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

	void buttonCallback_MainMenu(Ref* pSender);

	void setPhyWorld(PhysicsWorld* world) { m_pWorld = world; }

	void addRoom(RoomData* roomData);
	void addPlayer(RoomData* roomData);

private:
	PhysicsWorld*	m_pWorld;
	GameMediator*	m_pGameMediator;

	GameLevelData*	m_pCurLevelData;
	RoomData*	m_pCurRoomData;

	Player*	m_pPlayer;
	Vector<Enemy*>	m_vEnemys;

	Text*	m_pTextDeadNum;
	int	m_nDeadNumber;
};

#endif // __GAME_SCENE_H__