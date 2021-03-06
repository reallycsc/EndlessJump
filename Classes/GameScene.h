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
	void update(float dt) override;

	void onEnter() override;
	void onSceneAnimateEnd();
	virtual bool onTouchBegan(Touch* touch, Event* event) override;
	virtual void onTouchMoved(Touch* touch, Event* event) override;
	virtual void onTouchEnded(Touch* touch, Event* event) override;
	bool onContactBegin(const PhysicsContact& contact);

	void buttonCallback_Pause(Ref* pSender);
	void buttonCallback_Retry(Ref* pSender);

	void addRoom(RoomData* roomData);
	void resetRoom(int id);
	void addPlayer();

private:
	GameMediator*	m_pGameMediator;

	GameLevelData*	m_pCurLevelData;
	RoomData*	m_pCurRoomData;
	int	m_nRoomCount;

	Player*	m_pPlayer;
	Vector<Enemy*>	m_vEnemys;

	LayerColor*	m_pBlackLayer;
	Button*	m_pButtonPause;
	Button*	m_pButtonRetry;
	Text*	m_pTextDeadNum;
	int	m_nDeadNumber;
	int m_nDeadNumberMin;

	ParticleMeteor*	m_pParticleTail;
	Point	m_LastJumpPoint;

	bool	m_bIsLevelFinished;
};

#endif // __GAME_SCENE_H__
