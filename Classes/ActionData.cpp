#include "ActionData.h"
#include "GameLevelData.h"

/*===============================Base Class===============================*/
ActionData::ActionData(void)
	: m_nType(TYPE_NORMAL)
	, m_fDelay(0.0f)
	, m_pAction(nullptr)
	, m_bIsRepeat(false)
	, m_bIsReverse(false)
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
	: m_fDuration(0.0f)
	, m_Start(Point::ZERO)
	, m_Destination(Point::ZERO)
{
}

MoveActionData::~MoveActionData(void)
{
}

MoveActionData* MoveActionData::create(const float &delay, const float &duration, const Point &start, const Point &destination)
{
	MoveActionData* pRet = new(std::nothrow) MoveActionData();
	pRet->init(delay, duration, start, destination);
	pRet->autorelease();
	return pRet;
}

bool MoveActionData::init(const float &delay, const float &duration, const Point &start, const Point &destination)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_MOVE;
		m_fDelay = delay;
		m_fDuration = duration;
		m_Start = start;
		m_Destination = destination;
		m_pAction = Sequence::create(
			DelayTime::create(m_fDelay),
			CallFunc::create([=] {m_pAction->getTarget()->setPosition(m_Start); }),
			MoveTo::create(m_fDuration, m_Destination),
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
			NULL);
	else
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setPosition(m_Start); }),
			DelayTime::create(m_fDelay),
			MoveTo::create(m_fDuration, m_Destination),
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
	a->init(m_fDelay, m_fDuration, m_Start, m_Destination);
	a->setIsRepeat(m_bIsRepeat);
	a->setIsReverse(m_bIsReverse);
	a->updateAction();
	a->autorelease();
	return a;
}

/*===============================Rotate Action===============================*/
RotateActionData::RotateActionData(void)
	: m_fDuration(0.0f)
	, m_fAngle(0.0f)
	, m_Anchor(Vec2::ZERO)
{
}

RotateActionData::~RotateActionData(void)
{
}

RotateActionData* RotateActionData::create(const float &delay, const float &duration, const float &angle, const Vec2 &anchor)
{
	RotateActionData* pRet = new(std::nothrow) RotateActionData();
	pRet->init(delay, duration, angle, anchor);
	pRet->autorelease();
	return pRet;
}

bool RotateActionData::init(const float &delay, const float &duration, const float &angle, const Vec2 &anchor)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_ROTATE;
		m_fDelay = delay;
		m_fDuration = duration;
		m_fAngle = angle;
		m_Anchor = anchor;
		if (m_fAngle >= 360 || m_fAngle <= -360)
			m_pAction = Sequence::create(
				CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
				DelayTime::create(m_fDelay),
				RotateBy::create(m_fDuration, m_fAngle),
				NULL);
		else
			m_pAction = Sequence::create(
				DelayTime::create(m_fDelay),
				CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
				RotateBy::create(m_fDuration, m_fAngle),
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
			NULL);
	else if (m_bIsReverse)
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
			DelayTime::create(m_fDelay),
			RotateBy::create(m_fDuration, m_fAngle),
			RotateBy::create(m_fDuration, -m_fAngle),
			NULL);
	else
		m_pAction = Sequence::create(
			CallFunc::create([=] {m_pAction->getTarget()->setRotation(0); }),
			DelayTime::create(m_fDelay),
			RotateBy::create(m_fDuration, m_fAngle),
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
	a->init(m_fDelay, m_fDuration, m_fAngle, m_Anchor);
	a->setIsRepeat(m_bIsRepeat);
	a->setIsReverse(m_bIsReverse);
	a->updateAction();
	a->autorelease();
	return a;
}

/*===============================Blink Action===============================*/
BlinkActionData::BlinkActionData(void)
	: m_fDuration(0.0f)
	, m_fPostDelay(0.0f)
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
