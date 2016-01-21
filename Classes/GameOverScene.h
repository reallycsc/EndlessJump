#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "CommonHeader.h"

class GameOverScene : public Layer
{
public:
    static Scene* createScene(int deadCount);

    virtual bool init(int deadCount);
   
private:
	void buttonCallback_Next(Ref* pSender);
	void buttonCallback_UnlockNow(Ref* pSender);
	void buttonCallback_Retry(Ref* pSender);
	void buttonCallback_MainMenu(Ref* pSender);
};

#endif // __GAMEOVER_SCENE_H__
