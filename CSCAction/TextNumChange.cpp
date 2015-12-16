#include "TextNumChange.h"

TextNumChange::TextNumChange() :
	m_nFromNum(0),
	m_nToNum(0)
{

}

TextNumChange* TextNumChange::create(float duration, int fromNum, int toNum)
{
	TextNumChange* pRet = new(std::nothrow) TextNumChange();
	if (pRet && pRet->initWithDuration(duration, fromNum, toNum))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool TextNumChange::initWithDuration(float duration, int fromNum, int toNum)
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_nFromNum = fromNum;
		m_nToNum = toNum;
		return true;
	}

	return false;
}

void TextNumChange::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);

	Text *pText = dynamic_cast<Text*>(_target);
	if (pText)
	{
		pText->setString(StringUtils::format("%d", m_nFromNum));
	}

}

ActionInterval* TextNumChange::reverse(void)
{
	return TextNumChange::create(_duration, m_nToNum, m_nFromNum);
}


void TextNumChange::update(float t)
{

	Text *pText = dynamic_cast<Text*>(_target);
	if (pText)
	{
		int num = m_nFromNum + (m_nToNum - m_nFromNum) * t;
		pText->setString(StringUtils::format("%d", num));
	}

}