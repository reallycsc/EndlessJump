#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "CommonHeader.h"
#include "GameMediator.h"

class GameOverScene : public Layer
{
public:
    static Scene* createScene(int deadNum);

    virtual bool init(int deadNum);
   
private:
	void buttonCallback_Next(Ref* pSender);
	void buttonCallback_Retry(Ref* pSender);
	void buttonCallback_MainMenu(Ref* pSender);

private:
	GameMediator*	m_pGameMediator;

};

#endif // __GAMEOVER_SCENE_H__
