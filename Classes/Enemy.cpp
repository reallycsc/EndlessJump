#include "Enemy.h"

Enemy::Enemy(void)
	: m_nType(TYPE_NORMAL)
	, m_bIsSelected(false)
	, m_size(Size(50, 50))
	, m_color(Color4F::BLUE)
	, m_bIsPlayerAdded(false)
	, m_fRotateDuration(1.0f)
	, m_StartPoint(Point::ZERO)
	, m_DestPoint(Point::ZERO)
	, m_fMoveDuration(1.0f)
	, m_fBlinkDuration(1.0f)
	, m_fBlinkHideDuration(1.0f)
{
}

Enemy::~Enemy(void)
{
}

Enemy* Enemy::create(const Size &size, const Color3B &color, const int &id)
{
	Enemy *pRet = new(std::nothrow) Enemy();
	if (pRet && pRet->init(size, color, id))
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

bool Enemy::init(const Size &size, const Color3B &color, const int &id)
{  
    bool bRet = false;  
    do   
    {  
        CC_BREAK_IF(!Node::init());  

		m_nType = TYPE_NORMAL;
		m_size = size;
		m_color = Color4F(color);
		// draw enemy rect
		DrawNode* drawNode = DrawNode::create();
		drawNode->drawSolidRect(Vec2(0, 0), m_size, m_color);
		this->addChild(drawNode, 0, TAG_BLOCK);
		this->setContentSize(m_size);
		this->setAnchorPoint(Vec2(0.5f, 0));

		// set physcis body
		PhysicsBody* body = PhysicsBody::createBox(m_size);
		body->setDynamic(false);
		body->setGravityEnable(false);
		body->setCategoryBitmask(MASK_ENEMY);
		body->setCollisionBitmask(MASK_PLAYER);
		body->setContactTestBitmask(MASK_PLAYER);
		this->setPhysicsBody(body);

#ifdef DEBUG_MODE
		if (id >= 0) // add id label
		{
			Text* text = Text::create(StringUtils::format("%d", id), "fonts/arial.ttf", 20);
			text->setAnchorPoint(Vec2(0.5f, 0.5f));
			text->setPosition(m_size / 2);
			this->addChild(text, 2, TAG_TEXT_ID);
		}
#endif // DEBUG_MODE

        bRet = true;
    } while (0);
    
    return bRet;
}

Enemy* Enemy::createRotate(const Size &size, const Color3B &color, const int &id, const float &duration, const bool &isReverse)
{
	Enemy* enemy = Enemy::create(size, color, id);
	enemy->setType(TYPE_ROTATE);
	enemy->setAnchorPoint(Vec2(0.5f, 0.5f));
	enemy->setRotateDuration(duration);
	float angle = 360;
	if (isReverse)
	{
		enemy->setType(TYPE_ROTATE_REVERSE);
		angle = -360;
	}
	auto action = RepeatForever::create(RotateBy::create(duration, angle));
	action->setTag(TAG_ACTION_ROTATE);
	enemy->runAction(action);
	return enemy;
}

Enemy* Enemy::createMove(const Size& size, const Color3B& color, const int& id, const Point &start, const Point& dest, const float& duration)
{
	Enemy* enemy = Enemy::create(size, color, id);
	enemy->setType(TYPE_MOVE);
	enemy->setStartPoint(start);
	enemy->setDestPoint(dest);
	enemy->setMoveDuration(duration);
	enemy->setPosition(start);
	auto action = RepeatForever::create(Sequence::createWithTwoActions(
		MoveTo::create(duration, dest),
		MoveTo::create(duration, start)));
	action->setTag(TAG_ACTION_MOVE);
	enemy->runAction(action);
	return enemy;
}

Enemy* Enemy::createBlink(const Size& size, const Color3B& color, const int& id, const float &duration_blink, const float &duration_hide)
{
	Enemy* enemy = Enemy::create(size, color, id);
	enemy->setType(TYPE_BLINK);
	enemy->setBlinkDuration(duration_blink);
	enemy->setBlinkHideDuration(duration_hide);
	auto action = RepeatForever::create(Sequence::create(
		DelayTime::create(duration_blink),
		CallFunc::create([=] {enemy->setVisible(false); }),
		DelayTime::create(duration_hide),
		CallFunc::create([=] {enemy->setVisible(true); }),
		NULL));
	action->setTag(TAG_ACTION_BLINK);
	enemy->runAction(action);
	return enemy;
}

void Enemy::updateId(const int& id)
{
	this->removeChildByTag(TAG_TEXT_ID);
	Text* text = Text::create(StringUtils::format("%d", id), "fonts/arial.ttf", 20);
	text->setAnchorPoint(Vec2(0.5f, 0.5f));
	text->setPosition(m_size / 2);
	this->addChild(text, 2, TAG_TEXT_ID);
}

void Enemy::updateSize(const Size &size)
{
	this->removeChildByTag(TAG_BLOCK);

	m_size = size;
	DrawNode* newNode = DrawNode::create();
	newNode->drawSolidRect(Vec2(0, 0), m_size, m_color);
	this->addChild(newNode, 0, TAG_BLOCK);
	this->setContentSize(m_size);

	this->getPhysicsBody()->removeFromWorld();
	PhysicsBody* body = PhysicsBody::createBox(m_size);
	//body->setDynamic(false);
	body->setGravityEnable(false);
	body->setCategoryBitmask(MASK_ENEMY);
	body->setCollisionBitmask(MASK_PLAYER);
	body->setContactTestBitmask(MASK_PLAYER);
	this->setPhysicsBody(body);

	if (m_bIsSelected)
	{
		this->removeChildByTag(TAG_SELECTED_BLOCK);
		this->select();
	}

	// update id position
	this->getChildByTag(TAG_TEXT_ID)->setPosition(m_size / 2);
}

void Enemy::updateColor(const Color3B &color)
{
	this->removeChildByTag(TAG_BLOCK);

	m_color = Color4F(color);
	DrawNode* newNode = DrawNode::create();
	newNode->drawSolidRect(Vec2(0, 0), m_size, m_color);
	this->addChild(newNode, 0, TAG_BLOCK);

	if (m_bIsSelected)
	{
		this->removeChildByTag(TAG_SELECTED_BLOCK);
		this->select();
	}
}

void Enemy::updateRotateDuration(const float &duration)
{
	m_fRotateDuration = duration;
	if (m_nType == TYPE_ROTATE)
	{
		this->stopActionByTag(TAG_ACTION_ROTATE);
		auto action = RepeatForever::create(RotateBy::create(duration, 360));
		action->setTag(TAG_ACTION_ROTATE);
		this->runAction(action);
	}
	else if (m_nType == TYPE_ROTATE_REVERSE)
	{
		this->stopActionByTag(TAG_ACTION_ROTATE);
		auto action = RepeatForever::create(RotateBy::create(duration, -360));
		action->setTag(TAG_ACTION_ROTATE);
		this->runAction(action);
	}
}

void Enemy::updateStartPoint(const Point& start)
{
	m_StartPoint = start;
	if (m_nType == TYPE_MOVE)
	{
		this->stopActionByTag(TAG_ACTION_MOVE);
		auto action = RepeatForever::create(Sequence::createWithTwoActions(
			MoveTo::create(m_fMoveDuration, m_DestPoint),
			MoveTo::create(m_fMoveDuration, start)));
		action->setTag(TAG_ACTION_MOVE);
		this->runAction(action);
	}
}

void Enemy::updateDestPoint(const Point& dest)
{
	m_DestPoint = dest;
	if (m_nType == TYPE_MOVE)
	{
		this->stopActionByTag(TAG_ACTION_MOVE);
		auto action = RepeatForever::create(Sequence::createWithTwoActions(
			MoveTo::create(m_fMoveDuration, dest),
			MoveTo::create(m_fMoveDuration, m_StartPoint)));
		action->setTag(TAG_ACTION_MOVE);
		this->runAction(action);
	}
}

void Enemy::updateMoveDuration(const float& duration)
{
	m_fMoveDuration = duration;
	if (m_nType == TYPE_MOVE)
	{
		this->stopActionByTag(TAG_ACTION_MOVE);
		auto action = RepeatForever::create(Sequence::createWithTwoActions(
			MoveTo::create(duration, m_DestPoint),
			MoveTo::create(duration, m_StartPoint)));
		action->setTag(TAG_ACTION_MOVE);
		this->runAction(action);
	}
}

void Enemy::updateBlinkDuration(const float& duration)
{
	m_fBlinkDuration = duration;
	if (m_nType == TYPE_BLINK)
	{
		this->stopActionByTag(TAG_ACTION_BLINK);
		auto action = RepeatForever::create(Sequence::create(
			DelayTime::create(duration),
			CallFunc::create([=] {this->setVisible(false); }),
			DelayTime::create(m_fBlinkHideDuration),
			CallFunc::create([=] {this->setVisible(true); }),
			NULL));
		action->setTag(TAG_ACTION_BLINK);
		this->runAction(action);
	}
}

void Enemy::updateBlinkHideDuration(const float& duration)
{
	m_fBlinkHideDuration = duration;
	if (m_nType == TYPE_BLINK)
	{
		this->stopActionByTag(TAG_ACTION_BLINK);
		auto action = RepeatForever::create(Sequence::create(
			DelayTime::create(m_fBlinkDuration),
			CallFunc::create([=] {this->setVisible(false); }),
			DelayTime::create(duration),
			CallFunc::create([=] {this->setVisible(true); }),
			NULL));
		action->setTag(TAG_ACTION_BLINK);
		this->runAction(action);
	}
}

void Enemy::select()
{
	if (!m_bIsSelected)
	{
		DrawNode* drawNode = DrawNode::create();
		Color4F color = Color4F::WHITE;
		if (m_color == Color4F::WHITE)
			color = Color4F::RED;
		float halfWidth = m_size.width / 2;
		float halfHeight = m_size.height / 2;
		float radius = MIN(halfWidth, halfHeight);
		drawNode->drawCircle(Vec2(halfWidth, halfHeight), radius, 360, 20, false, color);
		drawNode->setAnchorPoint(Vec2(0.5f, 0.5f));
		this->addChild(drawNode, 1, TAG_SELECTED_BLOCK);
		m_bIsSelected = true;
	}
}

void Enemy::unSelect()
{
	if (m_bIsSelected)
	{
		this->removeChildByTag(TAG_SELECTED_BLOCK);
		m_bIsSelected = false;
	}
}

void Enemy::addPlayerBlockForLevelMake(Size playerSize)
{
	CS_RETURN_IF(m_nType == TYPE_ROTATE || m_nType == TYPE_ROTATE_REVERSE);

	if (!m_bIsPlayerAdded)
	{
		DrawNode* drawNode = DrawNode::create();
		drawNode->setLineWidth(1);
		Color4F color = Color4F::WHITE;
		if (m_color == Color4F::WHITE)
			color = Color4F::RED;
		float shortRadius = playerSize.width / 2;
		float sqrt2 = sqrtf(2.0f);
		float longRadius = playerSize.width / sqrt2;
		float longCenter = shortRadius / sqrt2;
		// left 
		//drawNode->drawRect(Point::ZERO - playerSize, Point::ZERO, color);
		drawNode->drawCircle(Point(-shortRadius, -shortRadius), longRadius, 360, 20, false, color);
		//drawNode->drawCircle(Point(-longCenter, -longCenter), shortRadius, 360, 20, false, color);
		// right
		//drawNode->drawRect(Point(m_size.width, -playerSize.height), Point(m_size.width + playerSize.width, 0), color);
		drawNode->drawCircle(Point(m_size.width + shortRadius, -shortRadius), longRadius, 360, 20, false, color);
		//drawNode->drawCircle(Point(m_size.width + longCenter, -longCenter), shortRadius, 360, 20, false, color);

		this->addChild(drawNode, 1, TAG_PLAYER_BLOCK);
		m_bIsPlayerAdded = true;
	}
} 

void Enemy::removePlayerBlockForLevelMake()
{
	if (m_bIsPlayerAdded)
	{
		this->removeChildByTag(TAG_PLAYER_BLOCK);
		m_bIsPlayerAdded = false;
	}
}
