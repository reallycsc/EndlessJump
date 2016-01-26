#include "Enemy.h"

Enemy::Enemy(void)
	: m_nID(0)
	, m_nRoomID(0)
	, m_bIsSelected(false)
	, m_size(Size(50, 50))
	, m_color(Color4F::BLUE)
	, m_bIsPlayerAdded(false)
	, m_bIsHaveMoveAction(false)
{
	m_vActions.clear();
}

Enemy::~Enemy(void)
{
	m_vActions.clear();
}

Enemy* Enemy::create(const Size &size, const Color3B &color, const int &id, const int &room_id)
{
	Enemy *pRet = new(std::nothrow) Enemy();
	pRet->init(size, color, id, room_id);
	pRet->autorelease();
	return pRet;
}

bool Enemy::init(const Size &size, const Color3B &color, const int &id, const int &room_id)
{  
    bool bRet = false;  
    do   
    {  
        CC_BREAK_IF(!Node::init());  

		m_nID = id;
		m_nRoomID = room_id;
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

#ifdef LEVEL_MAKER_MODE
		if (id >= 0) // add id label
		{
			Text* text = Text::create(StringUtils::format("%d", id), "fonts/arial.ttf", 20);
			text->setAnchorPoint(Vec2(0.5f, 0.5f));
			text->setPosition(m_size / 2);
			this->addChild(text, 2, TAG_TEXT_ID);
		}
#endif // LEVEL_MAKER_MODE

        bRet = true;
    } while (0);
    
    return bRet;
}

bool Enemy::addAction(ActionData* action_data)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!action_data);

		// All actions are unique
		int type = action_data->getType();
		for (size_t i = 0, length = m_vActions.size(); i < length; i++)
			if (m_vActions.at(i)->getType() == type)
				return false;
		this->prepareForAction(action_data);
		m_vActions.pushBack(action_data);
		this->runAction(action_data->getAction());
		bRet = true;
	} while (0);

	return bRet;
}

bool Enemy::removeAction(ActionData* action_data)
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!action_data);

		auto type = action_data->getType();
		this->stopActionByTag(type);
		// return to normal
		switch (type)
		{
		case TYPE_MOVE:
			m_bIsHaveMoveAction = false;
			this->setPosition(dynamic_cast<MoveActionData*>(action_data)->getStart());
			break;
		case TYPE_ROTATE:
			this->setAnchorPoint(Vec2(0.5f, 0));
			this->setRotation(0);
			break;
		case TYPE_BLINK:
			this->setVisible(true);
			break;
		case TYPE_SCALE:
			this->setAnchorPoint(Vec2(0.5f, 0));
			this->setScale(1, 1);
			break;
		default:
			break;
		}

		m_vActions.eraseObject(action_data);

		bRet = true;
	} while (0);

	return bRet;
}

void Enemy::updateAction(ActionData* action_data)
{
	CS_RETURN_IF(!action_data);

	action_data->updateAction();

	// stop old action
	this->stopActionByTag(action_data->getType());
	// set data for new action
	this->prepareForAction(action_data);
	// run new action
	this->runAction(action_data->getAction());
}

void Enemy::restartActions()
{
	for (size_t i = 0, length = m_vActions.size(); i < length; i++)
		this->updateAction(m_vActions.at(i));
}

void Enemy::updateId(const int& id)
{
	this->removeChildByTag(TAG_TEXT_ID);
	m_nID = id;
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
		m_bIsSelected = false;
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

void Enemy::updateStartPoint(const Point& start)
{
	CS_RETURN_IF(!m_bIsHaveMoveAction);

	for (size_t i = 0, length = m_vActions.size(); i < length; i++)
	{
		auto action_data = m_vActions.at(i);
		auto type = action_data->getType();
		if (type == TYPE_MOVE)
		{
			auto move_data = dynamic_cast<MoveActionData*>(action_data);
			this->stopActionByTag(move_data->getType());
			move_data->setStart(start);
			move_data->updateAction();
			this->runAction(move_data->getAction());
		}
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
	// CS_RETURN_IF(has rotate action)

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
		//float longCenter = shortRadius / sqrt2;
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

Point Enemy::getStartPoint()
{
	if(m_bIsHaveMoveAction)
	{
		for (size_t i = 0, length = m_vActions.size(); i < length; i++)
		{
			auto action_data = m_vActions.at(i);
			auto type = action_data->getType();
			if (type == TYPE_MOVE)
				return dynamic_cast<MoveActionData*>(action_data)->getStart();
		}
	}

	return Point::ZERO;
}

void Enemy::setActions(Vector<ActionData*>* actions)
{
	CS_RETURN_IF(!actions);

	for (size_t i = 0, length = actions->size(); i < length; i++)
	{
		auto action_data = actions->at(i);
		this->prepareForAction(action_data);
		auto action_data_clone = action_data->clone();
		m_vActions.pushBack(action_data_clone);
		this->runAction(action_data_clone->getAction());
	}
}

void Enemy::prepareForAction(ActionData* action_data)
{
	int type = action_data->getType();
	switch (type)
	{
	case TYPE_MOVE:
		m_bIsHaveMoveAction = true;
		break;
	case TYPE_ROTATE:
		this->setAnchorPoint(dynamic_cast<RotateActionData*>(action_data)->getAnchor());
		break;
	case TYPE_BLINK:
		break;
	case TYPE_SCALE:
		this->setAnchorPoint(dynamic_cast<ScaleActionData*>(action_data)->getAnchor());
		break;
	default:
		break;
	}
}
