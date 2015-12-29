#include "GameScene.h"
#include "CSCClass\CSCAction\Shake.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"

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

	m_nDeadNumber = 0;
    /////////////////////////////
	m_pGameMediator = GameMediator::getInstance();
	// get level data
	auto levelsData = m_pGameMediator->getGameLevelData();
	int curLevel = m_pGameMediator->getCurGameLevel();
	m_pCurLevelData = levelsData->at(curLevel - 1);
	auto roomsData = m_pCurLevelData->getRoomsData();
	m_nCurRoomIndex = 1;
	m_pCurRoomData = &roomsData->at(0);

	// load scene
	auto rootNode = CSLoader::createNode("GameScene.csb");
	this->addChild(rootNode);
	// get button
	auto buttonReturn = dynamic_cast<Button*>(rootNode->getChildByName("Button_Return"));
	buttonReturn->addClickEventListener(CC_CALLBACK_1(GameScene::buttonCallback_MainMenu, this));
	// get text
	m_pTextDeadNum = dynamic_cast<Text*>(rootNode->getChildByName("Text_DeadNumber"));
	m_pTextDeadNum->setString(StringUtils::format("%d", m_nDeadNumber));
	Text* levelName = dynamic_cast<Text*>(rootNode->getChildByName("Text_LevelName"));
	levelName->setString(m_pCurLevelData->getLevelName());
	Text* levelNumber = dynamic_cast<Text*>(rootNode->getChildByName("Text_LevelNumber"));
	levelNumber->setString(StringUtils::format("%d/%d", curLevel, m_pGameMediator->getMaxGameLevel()));

	// add first room
	this->addRoom(m_pCurRoomData);

	// add player
	this->addPlayer(m_pCurRoomData);

	//this->scheduleUpdate();

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

void GameScene::buttonCallback_MainMenu(Ref* pSender)
{
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
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
		m_nDeadNumber++;
		m_pTextDeadNum->setString(StringUtils::format("%d", m_nDeadNumber));

		this->runAction(Sequence::createWithTwoActions(
			DelayTime::create(0.5f),
			CallFuncN::create([=](Ref* pSender)->void
		{
			this->addPlayer(m_pCurRoomData);
		})));
	}

	return false;
}

// schedule functions
void GameScene::update(float dt)
{
}

void GameScene::addRoom(RoomData* roomData)
{
	// background
	DrawNode* background = DrawNode::create();
	background->drawSolidRect(Point::ZERO, roomData->size, Color4F(roomData->color));
	background->setPosition(roomData->position);
	this->addChild(background);

	// enemys
	auto enemysData = &roomData->enemysData;
	for (size_t i = 0, j = enemysData->size(); i < j; i++)
	{
		auto enemyData = &enemysData->at(i);
		auto enemy = Enemy::create(enemyData->size, roomData->enemy_color, i);
		enemy->setPosition(enemyData->position);
		background->addChild(enemy);
		m_vEnemys.pushBack(enemy);
	}
}

void GameScene::addPlayer(RoomData* roomData)
{
	m_pPlayer = Player::create(roomData->player_color, roomData->player_speed, roomData->player_jumpTime);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto screenWidth = visibleSize.width;
#if (DEBUG_FLAG == 1)
	screenWidth = screenWidth / 2;
#endif
	float duration = screenWidth / roomData->player_speed;
	Size playerSize = m_pPlayer->getContentSize();
	float posX = playerSize.width / 2;
	float posY = roomData->position.y + playerSize.height / 2;
	m_pPlayer->setPosition(posX, posY);
	m_pPlayer->runAction(Sequence::createWithTwoActions(
		MoveTo::create(duration, Vec2(posX + screenWidth, posY)),
		CallFuncN::create([=](Ref* pSender)->void
	{
		m_pPlayer->die();
		if (m_nCurRoomIndex < m_pCurLevelData->getRoomsData()->size())
		{
			// next room
			m_pCurRoomData = &m_pCurLevelData->getRoomsData()->at(m_nCurRoomIndex++);
			this->addRoom(m_pCurRoomData);
			this->addPlayer(m_pCurRoomData);
		}
		else
		{
			// the last room
			RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
			// Go through all child of Game class and draw in renderTexture
			// It's like screenshot
			renderTexture->begin();
			Director::getInstance()->getRunningScene()->visit();
			renderTexture->end();

			Director::getInstance()->getRenderer()->render();// Must add this for version 3.0 or image goes black  
			Director::getInstance()->getTextureCache()->addImage(renderTexture->newImage(), "GameOverImage");

			Director::getInstance()->replaceScene(GameOverScene::createScene(m_nDeadNumber));
		}
	})));
	this->addChild(m_pPlayer);
}
