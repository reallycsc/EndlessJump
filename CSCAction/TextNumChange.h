#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocostudio::timeline;
using namespace ui;

class TextNumChange : public ActionInterval
{
public:
	TextNumChange();

public:
	/** creates the action */
	static TextNumChange* create(float duration, int fromNum, int toNum);

	/** initializes the action */
	bool initWithDuration(float duration, int fromNum, int toNum);

	virtual void startWithTarget(Node *pTarget);
	virtual ActionInterval* reverse(void);
	virtual void update(float time);

protected:
	int m_nFromNum;
	int m_nToNum;
};