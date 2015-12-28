#pragma once
#include "../CommonDefinition.h"

NS_CSC_BEGIN
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
NS_CSC_END