#include "GameScene.h"
#include "CSCClass\CSCAction\Shake.h"
#include "CSCClass\CommonFunctions.h"

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	// open Debug
	PhysicsWorld* world = scene->getPhysicsWorld();
	world->setGravity(Vec2(0, 0));
	//world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
	layer->setPhyWorld(world);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
void GameScene::onEnter()
{
	Layer::onEnter();
	// bind contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	// bind touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
	// get level data
	m_pLevelData = GameLevelData::create();

	// add level background
	for (list<RoomData>::const_iterator roomIter = m_pLevelData->getRoomsData()->begin();
	roomIter !=  m_pLevelData->getRoomsData()->end(); ++roomIter)
	{
		auto background = DrawNode::create();
		background->drawSolidRect(roomIter->position, roomIter->position + roomIter->size, Color4F(roomIter->color));
		this->addChild(background);

		// add enemys
		for (vector<EnemyData>::const_iterator iter = roomIter->enemysData.begin();
		iter != roomIter->enemysData.end(); ++iter)
		{
			auto enemy = Enemy::create(iter->size, iter->color);
			enemy->setPosition(iter->position);
			this->addChild(enemy);
			m_vEnemys.pushBack(enemy);
		}
	}
	
	// add player
	m_iterCurRoom = m_pLevelData->getRoomsData()->begin();
	this->addPlayer();

	this->scheduleUpdate();

    return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	//Point point = touch->getLocation();
	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event)
{
	//Point point = touch->getLocation();
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
	if (m_pPlayer)
	{
		m_pPlayer->jump();
	}
}

bool GameScene::onContactBegin(const PhysicsContact& contact)
{
	auto body1 = contact.getShapeA()->getBody();
	auto body2 = contact.getShapeB()->getBody();
	if (!body1->getNode() || !body2->getNode())
	{
		return false;
	}

	if ((body1->getCategoryBitmask() & MASK_PLAYER) == MASK_PLAYER ||
		(body2->getCategoryBitmask() & MASK_PLAYER) == MASK_PLAYER)
	{
		this->runAction(CSCClass::Shake::create(0.1f, 3));

		auto particle = ParticleExplosion::createWithTotalParticles(130);
		particle->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("playerImage"));
		//particle->setBlendAdditive(true);
		particle->setStartSize(5);
		particle->setStartSizeVar(5);
		particle->setStartColor(m_pPlayer->getPlayerColor());
		particle->setStartColorVar(Color4F(0, 0, 0, 0));
		//particle->setEndColor(m_pPlayer->getPlayerColor());
		//particle->setEndColorVar(Color4F(0, 0, 0, 0));
		particle->setSpeed(500);
		particle->setSpeedVar(300);
		particle->setLife(0.5f);
		particle->setLifeVar(0.2f);
		particle->setPosition(contact.getContactData()->points[0]);
		this->addChild(particle);

		m_pPlayer->die();
		m_pPlayer = NULL;

		this->runAction(Sequence::createWithTwoActions(
			DelayTime::create(0.5f),
			CallFuncN::create([=](Ref* pSender)->void
		{
			this->addPlayer();
		})));
	}

	return false;
}

// schedule functions
void GameScene::update(float dt)
{
	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//float stepW = visibleSize.width * 0.2f * dt;
	//float halfEnemyWidth = m_pPlayer->getSpeed() * m_pPlayer->getJumpDuration() / 2 - m_pPlayer->getContentSize().width / 2;
	//if (m_pPlayer->getPositionX() + stepW * 6 >= visibleSize.width / 2 - halfEnemyWidth &&
	//	m_pPlayer->getPositionX() - stepW * 6 <= visibleSize.width / 2 + halfEnemyWidth)
	//{
	//	m_pPlayer->jump();
	//}
}

void GameScene::addPlayer()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	float duration = visibleSize.width / m_pLevelData->getPlayerSpeed();
	m_pPlayer = Player::create(m_iterCurRoom->enemysData.begin()->color, m_pLevelData);
	Size playerSize = m_pPlayer->getContentSize();
	float posX = playerSize.width / 2;
	float posY = m_iterCurRoom->position.y + playerSize.height / 2;
	m_pPlayer->setPosition(posX, posY);
	m_pPlayer->runAction(Sequence::createWithTwoActions(
		MoveTo::create(duration, Vec2(posX + visibleSize.width, posY)),
		CallFuncN::create([=](Ref* pSender)->void
	{
		m_pPlayer->die();
		++m_iterCurRoom;
		if (m_iterCurRoom != m_pLevelData->getRoomsData()->end())
		{
			this->addPlayer();
		}
	})));
	this->addChild(m_pPlayer);
}

// Debug functions
void GameScene::addMaximumEnemy_Debug()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	float stepT = 0.01f;
	float halfT = m_pLevelData->getPlayerJumpDuration() / 2;
	int steps = halfT / stepT;
	float realT = 0;
	float scaleT = 0;
	float scaleStepT = 0.5f / steps;
	for (int i = 0; i < steps; i++)
	{
		float enemyWidth = (m_pLevelData->getPlayerSpeed() * (halfT - realT) - m_pPlayer->getContentSize().width / 2) * 2;
		float enemyHeight = m_pPlayer->getJumpHeight() * 4 * scaleT * (1 - scaleT);
		if (enemyHeight > 0 && enemyWidth > 0)
		{
			auto enemy = Enemy::create(Size(enemyWidth, enemyHeight), Color3B::MAGENTA);
			enemy->setPosition(visibleSize.width / 2, m_pLevelData->getRoomsData()->begin()->position.y);
			this->addChild(enemy);
			m_vEnemys.pushBack(enemy);
			//i = steps;
		}
		realT += stepT;
		scaleT += scaleStepT;
	}
}