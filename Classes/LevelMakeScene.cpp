#include "LevelMakeScene.h"
#include "CSCClass\CommonFunctions.h"

LevelMakeScene::LevelMakeScene()
	: m_pPlayer(nullptr)
	, m_pDropDownListLevel(nullptr)
	, m_nCurEnemyId(0)
	, m_nCurJumpPointId(0)
	, m_bIsTrying(false)
	, m_bIsTouchEnemy(false)
	, m_touchOffset(Vec2(0,0))
{
	m_mTextFieldStructs.clear();
	m_vEnemys.clear();
	m_vJumpPoints.clear();
	m_vJumpPointsForAutoTrying.clear();
}

LevelMakeScene::~LevelMakeScene()
{
	m_mTextFieldStructs.clear();
	m_vEnemys.clear();
	m_vJumpPoints.clear();
	m_vJumpPointsForAutoTrying.clear();
}

Scene* LevelMakeScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	// open Debug
	PhysicsWorld* world = scene->getPhysicsWorld();
	world->setGravity(Vec2(0, 0));
	//world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = LevelMakeScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
void LevelMakeScene::onEnter()
{
	Layer::onEnter();
	// bind contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(LevelMakeScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	// bind touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(LevelMakeScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(LevelMakeScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(LevelMakeScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool LevelMakeScene::init()
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

	auto rootNode = CSLoader::createNode("LevelMakeScene.csb");
	this->addChild(rootNode);

	// get button
	auto buttonAddBlock = dynamic_cast<Button*>(rootNode->getChildByName("Button_AddBlock"));
	auto buttonRemoveBlock = dynamic_cast<Button*>(rootNode->getChildByName("Button_RemoveBlock"));
	auto buttonCalcFoothold = dynamic_cast<Button*>(rootNode->getChildByName("Button_CalcFoothold"));
	auto buttonTry = dynamic_cast<Button*>(rootNode->getChildByName("Button_Try"));
	auto buttonExport = dynamic_cast<Button*>(rootNode->getChildByName("Button_ExportLevel"));
	auto buttonImport = dynamic_cast<Button*>(rootNode->getChildByName("Button_ImportLevel"));
	buttonAddBlock->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_AddBlock, this));
	buttonRemoveBlock->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_RemoveBlock, this));
	buttonCalcFoothold->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_CalcFoothold, this));
	buttonTry->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Try, this));
	buttonExport->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Export, this));
	buttonImport->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Import, this));

	// get level panel
	auto panelLevel = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Level"));
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Stage_ID")), TAG_LEVEL_STAGE_ID, false);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Level_ID")), TAG_LEVEL_LEVEL_ID, false);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Width")), TAG_LEVEL_WIDTH);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Height")), TAG_LEVEL_HEIGHT);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Color_R")), TAG_LEVEL_COLOR_R);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Color_G")), TAG_LEVEL_COLOR_G);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Color_B")), TAG_LEVEL_COLOR_B);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Block_Color_R")), TAG_ENEMY_COLOR_R);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Block_Color_G")), TAG_ENEMY_COLOR_G);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Block_Color_B")), TAG_ENEMY_COLOR_B);

	// get block panel
	auto panelBlock = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Block"));
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_Width")), TAG_ENEMY_WIDTH);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_Height")), TAG_ENEMY_HEIGHT);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_PositionX")), TAG_ENEMY_POSITION_X);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_PositionY")), TAG_ENEMY_POSITION_Y);

	// get player panel
	auto panelPlayer = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Player"));
	this->getLevelNode(dynamic_cast<Node*>(panelPlayer->getChildByName("Node_Speed")), TAG_PLAYER_SPEED);
	this->getLevelNode(dynamic_cast<Node*>(panelPlayer->getChildByName("Node_JumpDuration")), TAG_PLAYER_JUMPTIME10);

	// get level choose panel
	auto panelLevelChoose = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_LevelChoose"));
	m_pDropDownListLevel = CSCClass::DropDownList::create(Label::createWithSystemFont("1", "Arial", 22), Size(80, 30));
	m_pDropDownListLevel->addLabel(Label::createWithSystemFont("2", "Arial", 22));
	m_pDropDownListLevel->addLabel(Label::createWithSystemFont("3", "Arial", 22));
	m_pDropDownListLevel->setPosition((panelLevelChoose->getContentSize() - m_pDropDownListLevel->getContentSize()) / 2);
	panelLevelChoose->addChild(m_pDropDownListLevel);


	// set initial number
	int levelWidth = visibleSize.width / 2;
	int levelHeight = visibleSize.height*0.2;
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_STAGE_ID), 0, 1, 999999);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_LEVEL_ID), 0, 1, 999999);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_WIDTH), 0, levelWidth, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_HEIGHT), 0, levelHeight, visibleSize.height);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_COLOR_R), 0, 95, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_COLOR_G), 0, 102, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_COLOR_B), 0, 120, 255);

	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R), 0, 125, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G), 0, 149, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B), 0, 153, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_WIDTH), 0, 50, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_HEIGHT), 0, 50, levelHeight);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_POSITION_X), 0, levelWidth*0.5, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_POSITION_Y), 0, 0, levelHeight);

	this->initStruct(&m_mTextFieldStructs.at(TAG_PLAYER_SPEED), 0, levelWidth*0.2, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10), 0, 8, 20);

	// add line between gameplay screen & edit screen
	DrawNode* line = DrawNode::create();
	line->drawLine(Vec2(levelWidth, 0), Vec2(levelWidth, visibleSize.height), Color4F::WHITE);
	this->addChild(line);

	this->addLevelBack();
	this->addEnemy();
	this->addPlayer();

	this->calcFoothold();

	this->scheduleUpdate();

    return true;
}

// event functions
bool LevelMakeScene::onTouchBegan(Touch *touch, Event *event)
{
	if (!m_bIsTrying)
	{
		Point point = touch->getLocation();  // get touch point
		for (ssize_t i = 0; i < m_vEnemys.size(); i++)
		{
			Enemy* enemy = m_vEnemys.at(i);
			if (enemy->getBoundingBox().containsPoint(point))
			{
				m_vEnemys.at(m_nCurEnemyId)->unSelect();
				m_nCurEnemyId = i;
				enemy->select();
				m_bIsTouchEnemy = true;
				Point position = enemy->getPosition();
				m_touchOffset = position - point;
				updateBlockTextFieldNumber(TAG_ENEMY_WIDTH, enemy->getContentSize().width);
				updateBlockTextFieldNumber(TAG_ENEMY_HEIGHT, enemy->getContentSize().height);
				updateBlockTextFieldNumber(TAG_ENEMY_POSITION_X, position.x);
				updateBlockTextFieldNumber(TAG_ENEMY_POSITION_Y, position.y);
			}
		}
	}
	return true;
}

void LevelMakeScene::onTouchMoved(Touch *touch, Event *event)
{
	if (m_bIsTouchEnemy)
	{
		Point point = touch->getLocation();  // get touch point
		Point position = point + m_touchOffset;
		int minX = m_mTextFieldStructs.at(TAG_ENEMY_WIDTH).number / 2;
		int maxX = m_mTextFieldStructs.at(TAG_LEVEL_WIDTH).number - minX;
		int maxY = m_mTextFieldStructs.at(TAG_LEVEL_HEIGHT).number - m_mTextFieldStructs.at(TAG_ENEMY_HEIGHT).number;
		position.x = MAX(MIN(position.x, maxX), minX);
		position.y = MAX(MIN(position.y, maxY), 0);
		m_vEnemys.at(m_nCurEnemyId)->setPosition(position);
		updateBlockTextFieldNumber(TAG_ENEMY_POSITION_X, position.x);
		updateBlockTextFieldNumber(TAG_ENEMY_POSITION_Y, position.y);
		this->calcFoothold();
	}
}

void LevelMakeScene::onTouchEnded(Touch *touch, Event *event)
{
	if (m_bIsTrying)
	{
		if (m_pPlayer)
		{
			m_pPlayer->jump();
		}
	}
	else
	{
		if (m_bIsTouchEnemy)
		{
			m_bIsTouchEnemy = false;
			m_touchOffset = Vec2(0, 0);
		}
	}
}

void LevelMakeScene::onTextFieldEvent(Ref* pSender, TextField::EventType type)
{
	TextField* field = dynamic_cast<TextField*>(pSender);
	char buf[10];
	bool isChanged = false;

	switch (type)
	{
	case TextField::EventType::ATTACH_WITH_IME:
		return;
	case TextField::EventType::DETACH_WITH_IME:
		isChanged = true;
		break;
	case TextField::EventType::INSERT_TEXT:
	{
		// format input
		string strHolder = field->getPlaceHolder();
		int pos = strHolder.find("-");
		sprintf(buf, "%d", 
			MAX(MIN(
				atoi(CSCClass::eraseStringNonNumber(field->getString()).c_str()), 
				atoi(strHolder.substr(pos + 1, strHolder.size() - pos - 1).c_str())
				), 0));
		field->setString(buf);

		break;
	}
	case TextField::EventType::DELETE_BACKWARD:
		break;
	default:
		break;
	}

	if (isChanged)
	{
		// set attribute
		int tag = field->getTag();
		TextFieldSliderBindInt* structTmp = &m_mTextFieldStructs.at(tag);
		structTmp->number = atoi(field->getString().c_str());
		if (structTmp->slider)
		{
			structTmp->slider->setPercent(structTmp->number * 100 / structTmp->max);
		}
		this->updateBlockByLevel(tag, structTmp->number);

		// update object
		this->updateLevelOrEnemys(tag);
		this->updateCurEnemy(tag);
		this->updatePlayer(tag);

		this->calcFoothold();
	}
}

void LevelMakeScene::onSliderEvent(Ref* pSender, Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		// set attribute
		Slider* slider = dynamic_cast<Slider*>(pSender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int tag = slider->getTag();
		TextFieldSliderBindInt* structTmp = &m_mTextFieldStructs.at(tag);
		structTmp->number = percent * (structTmp->max - structTmp->min) / maxPercent + structTmp->min;
		char buf[10];
		sprintf(buf, "%d", structTmp->number);
		structTmp->textField->setString(buf);
		this->updateBlockByLevel(tag, structTmp->number);

		// update object
		this->updateLevelOrEnemys(tag);
		this->updateCurEnemy(tag);
		this->updatePlayer(tag);

		this->calcFoothold();
	}
}

bool LevelMakeScene::onContactBegin(const PhysicsContact& contact)
{
	if (!m_bIsTrying)
		return false;

	auto body1 = contact.getShapeA()->getBody();
	auto body2 = contact.getShapeB()->getBody();
	if (!body1->getNode() || !body2->getNode())
	{
		return false;
	}

	if ((body1->getCategoryBitmask() & MASK_PLAYER) == MASK_PLAYER ||
		(body2->getCategoryBitmask() & MASK_PLAYER) == MASK_PLAYER)
	{
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
		m_pPlayer = nullptr;

		this->runAction(Sequence::createWithTwoActions(
			DelayTime::create(0.5f),
			CallFuncN::create([=](Ref* pSender)->void
		{
			this->addPlayerForTrying();
		})));
	}

	return false;
}

// schedule functions
void LevelMakeScene::update(float dt)
{
	if (m_bIsTrying && m_pPlayer)
	{
		if (m_nCurJumpPointId < m_vJumpPointsForAutoTrying.size())
		{
			float rangePerFrame = m_pPlayer->getSpeed() / Director::getInstance()->getFrameRate();
			float rightPt = m_pPlayer->getPositionX() + m_pPlayer->getContentSize().width / 2;
			float jumpPt = m_vJumpPointsForAutoTrying.at(m_nCurJumpPointId);
			if (rightPt + rangePerFrame >= jumpPt && rightPt < jumpPt)
			{
				m_pPlayer->jump();
				m_nCurJumpPointId++;
			}
			else if (rightPt > jumpPt)
			{
				m_nCurJumpPointId++;
			}
		}
	}
}

// callback functions
void LevelMakeScene::buttonCallback_AddBlock(Ref* pSender)
{
	this->addEnemy();
	this->calcFoothold();
}

void LevelMakeScene::buttonCallback_RemoveBlock(Ref* pSender)
{
	if (m_vEnemys.size() > 0 && m_nCurEnemyId >= 0)
	{
		m_vEnemys.at(m_nCurEnemyId)->removeFromParent();
		m_vEnemys.erase(m_nCurEnemyId);
		if (m_nCurEnemyId > 0)
		{
			m_nCurEnemyId--;
			m_vEnemys.at(m_nCurEnemyId)->select();
		}
		this->calcFoothold();
	}
}

void LevelMakeScene::buttonCallback_CalcFoothold(Ref* pSender)
{
	this->calcFoothold();
}

void LevelMakeScene::buttonCallback_Try(Ref* pSender)
{
	Button* button = dynamic_cast<Button*>(pSender);
	if (m_bIsTrying) // stop trying
	{
		button->setTitleText("Try");
		m_bIsTrying = false;
		for (auto obj : m_mTextFieldStructs)
		{
			obj.second.textField->setEnabled(true);
			if (obj.second.slider)
			{
				obj.second.slider->setEnabled(true);
			}
		}

		this->addPlayer();
	}
	else // start to try
	{
		m_bIsTouchEnemy = false;
		button->setTitleText("Stop");
		m_bIsTrying = true;
		for (auto obj : m_mTextFieldStructs)
		{
			obj.second.textField->setEnabled(false);
			if (obj.second.slider)
			{
				obj.second.slider->setEnabled(false);
			}
		}

		this->addPlayerForTrying();
	}
}

void LevelMakeScene::buttonCallback_Export(Ref* pSender)
{
	do
	{
		tinyxml2::XMLDocument *document = new tinyxml2::XMLDocument();
		CC_BREAK_IF(!document);

		XMLDeclaration *declaration = document->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
		CC_BREAK_IF(!declaration);
		document->LinkEndChild(declaration);

		XMLElement *root = document->NewElement("GameLevel");
		document->LinkEndChild(root);

		XMLElement *surface1 = document->NewElement("Level");
		surface1->SetAttribute("level", m_mTextFieldStructs.at(TAG_LEVEL_LEVEL_ID).number);
		surface1->SetAttribute("stage", m_mTextFieldStructs.at(TAG_LEVEL_STAGE_ID).number);
		surface1->SetAttribute("width", m_mTextFieldStructs.at(TAG_LEVEL_WIDTH).number);
		surface1->SetAttribute("height", m_mTextFieldStructs.at(TAG_LEVEL_HEIGHT).number);
		surface1->SetAttribute("color_r", m_mTextFieldStructs.at(TAG_LEVEL_COLOR_R).number);
		surface1->SetAttribute("color_g", m_mTextFieldStructs.at(TAG_LEVEL_COLOR_G).number);
		surface1->SetAttribute("color_b", m_mTextFieldStructs.at(TAG_LEVEL_COLOR_B).number);
		root->LinkEndChild(surface1);

		XMLElement *surface2 = document->NewElement("Player");
		surface2->SetAttribute("speed", m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number);
		surface2->SetAttribute("jumpTime", static_cast<float>(m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10).number) * 0.1f);
		surface1->LinkEndChild(surface2);

		surface2 = document->NewElement("Enemys");
		surface2->SetAttribute("color_r", m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number);
		surface2->SetAttribute("color_g", m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number);
		surface2->SetAttribute("color_b", m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number);
		surface1->LinkEndChild(surface2);

		for (size_t i = 0, j = m_vEnemys.size(); i < j; i++)
		{
			XMLElement *surface3 = document->NewElement("Enemy");
			surface3->SetAttribute("id", i + 1);
			Size size = m_vEnemys.at(i)->getContentSize();
			surface3->SetAttribute("width", size.width);
			surface3->SetAttribute("height", size.height);
			Point pos = m_vEnemys.at(i)->getPosition();
			surface3->SetAttribute("x", static_cast<int>(pos.x));
			surface3->SetAttribute("y", static_cast<int>(pos.y));
			surface2->LinkEndChild(surface3);
		}

		string filename = FileUtils::getInstance()->fullPathForFilename("config/LevelMake_Workstation.xml");
		document->SaveFile(filename.c_str());
	} while (false);
}

void LevelMakeScene::buttonCallback_Import(Ref* pSender)
{
	do
	{
		// load from file
		tinyxml2::XMLDocument document;
		string filename = FileUtils::getInstance()->fullPathForFilename("config/LevelMake_Workstation.xml");
		document.LoadFile(filename.c_str());
		XMLElement* root = document.RootElement();
		CC_BREAK_IF(!root);

		// load workstation
		XMLElement* surface1 = root->FirstChildElement("Level");
		CC_BREAK_IF(!surface1);
		this->updateBlockTextFieldNumber(TAG_LEVEL_LEVEL_ID, surface1->IntAttribute("level"));
		this->updateBlockTextFieldNumber(TAG_LEVEL_STAGE_ID, surface1->IntAttribute("stage"));
		int levelWidth = surface1->IntAttribute("width");
		this->updateBlockTextFieldNumber(TAG_LEVEL_WIDTH, levelWidth);
		this->updateBlockByLevel(TAG_LEVEL_WIDTH, levelWidth);
		int levelHeight = surface1->IntAttribute("height");
		this->updateBlockTextFieldNumber(TAG_LEVEL_HEIGHT, levelHeight);
		this->updateBlockByLevel(TAG_LEVEL_HEIGHT, levelHeight);
		this->updateBlockTextFieldNumber(TAG_LEVEL_COLOR_R, surface1->IntAttribute("color_r"));
		this->updateBlockTextFieldNumber(TAG_LEVEL_COLOR_G, surface1->IntAttribute("color_g"));
		this->updateBlockTextFieldNumber(TAG_LEVEL_COLOR_B, surface1->IntAttribute("color_b"));
		// update level
		this->removeChildByTag(TAG_BACKGROUND);
		this->addLevelBack();

		XMLElement* surface2 = surface1->FirstChildElement("Player");
		CC_BREAK_IF(!surface2);
		this->updateBlockTextFieldNumber(TAG_PLAYER_SPEED, surface2->IntAttribute("speed"));
		this->updateBlockTextFieldNumber(TAG_PLAYER_JUMPTIME10, static_cast<int>(surface2->FloatAttribute("jumpTime")) * 10);
		// update player
		this->addPlayer();

		surface2 = surface1->FirstChildElement("Enemys");
		CC_BREAK_IF(!surface2);
		int color_r = surface2->IntAttribute("color_r");
		int color_g = surface2->IntAttribute("color_g");
		int color_b = surface2->IntAttribute("color_b");
		this->updateBlockTextFieldNumber(TAG_ENEMY_COLOR_R, color_r);
		this->updateBlockTextFieldNumber(TAG_ENEMY_COLOR_G, color_g);
		this->updateBlockTextFieldNumber(TAG_ENEMY_COLOR_B, color_b);
		Color3B enemyColor = Color3B(color_r, color_g, color_b);
		// clear all enemys
		for (auto obj : m_vEnemys)
		{
			obj->removeFromParent();
		}
		m_vEnemys.clear();
		m_nCurEnemyId = 0;
		// add enemys
		for (XMLElement* surface3 = surface2->FirstChildElement("Enemy"); surface3 != NULL; surface3 = surface3->NextSiblingElement("Enemy"))
		{
			Enemy* enemy = Enemy::create(Size(surface3->IntAttribute("width"), surface3->IntAttribute("height")),
				enemyColor, surface3->IntAttribute("id"));
			enemy->setPosition(surface3->IntAttribute("x"), surface3->IntAttribute("y"));
			this->addChild(enemy, 1, TAG_ENEMY);
			m_vEnemys.pushBack(enemy);
		}
		// select the first one
		(*m_vEnemys.begin())->select();

		this->calcFoothold();
	} while (false);
}

// my functions
void LevelMakeScene::addLevelBack()
{
	int width = m_mTextFieldStructs.at(TAG_LEVEL_WIDTH).number;
	int height = m_mTextFieldStructs.at(TAG_LEVEL_HEIGHT).number;
	int color_r = m_mTextFieldStructs.at(TAG_LEVEL_COLOR_R).number;
	int color_g = m_mTextFieldStructs.at(TAG_LEVEL_COLOR_G).number;
	int color_b = m_mTextFieldStructs.at(TAG_LEVEL_COLOR_B).number;
	DrawNode* background = DrawNode::create();
	background->drawSolidRect(Vec2(0, 0), Vec2(width, height), Color4F(Color3B(color_r, color_g, color_b)));
	this->addChild(background, 0, TAG_BACKGROUND);
}

void LevelMakeScene::addEnemy()
{
	int width = m_mTextFieldStructs.at(TAG_ENEMY_WIDTH).number;
	int height = m_mTextFieldStructs.at(TAG_ENEMY_HEIGHT).number;
	int color_r = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number;
	int color_g = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number;
	int color_b = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number;

	m_nCurEnemyId = m_vEnemys.size();
	auto enemy = Enemy::create(Size(width, height), Color3B(color_r, color_g, color_b), m_nCurEnemyId);
	enemy->setPosition(m_mTextFieldStructs.at(TAG_ENEMY_POSITION_X).number, m_mTextFieldStructs.at(TAG_ENEMY_POSITION_Y).number);
	this->addChild(enemy, 1, TAG_ENEMY);
	m_vEnemys.pushBack(enemy);
	enemy->select();
}

void LevelMakeScene::addPlayer()
{
	if (m_pPlayer)
	{
		m_pPlayer->die();
		m_pPlayer = nullptr;
	}

	int color_r = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number;
	int color_g = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number;
	int color_b = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number;

	m_pPlayer = Player::create(Color3B(color_r, color_g, color_b));
	m_pPlayer->setPosition(m_pPlayer->getContentSize().width / 2, m_pPlayer->getContentSize().height / 2);
	m_pPlayer->setSpeed(m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number);
	m_pPlayer->setJumpDuration(m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10).number * 0.1f);
	this->addChild(m_pPlayer);
}

void LevelMakeScene::addPlayerForTrying()
{
	if (m_pPlayer)
	{
		m_pPlayer->die();
		m_pPlayer = nullptr;
	}

	m_nCurJumpPointId = 0;

	int color_r = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number;
	int color_g = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number;
	int color_b = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number;

	m_pPlayer = Player::create(Color3B(color_r, color_g, color_b));
	m_pPlayer->setSpeed(m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number);
	m_pPlayer->setJumpDuration(m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10).number * 0.1f);
	Size playerSize = m_pPlayer->getContentSize();
	float posX = playerSize.width / 2;
	float posY = playerSize.height / 2;
	int levelWidth = m_mTextFieldStructs.at(TAG_LEVEL_WIDTH).number;
	float duration = static_cast<float>(levelWidth) / static_cast<float>(m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number);
	m_pPlayer->setPosition(posX, posY);
	m_pPlayer->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		MoveTo::create(duration, Vec2(posX + levelWidth, posY)),
		CallFuncN::create([=](Ref* pSender)->void
	{
		m_pPlayer->die();
		m_pPlayer = nullptr;
		this->addPlayerForTrying();
	}))));

	this->addChild(m_pPlayer);
}

void LevelMakeScene::updateLevelOrEnemys(int tag)
{
	if (tag <= TAG_LEVEL_MIN || tag >= TAG_LEVEL_MAX)
		return;
	if (tag >= TAG_LEVEL_WIDTH && tag <= TAG_LEVEL_COLOR_B)
	{
		this->removeChildByTag(TAG_BACKGROUND);
		this->addLevelBack();
	}
	else if (tag >= TAG_ENEMY_COLOR_R && tag <= TAG_ENEMY_COLOR_B)
	{
		for (auto obj : m_vEnemys)
		{
			int color_r = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number;
			int color_g = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number;
			int color_b = m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number;
			obj->updateColor(Color3B(color_r, color_g, color_b));
		}
	}
}

void LevelMakeScene::updateCurEnemy(int tag)
{
	if (tag <= TAG_ENEMY_MIN || tag >= TAG_ENEMY_MAX)
		return;
	Enemy* obj = m_vEnemys.at(m_nCurEnemyId);
	int number = m_mTextFieldStructs.at(tag).number;
	switch (tag)
	{
	case TAG_ENEMY_WIDTH:
		obj->updateSize(Size(number, obj->getContentSize().height));
		break;
	case TAG_ENEMY_HEIGHT:
		obj->updateSize(Size(obj->getContentSize().width, number));
		break;
	case TAG_ENEMY_POSITION_X:
		obj->setPositionX(number);
		break;
	case TAG_ENEMY_POSITION_Y:
		obj->setPositionY(number);
		break;
	default:
		break;
	}
}

void LevelMakeScene::updatePlayer(int tag)
{
	if (tag <= TAG_PLAYER_MIN || tag >= TAG_PLAYER_MAX)
		return;
	if (tag == TAG_PLAYER_SPEED)
		m_pPlayer->setSpeed(m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number);
	else if (tag == TAG_PLAYER_JUMPTIME10)
		m_pPlayer->setJumpDuration(m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10).number * 0.1f);
	
}

void LevelMakeScene::calcFoothold()
{
	float jumpHeight = m_pPlayer->getJumpHeight();
	float jumpTime = m_pPlayer->getJumpDuration();
	float speed = m_pPlayer->getSpeed();
	float playerWidth = m_pPlayer->getContentSize().width;
	float jumpLength = jumpTime * speed;
	float endOffsetX = jumpLength - playerWidth;

	// calc all land point
	m_vJumpPoints.clear();
	m_vJumpPointsForAutoTrying.clear();
	for (auto obj : m_vEnemys)
	{
		// calc min start point & min end point for enemys
		Size enemySize = obj->getContentSize();
		if (enemySize.height >= jumpHeight)
			continue;
		Vec2 enemyPos = obj->getPosition();
		float tmp = sqrt(1 - enemySize.height / jumpHeight);
		float landTime1 = (1 - tmp) * jumpTime * 0.5;
		float landTime2 = jumpTime - (1 + tmp) * jumpTime * 0.5;
		float pStart = enemyPos.x - enemySize.width / 2 - landTime1 * speed;
		float pStartEnd = pStart + endOffsetX;
		float pEnd = enemyPos.x + enemySize.width / 2 + landTime2 * speed;
		float pEndStart = pEnd - endOffsetX;
		m_vJumpPoints.push_back({ pEndStart, pStart, pEnd, pStartEnd });
		m_vJumpPointsForAutoTrying.push_back(pEndStart);
		m_vJumpPointsForAutoTrying.push_back(pStart);
	}
	std::sort(m_vJumpPoints.begin(), m_vJumpPoints.end(), sortJumpPoints);
	std::sort(m_vJumpPointsForAutoTrying.begin(), m_vJumpPointsForAutoTrying.end(), sortJumpPointsForAutoTrying);

	// draw land line
	this->removeChildByTag(TAG_FOOTHOLDLINE);
	this->removeChildByTag(TAG_TIMETEXT);
	size_t length = m_vJumpPoints.size();
	if (length > 0)
	{
		Color4F colorStart = Color4F::GREEN;
		Color4F colorEnd = Color4F::BLUE;
		Color4F colorCollapse = Color4F::RED;
		int color_r = m_mTextFieldStructs.at(TAG_LEVEL_COLOR_R).number;
		int color_g = m_mTextFieldStructs.at(TAG_LEVEL_COLOR_G).number;
		int color_b = m_mTextFieldStructs.at(TAG_LEVEL_COLOR_B).number;
		if (color_r == 0 && color_g == 255 && color_b == 0)
		{
			colorStart = Color4F::RED;
			colorCollapse = Color4F::GREEN;
		}

		DrawNode* draw = DrawNode::create();
		int lineWidth = 2;
		draw->setLineWidth(lineWidth);
		
		float ctlOffsetX = jumpLength / 2;
		float ctlOffsetY = jumpHeight * 2;
		Vec2 vecCtlOffsetStart = Vec2(ctlOffsetX, ctlOffsetY);
		Vec2 vecCtlOffsetEnd = Vec2(-ctlOffsetX, ctlOffsetY);
		Vec2 vecEndOffset = Vec2(endOffsetX, 0);

		// draw 0 - p1 first width one jump line
		float p = m_vJumpPoints.at(0).pStart;
		this->drawJumpLineByStart(draw, Vec2(p, 0), vecCtlOffsetStart, vecEndOffset, colorStart);
		draw->drawLine(Vec2(0, lineWidth), Vec2(p, lineWidth), colorStart);
		// draw p1-p2 with two jump lines
		Color4F color = colorStart;
		int frameRate = Director::getInstance()->getFrameRate();
		Node* textNode = Node::create();
		char buf[10];
		for (size_t i = 1; i < length; i++)
		{
			float p1End = m_vJumpPoints.at(i - 1).pEnd;
			float p1StartEnd = m_vJumpPoints.at(i - 1).pStartEnd;
			float p2Start = m_vJumpPoints.at(i).pStart;
			float p2EndStart = m_vJumpPoints.at(i).pEndStart;
			// draw lines
			float landLength = p2Start - p1End - playerWidth;
			if (landLength < 0)
				color = colorCollapse;
			this->drawJumpLineByEnd(draw, Vec2(p1End, 0), vecCtlOffsetEnd, vecEndOffset, colorEnd);
			draw->drawLine(Vec2(p1End, lineWidth), Vec2(p2Start, lineWidth), color);
			this->drawJumpLineByStart(draw, Vec2(p2Start, 0), vecCtlOffsetStart, vecEndOffset, colorStart);
			// calc estimate time with frame number for 4-combinations
			this->AddEstimateFrameText(textNode, buf, p1End, p2Start, frameRate, playerWidth, speed, 20, Color4B(colorEnd));
			this->AddEstimateFrameText(textNode, buf, p1StartEnd, p2Start, frameRate, playerWidth, speed, 40, Color4B(colorStart));
			this->AddEstimateFrameText(textNode, buf, p1End, p2EndStart, frameRate, playerWidth, speed, 60, Color4B(colorEnd));
			this->AddEstimateFrameText(textNode, buf, p1StartEnd, p2EndStart, frameRate, playerWidth, speed, 80, Color4B(colorStart));
		}
		this->addChild(textNode, 1, TAG_TIMETEXT);
		// draw last p - width with one jump line
		int width = m_mTextFieldStructs.at(TAG_LEVEL_WIDTH).number;
		p = m_vJumpPoints.at(length - 1).pEnd;
		this->drawJumpLineByEnd(draw, Vec2(p, 0), vecCtlOffsetEnd, vecEndOffset, colorEnd);
		draw->drawLine(Vec2(p, lineWidth), Vec2(width, lineWidth), colorEnd);

		// draw start point for trying
		for (auto obj : m_vJumpPointsForAutoTrying)
		{
			draw->drawDot(Vec2(obj, 2), 4, Color4F::RED);
		}
		this->addChild(draw, 1, TAG_FOOTHOLDLINE);
	}
}

// inline functions
void LevelMakeScene::getLevelNode(Node* root, int tag, bool isWithSlider)
{
	TextFieldSliderBindInt structTmp;
	structTmp.textField = dynamic_cast<TextField*>(root->getChildByName("TextField"));
	structTmp.textField->addEventListener(CC_CALLBACK_2(LevelMakeScene::onTextFieldEvent, this));
	structTmp.textField->setTag(tag);
	if (isWithSlider)
	{
		structTmp.slider = dynamic_cast<Slider*>(root->getChildByName("Slider"));
		structTmp.slider->addEventListener(CC_CALLBACK_2(LevelMakeScene::onSliderEvent, this));
		structTmp.slider->setTag(tag);
	}
	else
	{
		structTmp.slider = nullptr;
	}
	m_mTextFieldStructs.insert(pair<int, TextFieldSliderBindInt>(tag, structTmp));
}

void LevelMakeScene::initStruct(TextFieldSliderBindInt* structTmp, int min, int number, int max)
{
	structTmp->min = min;
	structTmp->number = number;
	structTmp->max = max;
	char buf[10];
	sprintf(buf, "0-%d", static_cast<int>(max));
	structTmp->textField->setPlaceHolder(buf);
	sprintf(buf, "%d", static_cast<int>(number));
	structTmp->textField->setString(buf);
	if (structTmp->slider)
	{
		structTmp->slider->setPercent(number * 100 / max);
	}
}

void LevelMakeScene::updateBlockTextFieldNumber(int tag, int number)
{
	TextFieldSliderBindInt* structBlock = &m_mTextFieldStructs.at(tag);
	structBlock->number = MIN(number, structBlock->max);
	char buf[10];
	sprintf(buf, "%d", structBlock->number);
	structBlock->textField->setString(buf);
	if (structBlock->slider)
	{
		structBlock->slider->setPercent(structBlock->number * 100 / structBlock->max);
	}
}

void LevelMakeScene::updateBlockTextFieldMax(int tag, int max)
{
	TextFieldSliderBindInt* structBlock = &m_mTextFieldStructs.at(tag);
	structBlock->max = max;
	if (structBlock->number > structBlock->max)
	{
		structBlock->number = structBlock->max;
		char buf[10];
		sprintf(buf, "%d", structBlock->number);
		structBlock->textField->setString(buf);
	}
	if (structBlock->slider)
	{
		structBlock->slider->setPercent(structBlock->number * 100 / structBlock->max);
	}
}

void LevelMakeScene::updateBlockByLevel(int tag, int number)
{
	switch (tag)
	{
	case TAG_LEVEL_WIDTH:
		this->updateBlockTextFieldMax(TAG_ENEMY_WIDTH, number);
		this->updateBlockTextFieldMax(TAG_ENEMY_POSITION_X, number);
		break;
	case TAG_LEVEL_HEIGHT:
		this->updateBlockTextFieldMax(TAG_ENEMY_HEIGHT, number);
		this->updateBlockTextFieldMax(TAG_ENEMY_POSITION_Y, number);
		break;
	default:
		break;
	}
}

void LevelMakeScene::drawJumpLineByStart(DrawNode* draw, Vec2 start, Vec2 ctlOffset, Vec2 endOffset, Color4F color)
{
	draw->drawQuadBezier(start, start + ctlOffset, start + endOffset, 20, color);
}

void LevelMakeScene::drawJumpLineByEnd(DrawNode* draw, Vec2 end, Vec2 ctlOffset, Vec2 endOffset, Color4F color)
{
	draw->drawQuadBezier(end - endOffset, end + ctlOffset, end, 20, color);
}

void LevelMakeScene::AddEstimateFrameText(Node* parent, char* buf, float p1, float p2,
	int frameRate, float playerWidth, float speed, float posY, Color4B color) const
{
	int frameNumber = static_cast<int>(frameRate * (p2 - p1 - playerWidth) / speed);
	sprintf(buf, "%d", frameNumber);
	Text* text = Text::create(buf, "Arial", 16);
	if (frameNumber <= 0)
		color = Color4B::RED;
	text->setTextColor(color);
	text->setPosition(Point(p1 + (p2 - p1) / 2, posY));
	parent->addChild(text);
}
