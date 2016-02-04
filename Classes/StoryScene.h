#ifndef __STORY_SCENE_H__
#define __STORY_SCENE_H__

#include "CommonHeader.h"
#include "GameMediator.h"

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

	void showStoryline(Node* pSender, vector<LineData>* storylines, int index);

private:
	bool	m_bIsFinished;
	float	m_fPositionY;
};

#endif // __STORY_SCENE_H__
