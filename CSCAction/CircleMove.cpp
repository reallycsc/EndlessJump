#include "CircleMove.h"

NS_CSC_BEGIN
CircleMoveAct::CircleMoveAct(): m_duration(0), m_scaleDiff(0), m_currScale(0), m_angle(0), m_anglePreFrame(0), m_frameCnts(0)
{
}

CircleMoveAct* CircleMoveAct::create(float duration, const Point& center, float scale, float angle)
{
	CircleMoveAct *pRet = new CircleMoveAct();
	pRet->initWithDuration(duration, center, scale, angle);
	pRet->autorelease();

	return pRet;
}

	bool CircleMoveAct::initWithDuration(float duration, const Point& center, float scaleDiff, float angle)
{
	if (ActionInterval::initWithDuration(duration))
	{
		this->m_duration = duration;
		this->m_center = center;
		this->m_scaleDiff = scaleDiff;
		this->m_currScale = 1.0f;
		this->m_angle = angle;

		// calculate the angle when in update()
		this->m_anglePreFrame = angle / duration * Director::getInstance()->getAnimationInterval() / (180 / M_PI);
		this->m_frameCnts = 0;
		return true;
	}
	return false;
}

CircleMoveAct* CircleMoveAct::clone(void) const
{
	auto a = new CircleMoveAct();
	a->initWithDuration(m_duration, m_center, m_scaleDiff, m_angle);
	a->autorelease();
	return a;
}

void CircleMoveAct::startWithTarget(Node *target)
{
	ActionInterval::startWithTarget(target);
	m_initPos = target->getPosition();
}

void CircleMoveAct::update(float time)
{
	m_frameCnts++;
	m_currScale += m_scaleDiff;

	Point newPos = m_initPos.rotateByAngle(m_center, m_frameCnts * m_anglePreFrame);
	Point diff = newPos - m_center;
	newPos = diff * m_currScale + m_center;

	_target->setPosition(newPos);
}

void CircleMoveAct::stop(void)
{
	// Action is done, reset clip position
	_target->setPosition(m_initPos);

	ActionInterval::stop();
}
NS_CSC_END