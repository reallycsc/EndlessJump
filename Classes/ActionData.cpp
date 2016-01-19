#include "ActionData.h"

/*===============================Base Class===============================*/
ActionData::ActionData(void): m_nType(TYPE_NORMAL), m_fDelay(0), m_fDuration(0), m_fPostDelay(0), m_bIsRepeat(false), m_bIsReverse(false), m_pAction(nullptr)
{
}

ActionData::~ActionData(void)
{
	if (m_pAction)
	{
		m_pAction->release();
		m_pAction = nullptr;
	}
}

/*===============================Move Action===============================*/
MoveActionData::MoveActionData(void)
{
}

MoveActionData::~MoveActionData(void)
{
}

MoveActionData* MoveActionData::create(const float &delay, const float &duration, const float &postDelay, const Point &start, const Point &destination)
{
	MoveActionData* pRet = new(std::nothrow) MoveActionData();
	pRet->init(delay, duration, postDelay, start, destination);
	pRet->autorelease();
	return pRet;
}

bool MoveActionData::init(const float &delay, const float &duration, const float &postDelay, const Point &start, const Point &destination)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_MOVE;
		m_fDelay = delay;
		m_fDuration = duration;
		m_fPostDelay = postDelay;
		m_Start = start;
		m_Destination = destination;
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setPosition(m_Start); }),
			DelayTime::create(m_fDelay),
			MoveTo::create(m_fDuration, m_Destination),
			DelayTime::create(m_fPostDelay),
			NULL);
		m_pAction->setTag(m_nType);
		m_pAction->retain();
		bRet = true;
	} while (0);

	return bRet;
}

void MoveActionData::updateAction()
{
	m_pAction->release();
	m_pAction = nullptr;
	if (m_bIsReverse)
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setPosition(m_Start); }),
			DelayTime::create(m_fDelay),
			MoveTo::create(m_fDuration, m_Destination),
			MoveTo::create(m_fDuration, m_Start),
			DelayTime::create(m_fPostDelay),
			NULL);
	else
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setPosition(m_Start); }),
			DelayTime::create(m_fDelay),
			MoveTo::create(m_fDuration, m_Destination),
			DelayTime::create(m_fPostDelay),
			NULL);
	if (m_bIsRepeat)
		m_pAction = RepeatForever::create(m_pAction);
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

MoveActionData* MoveActionData::clone() const
{    
	// no copy constructor
	auto a = new (std::nothrow) MoveActionData();
	a->init(m_fDelay, m_fDuration, m_fPostDelay, m_Start, m_Destination);
	a->setIsRepeat(m_bIsRepeat);
	a->setIsReverse(m_bIsReverse);
	a->updateAction();
	a->autorelease();
	return a;
}

/*===============================Rotate Action===============================*/
RotateActionData::RotateActionData(void): m_fAngle(0)
{
}

RotateActionData::~RotateActionData(void)
{
}

RotateActionData* RotateActionData::create(const float &delay, const float &duration, const float &postDelay, const float &angle, const Vec2 &anchor)
{
	RotateActionData* pRet = new(std::nothrow) RotateActionData();
	pRet->init(delay, duration, postDelay, angle, anchor);
	pRet->autorelease();
	return pRet;
}

bool RotateActionData::init(const float &delay, const float &duration, const float &postDelay, const float &angle, const Vec2 &anchor)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_ROTATE;
		m_fDelay = delay;
		m_fDuration = duration;
		m_fPostDelay = postDelay;
		m_fAngle = angle;
		m_Anchor = anchor;
		if (m_fAngle >= 360 || m_fAngle <= -360)
			m_pAction = Sequence::create(
				CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
				DelayTime::create(m_fDelay),
				RotateBy::create(m_fDuration, m_fAngle),
				DelayTime::create(m_fPostDelay),
				NULL);
		else
			m_pAction = Sequence::create(
				CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
				DelayTime::create(m_fDelay),
				RotateBy::create(m_fDuration, m_fAngle),
				DelayTime::create(m_fPostDelay),
				NULL);
		m_pAction->setTag(m_nType);
		m_pAction->retain();
		bRet = true;
	} while (0);

	return bRet;
}

void RotateActionData::updateAction()
{
	m_pAction->release();
	m_pAction = nullptr;

	if (m_fAngle >= 360 || m_fAngle <= -360)
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
			DelayTime::create(m_fDelay),
			RotateBy::create(m_fDuration, m_fAngle),
			DelayTime::create(m_fPostDelay),
			NULL);
	else if (m_bIsReverse)
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
			DelayTime::create(m_fDelay),
			RotateBy::create(m_fDuration, m_fAngle),
			RotateBy::create(m_fDuration, -m_fAngle),
			DelayTime::create(m_fPostDelay),
			NULL);
	else
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
			DelayTime::create(m_fDelay),
			RotateBy::create(m_fDuration, m_fAngle),
			DelayTime::create(m_fPostDelay),
			NULL);
	if (m_bIsRepeat)
		m_pAction = RepeatForever::create(m_pAction);
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

RotateActionData* RotateActionData::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) RotateActionData();
	a->init(m_fDelay, m_fDuration, m_fPostDelay, m_fAngle, m_Anchor);
	a->setIsRepeat(m_bIsRepeat);
	a->setIsReverse(m_bIsReverse);
	a->updateAction();
	a->autorelease();
	return a;
}

/*===============================Blink Action===============================*/
BlinkActionData::BlinkActionData(void)
{
}

BlinkActionData::~BlinkActionData(void)
{
}

BlinkActionData* BlinkActionData::create(const float &delay, const float &duration, const float &postDelay)
{
	BlinkActionData* pRet = new(std::nothrow) BlinkActionData();
	pRet->init(delay, duration, postDelay);
	pRet->autorelease();
	return pRet;
}

bool BlinkActionData::init(const float &delay, const float &duration, const float &postDelay)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_BLINK;
		m_fDelay = delay;
		m_fDuration = duration;
		m_fPostDelay = postDelay;
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setVisible(false); }),
			DelayTime::create(m_fDelay),
			CallFunc::create([=] {m_pAction->getTarget()->setVisible(true); }),
			DelayTime::create(m_fDuration),
			CallFunc::create([=] {m_pAction->getTarget()->setVisible(false); }),
			DelayTime::create(m_fPostDelay),
			NULL);
		m_pAction->setTag(m_nType);
		m_pAction->retain();
		bRet = true;
	} while (0);

	return bRet;
}

void BlinkActionData::updateAction()
{
	m_pAction->release();
	m_pAction = nullptr;
	m_pAction = Sequence::create(
		CallFunc::create([=] {m_pAction->getTarget()->setVisible(false); }),
		DelayTime::create(m_fDelay),
		CallFunc::create([=] {m_pAction->getTarget()->setVisible(true); }),
		DelayTime::create(m_fDuration),
		CallFunc::create([=] {m_pAction->getTarget()->setVisible(false); }),
		DelayTime::create(m_fPostDelay),
		NULL);
	if (m_bIsRepeat)
		m_pAction = RepeatForever::create(m_pAction);
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

BlinkActionData* BlinkActionData::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) BlinkActionData();
	a->init(m_fDelay, m_fDuration, m_fPostDelay);
	a->setIsRepeat(m_bIsRepeat);
	a->setIsReverse(m_bIsReverse);
	a->updateAction();
	a->autorelease();
	return a;
}

/*===============================Scale Action===============================*/
ScaleActionData::ScaleActionData(void): m_fScaleX(0), m_fScaleY(0)
{
}

ScaleActionData::~ScaleActionData(void)
{
}

ScaleActionData* ScaleActionData::create(const float &delay, const float &duration, const float &postDelay, const float &scaleX, const float &scaleY, const Vec2 &anchor)
{
	ScaleActionData* pRet = new(std::nothrow) ScaleActionData();
	pRet->init(delay, duration, postDelay, scaleX, scaleY, anchor);
	pRet->autorelease();
	return pRet;
}

bool ScaleActionData::init(const float &delay, const float &duration, const float &postDelay, const float &scaleX, const float &scaleY, const Vec2 &anchor)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_SCALE;
		m_fDelay = delay;
		m_fDuration = duration;
		m_fPostDelay = postDelay;
		m_fScaleX = scaleX;
		m_fScaleY = scaleY;
		m_Anchor = anchor;
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setScale(1, 1); }),
			DelayTime::create(m_fDelay),
			ScaleTo::create(m_fDuration, m_fScaleX, m_fScaleY),
			DelayTime::create(m_fPostDelay),
			NULL);
		m_pAction->setTag(m_nType);
		m_pAction->retain();
		bRet = true;
	} while (0);

	return bRet;
}

void ScaleActionData::updateAction()
{
	m_pAction->release();
	m_pAction = nullptr;
	if (m_bIsReverse)
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setScale(1, 1); }),
			DelayTime::create(m_fDelay),
			ScaleTo::create(m_fDuration, m_fScaleX, m_fScaleY),
			ScaleTo::create(m_fDuration, 1, 1),
			DelayTime::create(m_fPostDelay),
			NULL);
	else
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setScale(1, 1); }),
			DelayTime::create(m_fDelay),
			ScaleTo::create(m_fDuration, m_fScaleX, m_fScaleY),
			DelayTime::create(m_fPostDelay),
			NULL);
	if (m_bIsRepeat)
		m_pAction = RepeatForever::create(m_pAction);
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

ScaleActionData* ScaleActionData::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) ScaleActionData();
	a->init(m_fDelay, m_fDuration, m_fPostDelay, m_fScaleX, m_fScaleY, m_Anchor);
	a->setIsRepeat(m_bIsRepeat);
	a->setIsReverse(m_bIsReverse);
	a->updateAction();
	a->autorelease();
	return a;
}
