#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "CommonHeader.h"

class MainMenuScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);

private:
	void buttonCallback_Play(Ref* pSender);
	void buttonCallback_LevelEditor(Ref* pSender);
};

#endif // __MAINMENU_SCENE_H__
