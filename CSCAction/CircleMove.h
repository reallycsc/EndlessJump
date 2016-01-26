#pragma once
#include "../CommonDefinition.h"

NS_CSC_BEGIN
class CircleMoveAct : public ActionInterval
{
public:
	CircleMoveAct();

	static CircleMoveAct* create(float duration, const Point& center, float scale, float angle);
	bool initWithDuration(float duration, const Point& center, float scaleDiff, float angle);

	// override of virtual function of base class (must override)
	virtual CircleMoveAct* clone() const override;
	virtual void startWithTarget(Node *target) override;
	virtual void update(float time) override;
	virtual void stop(void) override;

protected:
	float m_duration;
	Point m_center;
	float m_scaleDiff;
	float m_currScale;
	float m_angle;
	float m_anglePreFrame;
	int m_frameCnts;
	Point m_initPos;
};
NS_CSC_END
