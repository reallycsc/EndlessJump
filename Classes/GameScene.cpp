#include "GameScene.h"
#include "CSCClass/CSCAction/Shake.h"
#include "GamePauseScene.h"
#include "GameOverScene.h"
#include "StoryScene.h"
#include "CSCClass/CommonFunctions.h"

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	// open Debug
	PhysicsWorld* world = scene->getPhysicsWorld();
	world->setGravity(Vec2(0, 0));
#ifdef LEVEL_MAKER_MODE
	//world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

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

	if (m_bIsLevelFinished)
		Director::getInstance()->replaceScene(GameOverScene::createScene(m_nDeadNumber));
}

bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    /////////////////////////////
	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_pGameMediator = GameMediator::getInstance();
	m_nRoomCount = 0;
	m_nDeadNumber = 0;
	m_pPlayer = nullptr;
	m_pParticleTail = nullptr;
	m_LastJumpPoint = Point(-100, -100);
	m_bIsLevelFinished = false;

	// get level data
	auto levelsData = m_pGameMediator->getGameLevelData();
	int curLevel = m_pGameMediator->getCurGameLevel();
	m_pCurLevelData = levelsData->at(curLevel - 1);
	
	// load scene
	auto rootNode = CSLoader::createNode("GameScene.csb");
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
	this->addChild(rootNode, 100);
	// get button
	m_pButtonPause = dynamic_cast<Button*>(rootNode->getChildByName("Button_Pause"));
	m_pButtonPause->addClickEventListener(CC_CALLBACK_1(GameScene::buttonCallback_Pause, this));
	m_pButtonRetry = dynamic_cast<Button*>(rootNode->getChildByName("Button_Retry"));
	m_pButtonRetry->addClickEventListener(CC_CALLBACK_1(GameScene::buttonCallback_Retry, this));
	m_pButtonPause->setEnabled(false);
	m_pButtonRetry->setEnabled(false);
	// get text
	m_pTextDeadNum = dynamic_cast<Text*>(rootNode->getChildByName("Text_DeadNumber"));
	m_nDeadNumberMin = m_pGameMediator->getLevelMinDeadCount()->at(curLevel - 1);
	if (m_nDeadNumberMin < 0)
		m_pTextDeadNum->setString(StringUtils::format("%d", m_nDeadNumber));
	else
		m_pTextDeadNum->setString(StringUtils::format("%d/%d", m_nDeadNumber, m_nDeadNumberMin));
	
	Text* levelName = dynamic_cast<Text*>(rootNode->getChildByName("Text_LevelName"));
	levelName->setString(m_pCurLevelData->getLevelName());
	Text* levelNumber = dynamic_cast<Text*>(rootNode->getChildByName("Text_LevelNumber"));
	levelNumber->setString(StringUtils::format("%d/%d", curLevel, m_pGameMediator->getGameLevelCount()));

	// black layer
	m_pBlackLayer = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
	m_pBlackLayer->setPosition(Point::ZERO);
	this->addChild(m_pBlackLayer, 99);

	// play animate
    m_pButtonPause->setPositionY(visibleSize.height + 70);
    m_pButtonPause->runAction(Sequence::create(DelayTime::create(0.5f),
                                               MoveTo::create(0.5f, m_pButtonPause->getPosition() - Point(0, 80)),
                                               NULL));
    m_pButtonRetry->setPositionY(visibleSize.height + 70);
    m_pButtonRetry->runAction(Sequence::create(DelayTime::create(0.5f),
                                               MoveTo::create(0.5f, m_pButtonRetry->getPosition() - Point(0, 80)),
                                               NULL));
    levelName->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
    levelName->runAction(Sequence::create(DelayTime::create(0.5f),
                                          MoveTo::create(0.5f, Point(visibleSize.width - 10 - levelName->getContentSize().width, visibleSize.height - 74)),
                                          NULL));
    levelNumber->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 50));
    levelNumber->runAction(Sequence::create(DelayTime::create(0.5f),
                                            MoveTo::create(0.5f, Point(10 + levelNumber->getContentSize().width, visibleSize.height - 74)),
                                            CallFuncN::create(CC_CALLBACK_0(GameScene::onSceneAnimateEnd, this)),
                                            NULL));

	this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt)
{
	if (m_pPlayer && m_pParticleTail)
	{
		auto action = m_pPlayer->getActionByTag(ACTIONTAG_JUMP);
		if (!action || action->isDone())
			m_pParticleTail->setPosition(m_pPlayer->getPosition() - m_pPlayer->getContentSize() / 2);
		else
			m_pParticleTail->setPositionY(-1024);
	}
}

void GameScene::onSceneAnimateEnd()
{
    m_pButtonPause->setEnabled(true);
    m_pButtonRetry->setEnabled(true);
    // get level data
    auto roomsData = m_pCurLevelData->getRoomsData();
    m_pGameMediator->setCurGameRoom(1);
    m_pCurRoomData = &roomsData->at(0);
    
#ifdef SHOW_ALL_ROOM_MODE
    // add all room
    for (size_t i = 0, length = roomsData->size(); i < length; i++)
        this->addRoom(&roomsData->at(i));
#else
    // add first room
    this->addRoom(m_pCurRoomData);
#endif
    // add player
    this->addPlayer();
}

bool GameScene::onTouchBegan(Touch *touch, Event *event)
{
	//Point point = touch->getLocation();
	if (m_pPlayer && m_pPlayer->jump())
	{
		m_LastJumpPoint = Point(m_pPlayer->getPositionX() + m_pPlayer->getContentSize().width / 2, m_pPlayer->getPositionY() - m_pPlayer->getContentSize().height / 2 + 2);
	}

	return true;
}

void GameScene::onTouchMoved(Touch *touch, Event *event)
{
	//Point point = touch->getLocation();
}

void GameScene::onTouchEnded(Touch *touch, Event *event)
{
}

void GameScene::buttonCallback_Pause(Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
	// Go through all child of Game class and draw in renderTexture
	// It's like screenshot
	renderTexture->begin();
	Director::getInstance()->getRunningScene()->visit();
	renderTexture->end();

	Director::getInstance()->getRenderer()->render();// Must add this for version 3.0 or image goes black  
	Director::getInstance()->getTextureCache()->addImage(renderTexture->newImage(), "GamePauseImage");

	Director::getInstance()->pushScene(GamePauseScene::createScene());
}

void GameScene::buttonCallback_Retry(Ref* pSender)
{
	// clear all data
	m_nDeadNumber = 0;
	if (m_nDeadNumberMin < 0)
		m_pTextDeadNum->setString("0");
	else
		m_pTextDeadNum->setString(StringUtils::format("0/%d", m_nDeadNumberMin));
	
	if (m_pPlayer)
	{
		m_pPlayer->die();
		m_pPlayer = nullptr;
		m_pParticleTail->removeFromParent();
		m_pParticleTail = nullptr;
	}
	for (size_t i = 0; i < m_nRoomCount; i++)
		this->removeChildByName("room");
	m_vEnemys.clear();
	m_nRoomCount = 0;

	this->removeChildByName("LastContactPoint");
	this->removeChildByName("LastJumpPoint");

	// get room data
	auto roomsData = m_pCurLevelData->getRoomsData();
	m_pGameMediator->setCurGameRoom(1);
	m_pCurRoomData = &roomsData->at(0);

#ifdef SHOW_ALL_ROOM_MODE
	// add all room
	for (size_t i = 0, length = roomsData->size(); i < length; i++)
		this->addRoom(&roomsData->at(i));
#else
	// add first room
	this->addRoom(m_pCurRoomData);
#endif

	// add player
	this->addPlayer();
}

bool GameScene::onContactBegin(const PhysicsContact& contact)
{
	if (!m_pPlayer)
		return false;

	auto body1 = contact.getShapeA()->getBody();
	auto body2 = contact.getShapeB()->getBody();
	if (!body1->getNode() || !body2->getNode())
		return false;

	if ((body1->getCategoryBitmask() & MASK_PLAYER) == MASK_PLAYER ||
		(body2->getCategoryBitmask() & MASK_PLAYER) == MASK_PLAYER)
	{
		this->runAction(CSCClass::Shake::create(0.1f, 3));

		auto particle = ParticleExplosion::createWithTotalParticles(130);
		particle->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("playerImage"));
		particle->setStartSize(5);
		particle->setStartSizeVar(5);
		particle->setStartColor(m_pPlayer->getPlayerColor());
		particle->setStartColorVar(Color4F(0, 0, 0, 0));
		particle->setEndColor(m_pPlayer->getPlayerColor());
		particle->setEndColorVar(Color4F(0, 0, 0, 0));
		particle->setSpeed(500);
		particle->setSpeedVar(300);
		particle->setLife(0.5f);
		particle->setLifeVar(0.2f);
		auto contactPoint = contact.getContactData()->points[0];
		particle->setPosition(contactPoint);
		this->addChild(particle, 5);

		m_pPlayer->die();
		m_pPlayer = nullptr;
		m_pParticleTail->removeFromParent();
		m_pParticleTail = nullptr;
		m_nDeadNumber++;
		if (m_nDeadNumberMin < 0)
			m_pTextDeadNum->setString(StringUtils::format("%d", m_nDeadNumber));
		else
			m_pTextDeadNum->setString(StringUtils::format("%d/%d", m_nDeadNumber, m_nDeadNumberMin));
        
        m_pTextDeadNum->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1f, 1.5f), ScaleTo::create(0.1f,1)));

		this->runAction(Sequence::createWithTwoActions(
			DelayTime::create(0.01f),
			CallFuncN::create([=](Ref* pSender)->void
		{
			this->resetRoom(m_pCurRoomData->id);
			this->addPlayer();
		})));

		auto color_contact = Color4F::RED;
		auto color_jump = Color4F::BLUE;
		if (CSCClass::isTwoColorNear(m_pCurRoomData->color, Color3B::RED) || CSCClass::isTwoColorNear(m_pCurRoomData->color, Color3B::BLUE))
		{
			color_contact = Color4F::WHITE;
			color_jump = Color4F::GREEN;
		}

		// show last contact point
		this->removeChildByName("LastContactPoint");
		DrawNode* point_contact = DrawNode::create();
		point_contact->drawPoint(contactPoint, 4, color_contact);
		this->addChild(point_contact, 98, "LastContactPoint");

		// show last dead jump point
		this->removeChildByName("LastJumpPoint");
		DrawNode* point_jump = DrawNode::create();
		point_jump->drawPoint(m_LastJumpPoint, 4, color_jump);
		this->addChild(point_jump, 98, "LastJumpPoint");
	}

	return false;
}

void GameScene::addRoom(RoomData* roomData)
{
	// set black layer position
	m_pBlackLayer->setPositionY(m_pBlackLayer->getPositionY() + roomData->size.height);

	// background
	DrawNode* background = DrawNode::create();
	background->drawSolidRect(Point::ZERO, roomData->size, Color4F(roomData->color));
	background->setPosition(roomData->position);
	this->addChild(background, 4 - roomData->id, "room");
	m_nRoomCount++;

	// enemys
	auto enemysData = &roomData->enemysData;
	for (size_t i = 0, j = enemysData->size(); i < j; i++)
	{
		auto enemyData = &enemysData->at(i);
		Enemy* enemy = Enemy::create(enemyData->size, roomData->enemy_color, -1, roomData->id);
		auto actionsData = &enemyData->actionsData;
		enemy->setActions(actionsData);
		enemy->setPosition(enemyData->position);
		background->addChild(enemy);
		m_vEnemys.pushBack(enemy);
	}
}

void GameScene::resetRoom(int id)
{
	// reset enemys
	auto room_id = m_pCurRoomData->id;
	for (size_t i = 0, length = m_vEnemys.size(); i < length; i++)
	{
		auto enemy = m_vEnemys.at(i);
		if (enemy->getRoomID() == room_id)
			enemy->restartActions();
	}
}

void GameScene::addPlayer()
{
	CS_RETURN_IF(m_pPlayer); // in case there will be more than one player

	m_pPlayer = Player::create(m_pCurRoomData->player_color, m_pCurRoomData->player_speed, m_pCurRoomData->player_jumpTime);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto screenWidth = visibleSize.width;
#ifdef LEVEL_MAKER_MODE
	screenWidth = screenWidth / 2;
#endif
	float duration = screenWidth / m_pCurRoomData->player_speed;
	Size playerSize = m_pPlayer->getContentSize();
	float posX = playerSize.width / 2;
	float posY = m_pCurRoomData->position.y + playerSize.height / 2;
	m_pPlayer->setPosition(posX, posY);
	m_pPlayer->runAction(Sequence::createWithTwoActions(
		MoveTo::create(duration, Vec2(posX + screenWidth, posY)),
		CallFuncN::create([=](Ref* pSender)->void
	{
		m_pPlayer->die();
		m_pPlayer = nullptr;
		m_pParticleTail->removeFromParent();
		m_pParticleTail = nullptr;
		int roomIndex = m_pGameMediator->getCurGameRoom();
		if (roomIndex < m_pCurLevelData->getRoomsData()->size())
		{
			// next room
			m_pCurRoomData = &m_pCurLevelData->getRoomsData()->at(roomIndex++);
			m_pGameMediator->gotoNextGameRoom();
#ifndef SHOW_ALL_ROOM_MODE
			this->addRoom(m_pCurRoomData);
#else
			this->resetRoom(m_pCurRoomData->id);
#endif
			this->addPlayer();
		}
		else
		{
			// the last room
			m_bIsLevelFinished = true;
			RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
			// Go through all child of Game class and draw in renderTexture
			// It's like screenshot
			renderTexture->begin();
			Director::getInstance()->getRunningScene()->visit();
			renderTexture->end();

			Director::getInstance()->getRenderer()->render();// Must add this for version 3.0 or image goes black  
			Director::getInstance()->getTextureCache()->addImage(renderTexture->newImage(), "GameOverImage");

			auto story = m_pGameMediator->getLevelStoryLines(m_pGameMediator->getCurGameLevel());
			if (story)
				if (story->end == 0)
				{
					auto storyScene = StoryScene::createScene();
					if (storyScene && m_nDeadNumber == 0)
						Director::getInstance()->pushScene(storyScene);
					else
						Director::getInstance()->replaceScene(GameOverScene::createScene(m_nDeadNumber));
				}
				else
				{
					auto storyScene = StoryScene::createScene();
					if (storyScene)
						Director::getInstance()->pushScene(storyScene);
				}
			else
				Director::getInstance()->replaceScene(GameOverScene::createScene(m_nDeadNumber));
		}
	})));
	this->addChild(m_pPlayer, 4);

	// Add tail particles
	m_pParticleTail = ParticleMeteor::createWithTotalParticles(150);
	m_pParticleTail->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey("playerImage"));
	m_pParticleTail->setBlendAdditive(false);
	m_pParticleTail->setAngle(175);
	m_pParticleTail->setAngleVar(5);
	m_pParticleTail->setStartSize(3);
	m_pParticleTail->setStartSizeVar(2);
	m_pParticleTail->setStartColor(m_pPlayer->getPlayerColor());
	m_pParticleTail->setStartColorVar(Color4F(1, 1, 1, 0));
	m_pParticleTail->setEndColor(m_pPlayer->getPlayerColor());
	m_pParticleTail->setEndColorVar(Color4F(1, 1, 1, 0));
	m_pParticleTail->setSpeed(m_pCurRoomData->player_speed);
	m_pParticleTail->setSpeedVar(0);
	m_pParticleTail->setLife(0.15f);
	m_pParticleTail->setLifeVar(0.1f);
	this->addChild(m_pParticleTail, 5);
}
