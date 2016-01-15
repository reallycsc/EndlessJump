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
	void buttonCallback_LevelPlay(Ref* pSender);
	void buttonCallback_LevelEditor(Ref* pSender);

	void schedule_changeColor(float dt);

private:
	LayerColor*	m_pColorLayer;
	vector<pair<Color3B,Color3B>>	m_vLevelColors;
	Vector<Text*>	m_vLevelDeadCount;
};

#endif // __MAINMENU_SCENE_H__
