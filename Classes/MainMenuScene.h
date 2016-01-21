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
	void update(float dt);

	void buttonCallback_LevelPlay(Ref* pSender);
	void buttonCallback_LevelEditor(Ref* pSender);
	void buttonCallback_Credit(Ref* pSender);
	void buttonCallback_Mute(Ref* pSender);

	inline LayerGradient* createLayerColor();
private:
	LayerGradient*	m_pLayerColor1;
	LayerGradient*	m_pLayerColor2;
	vector<pair<Color3B,Color3B>>	m_vLevelColors;

	int m_nCurColorIndex;
	float	m_fLayerColorSpeed;
};

#endif // __MAINMENU_SCENE_H__
