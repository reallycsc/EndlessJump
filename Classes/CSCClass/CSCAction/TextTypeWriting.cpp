#include "TextTypeWriting.h"

NS_CSC_BEGIN
TextTypeWriting::TextTypeWriting()
{
}

TextTypeWriting* TextTypeWriting::create(float duration)
{
	TextTypeWriting* pRet = new(std::nothrow) TextTypeWriting();
	pRet->initWithDuration(duration);
	pRet->autorelease();
	return pRet;
}

bool TextTypeWriting::initWithDuration(float duration)
{
	if (ActionInterval::initWithDuration(duration))
	{
		return true;
	}

	return false;
}

void TextTypeWriting::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);

	Label *pText = dynamic_cast<Label*>(_target);
	if (pText)
	{
		m_sText = pText->getString();
		pText->setString("");
	}
}

ActionInterval* TextTypeWriting::reverse(void)
{
	return TextTypeWriting::create(_duration);
}


void TextTypeWriting::update(float t)
{
	Label *pText = dynamic_cast<Label*>(_target);
	if (pText)
	{
		auto substr = m_sText.substr(0, m_sText.size() * t);
		pText->setString(substr);
	}
}
NS_CSC_END