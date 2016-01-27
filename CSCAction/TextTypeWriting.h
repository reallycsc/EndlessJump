#pragma once
#include "../CommonDefinition.h"

NS_CSC_BEGIN
class TextTypeWriting : public ActionInterval
{
public:
	TextTypeWriting();

public:
	/** creates the action */
	static TextTypeWriting* create(float duration);

	/** initializes the action */
	bool initWithDuration(float duration);

	virtual void startWithTarget(Node *pTarget);
	virtual ActionInterval* reverse(void);
	virtual void update(float time);

protected:
	string m_sText;
};
NS_CSC_END