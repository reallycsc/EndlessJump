#pragma once
#include "CommonHeader.h"

enum enemyActionType
{
	TYPE_NORMAL = 1,
	TYPE_ROTATE,
	TYPE_MOVE,
	TYPE_BLINK,
};

/*===============================Base Class===============================*/
class ActionData : public Ref, public Clonable
{
public:
	ActionData(void);
	~ActionData(void);

	virtual void updateAction(){};
	virtual ActionData* clone() const
	{
		CC_ASSERT(0);
		return nullptr;
	}

public:
	CC_SYNTHESIZE(int, m_nType, Type);
	CC_SYNTHESIZE(float, m_fDelay, Delay);
	CC_SYNTHESIZE(bool, m_bIsRepeat, IsRepeat);
	CC_SYNTHESIZE(bool, m_bIsReverse, IsReverse);
	CC_SYNTHESIZE(ActionInterval*, m_pAction, Action);
};

/*===============================Move Action===============================*/
class MoveActionData : public ActionData
{
public:
	MoveActionData(void);
	~MoveActionData(void);

	static MoveActionData* create(const float &delay, const float &duration, const Point &start, const Point &destination);
	bool init(const float &delay, const float &duration, const Point &start, const Point &destination);

	virtual void updateAction() override;
	virtual MoveActionData* clone() const override;

public:
	CC_SYNTHESIZE(float, m_fDuration, Duration);
	CC_SYNTHESIZE(Point, m_Start, Start);
	CC_SYNTHESIZE(Point, m_Destination, Destination);
};

/*===============================Rotate Action===============================*/
class RotateActionData : public ActionData
{
public:
	RotateActionData(void);
	~RotateActionData(void);
	static RotateActionData* create(const float &delay, const float &duration, const float &angle, const Vec2 &anchor);
	bool init(const float &delay, const float &duration, const float &angle, const Vec2 &anchor);

	virtual void updateAction() override;
	virtual RotateActionData* clone() const override;

public:
	CC_SYNTHESIZE(float, m_fDuration, Duration);
	CC_SYNTHESIZE(float, m_fAngle, Angle);
	CC_SYNTHESIZE(Vec2, m_Anchor, Anchor);
};

/*===============================Blink Action===============================*/
class BlinkActionData : public ActionData
{
public:
	BlinkActionData(void);
	~BlinkActionData(void);
	static BlinkActionData* create(const float &delay, const float &duration, const float &postDelay);
	bool init(const float &delay, const float &duration, const float &postDelay);

	virtual void updateAction() override;
	virtual BlinkActionData* clone() const override;

public:
	CC_SYNTHESIZE(float, m_fDuration, Duration);
	CC_SYNTHESIZE(float, m_fPostDelay, PostDelay);
};