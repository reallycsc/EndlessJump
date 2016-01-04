#include "Enemy.h"

Enemy::Enemy(void)
	: m_nType(TYPE_NORMAL)
	, m_bIsSelected(false)
	, m_size(Size(50,50))
	, m_color(Color4F::BLUE)
	, m_bIsPlayerAdded(false)
	, m_fRotateDuration(1.0f)
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

#if DEBUG_FLAG == 1
		if (id >= 0) // add id label
		{
			char buf[10];
			sprintf(buf, "%d", id);
			Text* text = Text::create(buf, "Arial", 20);
			text->setAnchorPoint(Vec2(0.5f, 0.5f));
			text->setPosition(m_size / 2);
			this->addChild(text, 2);
		}
#endif // DEBUG_FLAG == 1

        bRet = true;
    } while (0);
    
    return bRet;
}

Enemy* Enemy::createWithType(const int type, const Size &size, const Color3B &color, const int &id, const float rotateDuration)
{
	Enemy* enemy = Enemy::create(size, color, id);
	enemy->setType(type);
	switch (type)
	{
	case TYPE_NORMAL:
		break;
	case TYPE_ROTATE:
	{
		enemy->setAnchorPoint(Vec2(0.5f, 0.5f));
		enemy->setRotateDuration(rotateDuration);
		auto action = RepeatForever::create(RotateBy::create(rotateDuration, -360));
		action->setTag(TAG_ACTION_ROTATE);
		enemy->runAction(action);
		break;
	}
	default:
		break;
	}
	return enemy;
}

void Enemy::updateSize(const Size& size)
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
}

void Enemy::updateColor(const Color3B& color)
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

void Enemy::updateRotateDuration(const float duration)
{
	m_fRotateDuration = duration;
	if (m_nType == TYPE_ROTATE)
	{
		this->stopActionByTag(TAG_ACTION_ROTATE);
		auto action = RepeatForever::create(RotateBy::create(duration, -360));
		action->setTag(TAG_ACTION_ROTATE);
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
	CS_RETURN_IF(m_nType == TYPE_ROTATE);

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
		drawNode->drawRect(Point::ZERO - playerSize, Point::ZERO, color);
		//drawNode->drawCircle(Point(-shortRadius, -shortRadius), longRadius, 360, 20, false, color);
		//drawNode->drawCircle(Point(-longCenter, -longCenter), shortRadius, 360, 20, false, color);
		// right
		drawNode->drawRect(Point(m_size.width, -playerSize.height), Point(m_size.width + playerSize.width, 0), color);
		//drawNode->drawCircle(Point(m_size.width + shortRadius, -shortRadius), longRadius, 360, 20, false, color);
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
