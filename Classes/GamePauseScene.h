#ifndef __GAMEPAUSE_SCENE_H__
#define __GAMEPAUSE_SCENE_H__

#include "CommonHeader.h"

class GamePauseScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GamePauseScene);

private:
	void buttonCallback_Resume(Ref* pSender);
	void buttonCallback_MainMenu(Ref* pSender);

	void onFrameEvent(Frame* frame) const;

private:
	ActionTimeline*	m_pAnimate;
	Layout*	m_pLayout;
	Text*	m_pTextCountdown;
};

#endif // __GAMEPAUSE_SCENE_H__
