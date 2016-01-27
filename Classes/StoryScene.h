#ifndef __STORY_SCENE_H__
#define __STORY_SCENE_H__

#include "CommonHeader.h"

const int FONT_SIZE = 32;
const float WORD_DISPLAY_SPEED = 0.08f;

class StoryScene : public Layer
{
public:
    static Scene* createScene();

	// implement the "static create()" method manually
	CREATE_FUNC(StoryScene);

	virtual bool init() override;

private:
	virtual bool onTouchBegan(Touch* touch, Event* event) override;
	virtual void onTouchMoved(Touch* touch, Event* event) override;
	virtual void onTouchEnded(Touch* touch, Event* event) override;

	void showStoryline(Node* pSender, int index);

private:
	bool	m_bIsFinished;

	vector<string>* m_pvStorylines;
};

#endif // __STORY_SCENE_H__
