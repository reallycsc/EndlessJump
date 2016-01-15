#include "ActionData.h"
#include "GameLevelData.h"

/*===============================Base Class===============================*/
ActionData::ActionData(void)
	: m_nType(TYPE_NORMAL)
	, m_fDelay(0.0f)
	, m_pAction(nullptr)
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
	if (pRet && pRet->init(delay, duration, start, destination))
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
		m_pAction = RepeatForever::create(Sequence::create(
			DelayTime::create(m_fDelay),
			MoveTo::create(m_fDuration, m_Destination),
			MoveTo::create(m_fDuration, m_Start),
			NULL));
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
	m_pAction = RepeatForever::create(Sequence::create(
		DelayTime::create(m_fDelay),
		MoveTo::create(m_fDuration, m_Destination),
		MoveTo::create(m_fDuration, m_Start),
		NULL));
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

MoveActionData* MoveActionData::clone() const
{    
	// no copy constructor
	auto a = new (std::nothrow) MoveActionData();
	a->init(m_fDelay, m_fDuration, m_Start, m_Destination);
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
	if (pRet && pRet->init(delay, duration, angle, anchor))
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
		{
			m_pAction = RepeatForever::create(Sequence::createWithTwoActions(
				DelayTime::create(m_fDelay),
				RotateBy::create(m_fDuration, m_fAngle)));
		}
		else
		{
			m_pAction = RepeatForever::create(Sequence::create(
				DelayTime::create(m_fDelay),
				RotateBy::create(m_fDuration, m_fAngle),
				RotateBy::create(m_fDuration, -m_fAngle),
				NULL));
		}
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
	{
		m_pAction = RepeatForever::create(Sequence::createWithTwoActions(
			DelayTime::create(m_fDelay),
			RotateBy::create(m_fDuration, m_fAngle)));
	}
	else
	{
		m_pAction = RepeatForever::create(Sequence::create(
			DelayTime::create(m_fDelay),
			RotateBy::create(m_fDuration, m_fAngle),
			RotateBy::create(m_fDuration, -m_fAngle),
			NULL));
	}
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

RotateActionData* RotateActionData::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) RotateActionData();
	a->init(m_fDelay, m_fDuration, m_fAngle, m_Anchor);
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
	if (pRet && pRet->init(delay, duration, postDelay))
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

bool BlinkActionData::init(const float &delay, const float &duration, const float &postDelay)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_BLINK;
		m_fDelay = delay;
		m_fDuration = duration;
		m_fPostDelay = postDelay;
		m_pAction = RepeatForever::create(Sequence::create(
			DelayTime::create(m_fDelay),
			CallFunc::create([=] {m_pAction->getTarget()->setVisible(true); }),
			DelayTime::create(m_fDuration),
			CallFunc::create([=] {m_pAction->getTarget()->setVisible(false); }),
			DelayTime::create(m_fPostDelay),
			NULL));
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
	m_pAction = RepeatForever::create(Sequence::create(
		DelayTime::create(m_fDelay),
		CallFunc::create([=] {m_pAction->getTarget()->setVisible(true); }),
		DelayTime::create(m_fDuration),
		CallFunc::create([=] {m_pAction->getTarget()->setVisible(false); }),
		DelayTime::create(m_fPostDelay),
		NULL));
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

BlinkActionData* BlinkActionData::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) BlinkActionData();
	a->init(m_fDelay, m_fDuration, m_fPostDelay);
	a->autorelease();
	return a;
}

/*===============================Move oneway Action===============================*/
MoveOnewayActionData::MoveOnewayActionData(void)
{
}

MoveOnewayActionData::~MoveOnewayActionData(void)
{
}

MoveOnewayActionData* MoveOnewayActionData::create(const float &delay, const float &duration, const Point &start, const Point &destination)
{
	MoveOnewayActionData* pRet = new(std::nothrow) MoveOnewayActionData();
	if (pRet && pRet->init(delay, duration, start, destination))
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

bool MoveOnewayActionData::init(const float &delay, const float &duration, const Point &start, const Point &destination)
{
	bool bRet = false;
	do
	{
		m_nType = TYPE_MOVE_ONEWAY;
		m_fDelay = delay;
		m_fDuration = duration;
		m_Start = start;
		m_Destination = destination;
		m_pAction = RepeatForever::create(Sequence::create(
			DelayTime::create(m_fDelay),
			MoveTo::create(m_fDuration, m_Destination),
			CallFunc::create([=] {m_pAction->getTarget()->setPosition(m_Start); }),
			NULL));
		m_pAction->setTag(m_nType);
		m_pAction->retain();
		bRet = true;
	} while (0);

	return bRet;
}

void MoveOnewayActionData::updateAction()
{
	m_pAction->release();
	m_pAction = nullptr;
	m_pAction = RepeatForever::create(Sequence::create(
		DelayTime::create(m_fDelay),
		MoveTo::create(m_fDuration, m_Destination),
		CallFunc::create([=] {m_pAction->getTarget()->setPosition(m_Start); }),
		NULL));
	m_pAction->setTag(m_nType);
	m_pAction->retain();
}

MoveOnewayActionData* MoveOnewayActionData::clone() const
{
	// no copy constructor
	auto a = new (std::nothrow) MoveOnewayActionData();
	a->init(m_fDelay, m_fDuration, m_Start, m_Destination);
	a->autorelease();
	return a;
}