#include "LevelMakeScene.h"
#include "CSCClass\CommonFunctions.h"
#include "GameMediator.h"
#include "MainMenuScene.h"

LevelMakeScene::LevelMakeScene()
	: m_pPlayer(nullptr)
	, m_pDropDownListLevel(nullptr)
	, m_pDropDownListRoom(nullptr)
	, m_nCurEnemyId(0)
	, m_nCurJumpPointId(0)
	, m_bIsAutoTrying(false)
	, m_bIsTouchEnemy(false)
	, m_touchOffset(Vec2(0,0))
{
	m_mTextFieldStructs.clear();
	m_vEnemys.clear();
	m_vJumpPoints.clear();
	m_vJumpPointsForAutoTrying.clear();
	m_vAirPoints.clear();
}

LevelMakeScene::~LevelMakeScene()
{
	m_mTextFieldStructs.clear();
	m_vEnemys.clear();
	m_vJumpPoints.clear();
	m_vJumpPointsForAutoTrying.clear();
	m_vAirPoints.clear();

	_eventDispatcher->removeCustomEventListeners(CSCClass::EVENT_DROPDOWNLIST_SELECTED + "blockType");
	_eventDispatcher->removeCustomEventListeners(CSCClass::EVENT_DROPDOWNLIST_SELECTED + "level");
	_eventDispatcher->removeCustomEventListeners(CSCClass::EVENT_DROPDOWNLIST_SELECTED + "room");
}

Scene* LevelMakeScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	// open Debug
	PhysicsWorld* world = scene->getPhysicsWorld();
	world->setGravity(Vec2(0, 0));
    
    // 'layer' is an autorelease object
    auto layer = LevelMakeScene::create();
	layer->setWorld(world);

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
	auto buttonReturn = dynamic_cast<Button*>(rootNode->getChildByName("Button_Return"));
	auto buttonAddNewLevel = dynamic_cast<Button*>(rootNode->getChildByName("Button_AddNewLevel"));
	auto buttonAddNewRoom = dynamic_cast<Button*>(rootNode->getChildByName("Button_AddNewRoom"));
	auto buttonAddBlock = dynamic_cast<Button*>(rootNode->getChildByName("Button_AddBlock"));
	auto buttonRemoveBlock = dynamic_cast<Button*>(rootNode->getChildByName("Button_RemoveBlock"));
	auto buttonShowHideDebug = dynamic_cast<Button*>(rootNode->getChildByName("Button_ShowHideDebug"));
	auto buttonTry = dynamic_cast<Button*>(rootNode->getChildByName("Button_Try"));
	auto buttonSave = dynamic_cast<Button*>(rootNode->getChildByName("Button_SaveLevel")); 
	auto buttonLoad = dynamic_cast<Button*>(rootNode->getChildByName("Button_LoadLevel"));
	auto buttonExport = dynamic_cast<Button*>(rootNode->getChildByName("Button_ExportLevel"));
	auto buttonImport = dynamic_cast<Button*>(rootNode->getChildByName("Button_ImportLevel"));
	buttonReturn->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_MainMenu, this));
	buttonAddNewLevel->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_AddNewLevel, this));
	buttonAddNewRoom->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_AddNewRoom, this));
	buttonAddBlock->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_AddBlock, this));
	buttonRemoveBlock->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_RemoveBlock, this));
	buttonShowHideDebug->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_ShowHideDebug, this));
	buttonTry->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Try, this));
	buttonSave->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Save, this));
	buttonLoad->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Load, this));
	buttonExport->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Export, this));
	buttonImport->addClickEventListener(CC_CALLBACK_1(LevelMakeScene::buttonCallback_Import, this));
	// get level panel
	auto panelLevel = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Level"));
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Level_ID")), TAG_LEVEL_ID, false);
	m_pTextFieldLevelName = dynamic_cast<TextField*>(panelLevel->getChildByName("Node_Level_Title")->getChildByName("TextField"));
	m_pTextFieldLevelName->addEventListener(CC_CALLBACK_2(LevelMakeScene::onTextFieldEvent, this));
	m_pTextFieldLevelName->setTag(TAG_LEVEL_NAME);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Level_MaxDeadTime")), TAG_LEVEL_MAX_DEADTIME, false);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Room_ID")), TAG_LEVEL_ROOM_ID, false);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Width")), TAG_LEVEL_ROOM_WIDTH);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Height")), TAG_LEVEL_ROOM_HEIGHT);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Color_R")), TAG_LEVEL_ROOM_COLOR_R);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Color_G")), TAG_LEVEL_ROOM_COLOR_G);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Color_B")), TAG_LEVEL_ROOM_COLOR_B);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Block_Color_R")), TAG_ENEMY_COLOR_R);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Block_Color_G")), TAG_ENEMY_COLOR_G);
	this->getLevelNode(dynamic_cast<Node*>(panelLevel->getChildByName("Node_Block_Color_B")), TAG_ENEMY_COLOR_B);
	// get block panel
	auto panelBlock = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Block"));
	auto textType = dynamic_cast<Text*>(panelBlock->getChildByName("Text_Type"));
	m_pDropDownListType = CSCClass::DropDownList::create(Label::createWithSystemFont("Normal", "fonts/arial.ttf", 22), Size(80, 30), "blockType"); // TYPE_NORMAL
	m_pDropDownListType->addLabel(Label::createWithSystemFont("Rotate", "fonts/arial.ttf", 22)); // TYPE_ROTATE
	m_pDropDownListType->addLabel(Label::createWithSystemFont("Move", "fonts/arial.ttf", 22)); // TYPE_MOVE
	m_pDropDownListType->addLabel(Label::createWithSystemFont("Blink", "fonts/arial.ttf", 22)); // TYPE_BLINK
	m_pDropDownListType->addLabel(Label::createWithSystemFont("RotateR", "fonts/arial.ttf", 22)); // TYPE_ROTATE_REVERSE
	m_pDropDownListType->setPosition(90, textType->getPositionY() - m_pDropDownListType->getContentSize().height / 2);
	panelBlock->addChild(m_pDropDownListType);
	_eventDispatcher->addCustomEventListener(CSCClass::EVENT_DROPDOWNLIST_SELECTED + "blockType", CC_CALLBACK_1(LevelMakeScene::onDropDownList_BlockType, this));
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_Width")), TAG_ENEMY_WIDTH);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_Height")), TAG_ENEMY_HEIGHT);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_PositionX")), TAG_ENEMY_POSITION_X);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_PositionY")), TAG_ENEMY_POSITION_Y);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_RotateDuration")), TAG_ENEMY_ROTATE_TIME10);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_DestinationX")), TAG_ENEMY_DESTINATION_X);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_DestinationY")), TAG_ENEMY_DESTINATION_Y);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_MoveDuration")), TAG_ENEMY_MOVE_TIME10);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_BlinkDuration")), TAG_ENEMY_BLINK_TIME10);
	this->getLevelNode(dynamic_cast<Node*>(panelBlock->getChildByName("Node_BlinkHideDuration")), TAG_ENEMY_BLINKHIDE_TIME10);
	// get player panel
	auto panelPlayer = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_Player"));
	this->getLevelNode(dynamic_cast<Node*>(panelPlayer->getChildByName("Node_Speed")), TAG_PLAYER_SPEED);
	this->getLevelNode(dynamic_cast<Node*>(panelPlayer->getChildByName("Node_JumpDuration")), TAG_PLAYER_JUMPTIME10);

	// get level choose panel
	auto panelLevelChoose = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_LevelChoose"));
	m_pDropDownListLevel = CSCClass::DropDownList::create(Label::createWithSystemFont("1", "fonts/arial.ttf", 22), Size(80, 30), "level");
	m_pDropDownListLevel->setPosition(70, (panelLevelChoose->getContentSize().height - m_pDropDownListLevel->getContentSize().height) / 2);
	panelLevelChoose->addChild(m_pDropDownListLevel);
	_eventDispatcher->addCustomEventListener(CSCClass::EVENT_DROPDOWNLIST_SELECTED + "level", CC_CALLBACK_1(LevelMakeScene::onDropDownList_Level, this));

	m_pDropDownListRoom = CSCClass::DropDownList::create(Label::createWithSystemFont("1", "fonts/arial.ttf", 22), Size(80, 30), "room");
	m_pDropDownListRoom->setPosition(270, (panelLevelChoose->getContentSize().height - m_pDropDownListRoom->getContentSize().height) / 2);
	panelLevelChoose->addChild(m_pDropDownListRoom);
	_eventDispatcher->addCustomEventListener(CSCClass::EVENT_DROPDOWNLIST_SELECTED + "room", CC_CALLBACK_1(LevelMakeScene::onDropDownList_Room, this));

	// set initial number
	int levelWidth = visibleSize.width / 2;
	int levelHeight = visibleSize.height*0.2;
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_ID), 0, 1, 9999);
	m_sLevelName = "Initial Title";
	m_pTextFieldLevelName->setString(m_sLevelName);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_MAX_DEADTIME), 0, 10, 999999);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_ROOM_ID), 0, 1, 9999);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_ROOM_WIDTH), 0, levelWidth, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_ROOM_HEIGHT), 0, levelHeight, visibleSize.height);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_R), 0, 95, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_G), 0, 102, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_B), 0, 120, 255);

	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R), 0, 125, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G), 0, 149, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B), 0, 153, 255);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_WIDTH), 0, 50, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_HEIGHT), 0, 50, levelHeight);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_POSITION_X), -levelWidth, levelWidth * 0.5, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_POSITION_Y), -levelHeight, 0, levelHeight);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_ROTATE_TIME10), 1, 10, 99);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_DESTINATION_X), -levelWidth, levelWidth * 0.5, levelWidth * 2);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_DESTINATION_Y), -levelHeight, 0, levelHeight * 2);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_MOVE_TIME10), 1, 10, 99);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_BLINK_TIME10), 1, 10, 99);
	this->initStruct(&m_mTextFieldStructs.at(TAG_ENEMY_BLINKHIDE_TIME10), 1, 10, 99);

	this->initStruct(&m_mTextFieldStructs.at(TAG_PLAYER_SPEED), 0, levelWidth*0.2, levelWidth);
	this->initStruct(&m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10), 1, 6, 20);

	// add line between gameplay screen & edit screen
	DrawNode* line = DrawNode::create();
	line->drawLine(Vec2(levelWidth, 0), Vec2(levelWidth, visibleSize.height), Color4F::WHITE);
	this->addChild(line);

	// add default level & room
	auto levelsData = GameMediator::getInstance()->getGameLevelData();
	if (levelsData->size() > 0)
	{
		this->setDropDownList(levelsData);
		this->loadDataFrom(1, 1);
	}
	else
	{
		this->addRoom();
		this->addEnemy();
		this->addPlayer();

		//this->calcFoothold();
	}

	this->scheduleUpdate();

    return true;
}

// event functions
bool LevelMakeScene::onTouchBegan(Touch *touch, Event *event)
{
	if (m_bIsAutoTrying)
	{
		if (m_pPlayer)
		{
			m_pPlayer->jump();
		}
	}
	else
	{
		Point point = touch->getLocation();  // get touch point
		for (ssize_t i = 0; i < m_vEnemys.size(); i++)
		{
			Enemy* enemy = m_vEnemys.at(i);
			if (enemy->getBoundingBox().containsPoint(point))
			{
				m_bIsTouchEnemy = true;
				m_touchOffset = enemy->getPosition() - point;
				m_vEnemys.at(m_nCurEnemyId)->unSelect();
				m_nCurEnemyId = i;

				this->selectEnemy(enemy);
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
		int maxX = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_WIDTH).number - minX;
		int maxY = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_HEIGHT).number;
		position.x = MAX(MIN(position.x, maxX), minX);
		position.y = MAX(MIN(position.y, maxY), 0);
		auto enemy = m_vEnemys.at(m_nCurEnemyId);
		if (enemy->getType() == TYPE_MOVE)
			enemy->updateStartPoint(position);
		enemy->setPosition(position);
		this->updateBlockTextFieldNumber(TAG_ENEMY_POSITION_X, position.x);
		this->updateBlockTextFieldNumber(TAG_ENEMY_POSITION_Y, position.y);
		//this->calcFoothold();
	}
}

void LevelMakeScene::onTouchEnded(Touch *touch, Event *event)
{
	if (!m_bIsAutoTrying)
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
	int tag = field->getTag();
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
		if (tag != TAG_LEVEL_NAME)
		{
			// format input
			string strHolder = field->getPlaceHolder();
			int pos = strHolder.find("-");
			if (field->getString().compare("-") == 0)
				return;
			int number = MAX(MIN(
					atoi(CSCClass::eraseStringNonNumber(field->getString()).c_str()),
					atoi(strHolder.substr(pos + 1, strHolder.size() - pos - 1).c_str())
					), atoi(strHolder.substr(0, pos - 1).c_str()));
			field->setString(StringUtils::format("%d", number));
		}
		
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
		if(tag == TAG_LEVEL_NAME)
			m_sLevelName = field->getString();
		else
		{
			TextFieldSliderBindInt* structTmp = &m_mTextFieldStructs.at(tag);
			structTmp->number = atoi(field->getString().c_str());
			if (structTmp->slider)
				structTmp->slider->setPercent((structTmp->number - structTmp->min) * 100 / (structTmp->max - structTmp->min));
			this->updateBlockByLevel(tag, structTmp->number);

			// update object
			this->updateLevelOrEnemys(tag);
			this->updateCurEnemy(tag);
			this->updatePlayer(tag);

			//this->calcFoothold();
		}
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
		structTmp->textField->setString(StringUtils::format("%d", structTmp->number));
		this->updateBlockByLevel(tag, structTmp->number);

		// update object
		this->updateLevelOrEnemys(tag);
		this->updateCurEnemy(tag);
		this->updatePlayer(tag);

		//this->calcFoothold();
	}
}

void LevelMakeScene::onDropDownList_BlockType(EventCustom* event)
{
	CS_RETURN_IF(m_vEnemys.size() == 0);

	int type = m_pDropDownListType->getSelectedIndex() + 1;
	Enemy* enemy = m_vEnemys.at(m_nCurEnemyId);
	CS_RETURN_IF(type == enemy->getType());

	this->updateBlockType(type);

	// remove old enemy & add new enemy
	m_vEnemys.at(m_nCurEnemyId)->removeFromParent();
	m_vEnemys.erase(m_nCurEnemyId);
	this->addEnemy();
	for (size_t i = 0, length = m_vEnemys.size(); i < length; i++)
		m_vEnemys.at(i)->updateId(i);
}

void LevelMakeScene::onDropDownList_Level(EventCustom* event)
{
	// update level id
	int index = m_pDropDownListLevel->getSelectedIndex();
	int level = index + 1;
	this->updateBlockTextFieldNumber(TAG_LEVEL_ID, level);
	// make new room id dropdownlist
	auto roomsData = GameMediator::getInstance()->getGameLevelData()->at(index)->getRoomsData();
	m_pDropDownListRoom->clearAllLabels();
	for (size_t i = 0, length = roomsData->size(); i < length; i++)
		m_pDropDownListRoom->addLabel(Label::createWithSystemFont(StringUtils::format("%d", i + 1), "fonts/arial.ttf", 22));
	// load room 1 from this level
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_ID, 1);
	m_pDropDownListRoom->setSelectedIndex(0);
	this->loadDataFrom(level, 1);
}

void LevelMakeScene::onDropDownList_Room(EventCustom* event)
{
	// after room selected
	int level = m_mTextFieldStructs.at(TAG_LEVEL_ID).number;
	int room = m_pDropDownListRoom->getSelectedIndex() + 1;
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_ID, room);
	this->loadDataFrom(level, room);
}

bool LevelMakeScene::onContactBegin(const PhysicsContact& contact)
{
	if (!m_bIsAutoTrying)
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
	this->calcFoothold();
	if (m_bIsAutoTrying && m_pPlayer)
	{
		if (m_nCurJumpPointId < m_vJumpPointsForAutoTrying.size())
		{
			float rangePerFrame = m_pPlayer->getSpeed() / Director::getInstance()->getFrameRate();
			float rightPt = m_pPlayer->getPositionX() + m_pPlayer->getContentSize().width / 2;
			pair<float,bool> jumpPt = m_vJumpPointsForAutoTrying.at(m_nCurJumpPointId);
			if (jumpPt.second) // start point, jump at this frame
			{
				if (rightPt + rangePerFrame >= jumpPt.first && rightPt < jumpPt.first)
				{
					m_pPlayer->jump();
					m_nCurJumpPointId++;
				}
				else if (rightPt > jumpPt.first)
				{
					m_nCurJumpPointId++;
				}
			}
			else // endStart point, jump the next frame
			{
				if (rightPt >= jumpPt.first && rightPt - rangePerFrame < jumpPt.first)
				{
					m_pPlayer->jump();
					m_nCurJumpPointId++;
				}
				else if (rightPt > jumpPt.first)
				{
					m_nCurJumpPointId++;
				}
			}
		}
	}
}

void LevelMakeScene::buttonCallback_MainMenu(Ref* pSender)
{
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
}

void LevelMakeScene::buttonCallback_AddNewLevel(Ref* pSender)
{
	auto levelsData = GameMediator::getInstance()->getGameLevelData();
	int newLevelIndex = levelsData->size() + 1;
	m_pDropDownListLevel->addLabel(Label::createWithSystemFont(StringUtils::format("%d", newLevelIndex), "fonts/arial.ttf", 22));
	m_pDropDownListLevel->setSelectedIndex(newLevelIndex - 1);
	m_pDropDownListRoom->clearAllLabels();
	m_pDropDownListRoom->addLabel(Label::createWithSystemFont("1", "fonts/arial.ttf", 22));
	m_pDropDownListRoom->setSelectedIndex(0);
	this->updateBlockTextFieldNumber(TAG_LEVEL_ID, newLevelIndex);
	m_sLevelName = "Initial Title";
	m_pTextFieldLevelName->setString(m_sLevelName);
	this->updateBlockTextFieldNumber(TAG_LEVEL_MAX_DEADTIME, 10);
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_ID, 1);
	this->saveDataTo(newLevelIndex, 1);
}

void LevelMakeScene::buttonCallback_AddNewRoom(Ref* pSender)
{
	auto levelsData = GameMediator::getInstance()->getGameLevelData();
	CS_RETURN_IF(levelsData->size() == 0);
	int level = m_mTextFieldStructs.at(TAG_LEVEL_ID).number;
	auto roomsData = levelsData->at(level - 1)->getRoomsData();
	int newRoomIndex = roomsData->size() + 1;
	m_pDropDownListRoom->addLabel(Label::createWithSystemFont(StringUtils::format("%d", newRoomIndex), "fonts/arial.ttf", 22));
	m_pDropDownListRoom->setSelectedIndex(newRoomIndex - 1);
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_ID, newRoomIndex);
	this->saveDataTo(level, newRoomIndex);
}

// callback functions
void LevelMakeScene::buttonCallback_AddBlock(Ref* pSender)
{
	this->addEnemy();
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
			this->selectEnemy(m_vEnemys.at(m_nCurEnemyId));
		}
		for (size_t i = 0, length = m_vEnemys.size(); i < length; i++)
			m_vEnemys.at(i)->updateId(i);
	}
}

void LevelMakeScene::buttonCallback_ShowHideDebug(Ref* pSender)
{
	Button* button = static_cast<Button*>(pSender);
	if (m_pWorld->getDebugDrawMask() == PhysicsWorld::DEBUGDRAW_ALL)
	{
		m_pWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
		button->setTitleText("Show Debug");
	}
	else
	{
		m_pWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
		button->setTitleText("Hide Debug");
	}
}

void LevelMakeScene::buttonCallback_Try(Ref* pSender)
{
	Button* button = dynamic_cast<Button*>(pSender);
	if (m_bIsAutoTrying) // stop trying
	{
		button->setTitleText("Try");
		m_bIsAutoTrying = false;
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
		m_bIsAutoTrying = true;
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

void LevelMakeScene::buttonCallback_Save(Ref* pSender)
{
	int level = m_mTextFieldStructs.at(TAG_LEVEL_ID).number;
	int room = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_ID).number;
	this->saveDataTo(level, room);
}

void LevelMakeScene::buttonCallback_Load(Ref* pSender)
{
	int level = m_mTextFieldStructs.at(TAG_LEVEL_ID).number;
	int room = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_ID).number;
	this->loadDataFrom(level, room);
}

void LevelMakeScene::buttonCallback_Export(Ref* pSender)
{
	GameMediator::getInstance()->saveGameLevelFile();
}

void LevelMakeScene::buttonCallback_Import(Ref* pSender)
{
	GameMediator::getInstance()->loadGameLevelFile();
	auto levelsData = GameMediator::getInstance()->getGameLevelData();
	this->setDropDownList(levelsData);
}

// draw functions
void LevelMakeScene::addRoom()
{
	DrawNode* background = DrawNode::create();
	background->drawSolidRect(Point::ZERO, 
		Size(m_mTextFieldStructs.at(TAG_LEVEL_ROOM_WIDTH).number, m_mTextFieldStructs.at(TAG_LEVEL_ROOM_HEIGHT).number),
		Color4F(Color3B(m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_R).number,
			m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_G).number, 
			m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_B).number)));
	this->addChild(background, 0, TAG_BACKGROUND);
}

void LevelMakeScene::addEnemy()
{
	int type = m_pDropDownListType->getSelectedIndex() + 1;
	Size size = Size(m_mTextFieldStructs.at(TAG_ENEMY_WIDTH).number, m_mTextFieldStructs.at(TAG_ENEMY_HEIGHT).number);
	Color3B color = Color3B(m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number, m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number, m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number);
	Point position = Point(m_mTextFieldStructs.at(TAG_ENEMY_POSITION_X).number, m_mTextFieldStructs.at(TAG_ENEMY_POSITION_Y).number);

	m_nCurEnemyId = m_vEnemys.size();
	Enemy* enemy = NULL;
	switch (type)
	{
	case TYPE_NORMAL:
		enemy = Enemy::create(size, color, m_nCurEnemyId);
		break;
	case TYPE_ROTATE:
		enemy = Enemy::createRotate(size, color, m_nCurEnemyId,
			m_mTextFieldStructs.at(TAG_ENEMY_ROTATE_TIME10).number * 0.1f);
		break;
	case TYPE_ROTATE_REVERSE:
		enemy = Enemy::createRotate(size, color, m_nCurEnemyId,
			m_mTextFieldStructs.at(TAG_ENEMY_ROTATE_TIME10).number * 0.1f, true);
		break;
	case TYPE_MOVE:
		enemy = Enemy::createMove(size, color, m_nCurEnemyId, position, 
			Point(m_mTextFieldStructs.at(TAG_ENEMY_DESTINATION_X).number, m_mTextFieldStructs.at(TAG_ENEMY_DESTINATION_Y).number),
			m_mTextFieldStructs.at(TAG_ENEMY_MOVE_TIME10).number * 0.1f);
		break;
	case TYPE_BLINK:
		enemy = Enemy::createBlink(size, color, m_nCurEnemyId, 
			m_mTextFieldStructs.at(TAG_ENEMY_BLINK_TIME10).number * 0.1f,
			m_mTextFieldStructs.at(TAG_ENEMY_BLINKHIDE_TIME10).number * 0.1f);
		break;
	default:
		break;
	}
	enemy->setPosition(position);
	this->addChild(enemy, 1, TAG_ENEMY);
	m_vEnemys.pushBack(enemy);
	enemy->select();
	m_pDropDownListType->setSelectedIndex(type - 1);
	this->updateBlockType(type);
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

	m_pPlayer = Player::create(Color3B(color_r, color_g, color_b),
		m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number,
		m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10).number * 0.1f);
	m_pPlayer->setPosition(m_pPlayer->getContentSize().width / 2, m_pPlayer->getContentSize().height / 2);
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

	m_pPlayer = Player::create(Color3B(color_r, color_g, color_b), 
		m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number,
		m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10).number * 0.1f);
	Size playerSize = m_pPlayer->getContentSize();
	float posX = playerSize.width / 2;
	float posY = playerSize.height / 2;
	int levelWidth = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_WIDTH).number;
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

void LevelMakeScene::selectEnemy(Enemy* enemy)
{
	enemy->select();
	int type = enemy->getType();
	m_pDropDownListType->setSelectedIndex(type - 1);
	Size size = enemy->getContentSize();
	this->updateBlockTextFieldNumber(TAG_ENEMY_WIDTH, size.width);
	this->updateBlockTextFieldNumber(TAG_ENEMY_HEIGHT, size.height);
	Point position = enemy->getPosition();
	this->updateBlockTextFieldNumber(TAG_ENEMY_POSITION_X, position.x);
	this->updateBlockTextFieldNumber(TAG_ENEMY_POSITION_Y, position.y);
	switch (type)
	{
	case TYPE_ROTATE:
	{
		this->updateBlockTextFieldNumber(TAG_ENEMY_ROTATE_TIME10, enemy->getRotateDuration() * 10);
		break;
	}
	case TYPE_ROTATE_REVERSE:
	{
		this->updateBlockTextFieldNumber(TAG_ENEMY_ROTATE_TIME10, enemy->getRotateDuration() * 10);
		break;
	}
	case TYPE_MOVE:
	{
		Point startPos = enemy->getStartPoint();
		this->updateBlockTextFieldNumber(TAG_ENEMY_POSITION_X, startPos.x);
		this->updateBlockTextFieldNumber(TAG_ENEMY_POSITION_Y, startPos.y);
		Point destPos = enemy->getDestPoint();
		this->updateBlockTextFieldNumber(TAG_ENEMY_DESTINATION_X, destPos.x);
		this->updateBlockTextFieldNumber(TAG_ENEMY_DESTINATION_Y, destPos.y);
		this->updateBlockTextFieldNumber(TAG_ENEMY_MOVE_TIME10, enemy->getMoveDuration() * 10);
		break;
	}
	case TYPE_BLINK:
	{
		this->updateBlockTextFieldNumber(TAG_ENEMY_BLINK_TIME10, enemy->getBlinkDuration() * 10);
		this->updateBlockTextFieldNumber(TAG_ENEMY_BLINKHIDE_TIME10, enemy->getBlinkHideDuration() * 10);
		break;
	}
	default:
		break;
	}
	this->updateBlockType(type);
}

void LevelMakeScene::updateLevelOrEnemys(int tag)
{
	if (tag >= TAG_LEVEL_ROOM_WIDTH && tag <= TAG_LEVEL_ROOM_COLOR_B)
	{
		this->removeChildByTag(TAG_BACKGROUND);
		this->addRoom();
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
	CS_RETURN_IF(tag <= TAG_ENEMY_MIN || tag >= TAG_ENEMY_MAX);
	CS_RETURN_IF(m_vEnemys.size() == 0);

	Enemy* obj = m_vEnemys.at(m_nCurEnemyId);
	int number = m_mTextFieldStructs.at(tag).number;
	switch (tag)
	{
	case TAG_ENEMY_WIDTH:
		obj->updateSize(Size(number, obj->getContentSize().height));
		if (obj->getIsPlayerAdded())
		{
			obj->removePlayerBlockForLevelMake();
			obj->addPlayerBlockForLevelMake(m_pPlayer->getContentSize());
		}
		break;
	case TAG_ENEMY_HEIGHT:
		obj->updateSize(Size(obj->getContentSize().width, number));
		if (obj->getIsPlayerAdded())
		{
			obj->removePlayerBlockForLevelMake();
			obj->addPlayerBlockForLevelMake(m_pPlayer->getContentSize());
		}
		break;
	case TAG_ENEMY_POSITION_X:
		if (obj->getType() == TYPE_MOVE)
			obj->updateStartPoint(Point(number, obj->getStartPoint().y));
		obj->setPositionX(number);
		break;
	case TAG_ENEMY_POSITION_Y:
		if (obj->getType() == TYPE_MOVE)
			obj->updateStartPoint(Point(obj->getStartPoint().x, number));
		obj->setPositionY(number);
		break;
	case TAG_ENEMY_ROTATE_TIME10:
		obj->updateRotateDuration(number * 0.1f);
		break;
	case TAG_ENEMY_DESTINATION_X:
		obj->updateDestPoint(Point(number, obj->getDestPoint().y));
		break;
	case TAG_ENEMY_DESTINATION_Y:
		obj->updateDestPoint(Point(obj->getDestPoint().x, number));
		break;
	case TAG_ENEMY_MOVE_TIME10:
		obj->updateMoveDuration(number * 0.1f);
		break;
	case TAG_ENEMY_BLINK_TIME10:
		obj->updateBlinkDuration(number * 0.1f);
		break;
	case TAG_ENEMY_BLINKHIDE_TIME10:
		obj->updateBlinkHideDuration(number * 0.1f);
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
	CS_RETURN_IF(!m_pPlayer);

	float jumpHeight = m_pPlayer->getJumpHeight();
	float jumpTime = m_pPlayer->getJumpDuration();
	float speed = m_pPlayer->getSpeed();
	Size playerSize = m_pPlayer->getContentSize();
	float jumpLength = jumpTime * speed;
	float endOffsetX = jumpLength - playerSize.width * 0.7; // this 0.7 is tryout

	float ctlOffsetX = jumpLength / 2;
	Vec2 vecCtlOffsetStart = Vec2(ctlOffsetX, jumpHeight);
	Vec2 vecCtlOffsetEnd = Vec2(-ctlOffsetX, jumpHeight);
	Vec2 vecEndOffset = Vec2(endOffsetX, 0);

	// calc all land point
	m_vJumpPoints.clear();
	m_vJumpPointsForAutoTrying.clear();
	m_vAirPoints.clear();
	for (auto obj : m_vEnemys)
	{
		Size enemySize = obj->getContentSize();
		Vec2 enemyBottomPos = Vec2(obj->getPositionX(), obj->getPositionY() - obj->getAnchorPoint().y * enemySize.height);
		if (enemyBottomPos.y >= playerSize.height)
		{
			obj->addPlayerBlockForLevelMake(playerSize);
			// add left and right bottom point to air point for conflict calc
			Point pBottomLeft = Point(enemyBottomPos.x - enemySize.width / 2 - playerSize.width, enemyBottomPos.y - playerSize.height);
			Point pBottomRight = Point(enemyBottomPos.x + enemySize.width / 2 + playerSize.width, enemyBottomPos.y - playerSize.height);
			m_vAirPoints.push_back(pBottomLeft);
			m_vAirPoints.push_back(pBottomRight);
		}
	}
	for (auto obj : m_vEnemys)
	{
		Size enemySize = obj->getContentSize();
		Vec2 enemyBottomPos = Vec2(obj->getPositionX(), obj->getPositionY() - obj->getAnchorPoint().y * enemySize.height);
		if (enemyBottomPos.y < playerSize.height)
		{
			obj->removePlayerBlockForLevelMake();
			// calc min start point & min end point for enemys on the land
			float realHeight = enemyBottomPos.y + enemySize.height;
			if (realHeight >= jumpHeight) // too high, can't jumpover
				continue;
			float tmp = sqrt(1 - realHeight / jumpHeight);
			float landTime1 = (1 - tmp) * jumpTime * 0.5;
			float landTime2 = jumpTime - (1 + tmp) * jumpTime * 0.5;
			float pStart = enemyBottomPos.x - enemySize.width / 2 - landTime1 * speed;
			float pStartEnd = pStart + endOffsetX;
			float pEnd = enemyBottomPos.x + enemySize.width / 2 + landTime2 * speed;
			float pEndStart = pEnd - endOffsetX;
			m_vJumpPoints.push_back({ pEndStart, pStart, pEnd, pStartEnd });

			Vec2 vecStart = Vec2(pStart, 0);
			if (!this->isJumpLineConflict(vecStart, vecStart + vecCtlOffsetStart, vecStart + vecEndOffset))
				m_vJumpPointsForAutoTrying.push_back(pair<float, bool>(pStart, true));
			vecStart = Vec2(pEndStart, 0);
			if (!this->isJumpLineConflict(vecStart, vecStart + vecCtlOffsetStart, vecStart + vecEndOffset))
				m_vJumpPointsForAutoTrying.push_back(pair<float, bool>(pEndStart, false));
		}
	}
	std::sort(m_vJumpPoints.begin(), m_vJumpPoints.end(), sortJumpPoints);
	std::sort(m_vJumpPointsForAutoTrying.begin(), m_vJumpPointsForAutoTrying.end(), sortJumpPointsForAutoTrying);

	// draw land line
	this->removeChildByTag(TAG_FOOTHOLDLINE);
	this->removeChildByTag(TAG_TIMETEXT);
	size_t length = m_vJumpPoints.size();
	if (length > 0)
	{
		// set colors
		Color4F colorStart = Color4F::GREEN;
		Color4F colorEnd = Color4F::BLUE;
		Color4F colorCollapse = Color4F::RED;
		int color_r = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_R).number;
		int color_g = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_G).number;
		int color_b = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_B).number;
		if (color_r == 0 && color_g == 255 && color_b == 0)
		{
			colorStart = Color4F::RED;
			colorCollapse = Color4F::GREEN;
		}

		DrawNode* draw = DrawNode::create();
		int lineWidth = 1;
		draw->setLineWidth(lineWidth);

		Node* textNode = Node::create();
		int frameRate = Director::getInstance()->getFrameRate();
		// draw 0 - p1 first width one jump line
		float p1 = m_vJumpPoints.at(0).pStart;
		Vec2 vecEnd;
		Vec2 vecStart = Vec2(p1, 0);
		this->drawJumpLine(draw, vecStart, vecStart + vecCtlOffsetStart, vecStart + vecEndOffset, colorStart);
		draw->drawLine(Vec2(0, lineWidth), Vec2(p1, lineWidth), colorStart);
		// calc estimate time with frame number for start jump
		this->AddEstimateFrameText(textNode, 0, p1, frameRate, playerSize.width, speed, 20, Color4B(colorStart));
		float p2 = m_vJumpPoints.at(0).pEndStart;
		this->AddEstimateFrameText(textNode, 0, p2, frameRate, playerSize.width, speed, 20, Color4B(colorEnd));
		this->AddEstimateFrameText(textNode, p2, p1, frameRate, 0, speed, 20, Color4B(colorStart));
		// draw p1-p2 with two jump lines
		Color4F color = colorStart;
		for (size_t i = 1; i < length; i++)
		{
			float p1End = m_vJumpPoints.at(i - 1).pEnd;
			float p1StartEnd = m_vJumpPoints.at(i - 1).pStartEnd;
			float p2Start = m_vJumpPoints.at(i).pStart;
			float p2EndStart = m_vJumpPoints.at(i).pEndStart;
			// draw lines
			float landLength = p2Start - p1End - playerSize.width;
			if (landLength < 0)
				color = colorCollapse;
			vecEnd = Vec2(p1End, 0);
			this->drawJumpLine(draw, vecEnd - vecEndOffset, vecEnd + vecCtlOffsetEnd, vecEnd, colorEnd);
			draw->drawLine(Vec2(p1End, lineWidth), Vec2(p2Start, lineWidth), color);
			vecStart = Vec2(p2Start, 0);
			this->drawJumpLine(draw, vecStart, vecStart + vecCtlOffsetStart, vecStart + vecEndOffset, colorStart);
			// calc estimate time with frame number for 4-combinations
			this->AddEstimateFrameText(textNode, p1End, p2Start, frameRate, playerSize.width, speed, 20, Color4B(colorEnd));
			this->AddEstimateFrameText(textNode, p1StartEnd, p2Start, frameRate, playerSize.width, speed, 40, Color4B(colorStart));
			this->AddEstimateFrameText(textNode, p1End, p2EndStart, frameRate, playerSize.width, speed, 60, Color4B(colorEnd));
			this->AddEstimateFrameText(textNode, p1StartEnd, p2EndStart, frameRate, playerSize.width, speed, 80, Color4B(colorStart));
			this->AddEstimateFrameText(textNode, p2EndStart, p2Start, frameRate, 0, speed, 20, Color4B(colorStart));
		}
		this->addChild(textNode, 1, TAG_TIMETEXT);
		// draw last p - width with one jump line
		int width = m_mTextFieldStructs.at(TAG_LEVEL_ROOM_WIDTH).number;
		p1 = m_vJumpPoints.at(length - 1).pEnd;
		vecEnd = Vec2(p1, 0);
		this->drawJumpLine(draw, vecEnd - vecEndOffset, vecEnd +  vecCtlOffsetEnd, vecEnd, colorEnd);
		draw->drawLine(Vec2(p1, lineWidth), Vec2(width, lineWidth), colorEnd);

		// draw start point for trying
		for (auto obj : m_vJumpPointsForAutoTrying)
			draw->drawDot(Vec2(obj.first, 2), 4, Color4F::RED);
		this->addChild(draw, 1, TAG_FOOTHOLDLINE);
	}
}

// data functions
void LevelMakeScene::loadDataFrom(int level, int room)
{
	auto levelDatas = GameMediator::getInstance()->getGameLevelData();
	CS_RETURN_IF(level > levelDatas->size());
	auto levelData = levelDatas->at(level - 1);
	CS_RETURN_IF(room > levelData->getRoomsData()->size());
	auto roomData = &(levelData->getRoomsData()->at(room - 1));

	// level name & dead times
	m_sLevelName = levelData->getLevelName();
	m_pTextFieldLevelName->setString(m_sLevelName);
	this->updateBlockTextFieldNumber(TAG_LEVEL_MAX_DEADTIME, levelData->getMaxDeadTime());
	// room
	auto roomSize = roomData->size;
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_WIDTH, roomSize.width);
	this->updateBlockByLevel(TAG_LEVEL_ROOM_WIDTH, roomSize.width);
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_HEIGHT, roomSize.height);
	this->updateBlockByLevel(TAG_LEVEL_ROOM_HEIGHT, roomSize.height);
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_COLOR_R, roomData->color.r);
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_COLOR_G, roomData->color.g);
	this->updateBlockTextFieldNumber(TAG_LEVEL_ROOM_COLOR_B, roomData->color.b);
	// update level
	this->removeChildByTag(TAG_BACKGROUND);
	this->addRoom();

	// player
	this->updateBlockTextFieldNumber(TAG_PLAYER_SPEED, roomData->player_speed);
	this->updateBlockTextFieldNumber(TAG_PLAYER_JUMPTIME10, roomData->player_jumpTime * 10);
	// update player
	this->addPlayer();

	// clear all enemys
	this->updateBlockTextFieldNumber(TAG_ENEMY_COLOR_R, roomData->enemy_color.r);
	this->updateBlockTextFieldNumber(TAG_ENEMY_COLOR_G, roomData->enemy_color.g);
	this->updateBlockTextFieldNumber(TAG_ENEMY_COLOR_B, roomData->enemy_color.b);
	for (auto obj : m_vEnemys)
		obj->removeFromParent();
	m_vEnemys.clear();
	m_nCurEnemyId = 0;
	// add enemys
	auto enemys = &(roomData->enemysData);
	for (size_t i = 0, j = enemys->size(); i < j; i++)
	{
		auto enemyData = enemys->at(i);
		Enemy* enemy = NULL;
		int id = enemyData.id - 1;
		switch (enemyData.type)
		{
		case TYPE_NORMAL:
			enemy = Enemy::create(enemyData.size, roomData->enemy_color, id);
			break;
		case TYPE_ROTATE:
			enemy = Enemy::createRotate(enemyData.size, roomData->enemy_color, id, enemyData.rotateTime);
			break;
		case TYPE_ROTATE_REVERSE:
			enemy = Enemy::createRotate(enemyData.size, roomData->enemy_color, id, enemyData.rotateTime, true);
			break;
		case TYPE_MOVE:
			enemy = Enemy::createMove(enemyData.size, roomData->enemy_color, id, enemyData.position, enemyData.destination, enemyData.moveTime);
			break;
		case TYPE_BLINK:
			enemy = Enemy::createBlink(enemyData.size, roomData->enemy_color, id, enemyData.blinkTime, enemyData.blinkHideTime);
			break;
		default:
			enemy = Enemy::create(enemyData.size, roomData->enemy_color, id);
			break;
		}
		enemy->setPosition(enemyData.position);
		this->addChild(enemy, 1, TAG_ENEMY);
		m_vEnemys.pushBack(enemy);
	}
	// select the first one
	if (enemys->size() > 0)
		this->selectEnemy(*m_vEnemys.begin());
}

void LevelMakeScene::saveDataTo(int level, int room)
{
	auto levelsData = GameMediator::getInstance()->getGameLevelData();
	GameLevelData* pLevelData;
	RoomData* pRoomData;
	Point roomPosition;
	int levelIndex = level - 1;
	if (levelIndex < levelsData->size()) // level exist, change it
	{
		pLevelData = levelsData->at(levelIndex);
		auto roomsData = pLevelData->getRoomsData();
		int roomIndex = room - 1;
		if (roomIndex < roomsData->size()) // room exist, change it 
		{
			pRoomData = &(roomsData->at(roomIndex));
			if (roomIndex > 0)
			{
				auto preRoomData = &(roomsData->at(roomIndex - 1));
				roomPosition = Point(0, preRoomData->position.y + preRoomData->size.height);
			}
			else
			{
				roomPosition = Point::ZERO;
			}
		}
		else // room not exist, add it
		{
			RoomData roomData;
			pLevelData->getRoomsData()->push_back(roomData);
			pRoomData = &(*pLevelData->getRoomsData()->rbegin());
			roomPosition = Point::ZERO;
		}
	}
	else // level not exist, add it
	{
		pLevelData = GameLevelData::create();
		RoomData roomData;
		pLevelData->getRoomsData()->push_back(roomData);
		pRoomData = &(*pLevelData->getRoomsData()->rbegin());
		roomPosition = Point::ZERO;
		levelsData->push_back(pLevelData);
	}
	// set datas
	pLevelData->setLevel(level);
	pLevelData->setLevelName(m_sLevelName);
	pLevelData->setMaxDeadTime(m_mTextFieldStructs.at(TAG_LEVEL_MAX_DEADTIME).number);
	pRoomData->id = room;
	pRoomData->size = Size(m_mTextFieldStructs.at(TAG_LEVEL_ROOM_WIDTH).number, m_mTextFieldStructs.at(TAG_LEVEL_ROOM_HEIGHT).number);
	pRoomData->position = roomPosition;
	pRoomData->color = Color3B(m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_R).number,
		m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_G).number,
		m_mTextFieldStructs.at(TAG_LEVEL_ROOM_COLOR_B).number);

	pRoomData->player_speed = m_mTextFieldStructs.at(TAG_PLAYER_SPEED).number;
	pRoomData->player_jumpTime = m_mTextFieldStructs.at(TAG_PLAYER_JUMPTIME10).number * 0.1f;
	pRoomData->player_color = Color3B(m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number,
		m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number,
		m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number);

	pRoomData->enemy_color = Color3B(m_mTextFieldStructs.at(TAG_ENEMY_COLOR_R).number,
		m_mTextFieldStructs.at(TAG_ENEMY_COLOR_G).number,
		m_mTextFieldStructs.at(TAG_ENEMY_COLOR_B).number);
	pRoomData->enemysData.clear();
	for (size_t i = 0, j = m_vEnemys.size(); i < j; i++)
	{
		EnemyData enemyData;
		auto enemy = m_vEnemys.at(i);
		enemyData.id = i + 1;
		enemyData.type = enemy->getType();
		enemyData.size = enemy->getContentSize();
		enemyData.position = enemy->getPosition();
		switch (enemyData.type)
		{
		case TYPE_ROTATE:
			enemyData.rotateTime = enemy->getRotateDuration();
			break;
		case TYPE_ROTATE_REVERSE:
			enemyData.rotateTime = enemy->getRotateDuration();
			break;
		case TYPE_MOVE:
			enemyData.position = enemy->getStartPoint();
			enemyData.destination = enemy->getDestPoint();
			enemyData.moveTime = enemy->getMoveDuration();
			break;
		case TYPE_BLINK:
			enemyData.blinkTime = enemy->getBlinkDuration();
			enemyData.blinkHideTime = enemy->getBlinkHideDuration();
			break;
		default:
			break;
		}
		pRoomData->enemysData.push_back(enemyData);
	}
}

void LevelMakeScene::setDropDownList(vector<GameLevelData*>* levelsData)
{
	m_pDropDownListLevel->clearAllLabels();
	for (size_t i = 0, j = levelsData->size(); i < j; i++)
		m_pDropDownListLevel->addLabel(Label::createWithSystemFont(StringUtils::format("%d", i + 1), "fonts/arial.ttf", 22));

	auto roomsData = levelsData->at(m_pDropDownListLevel->getSelectedIndex())->getRoomsData();
	m_pDropDownListRoom->clearAllLabels();
	for (size_t i = 0, j = roomsData->size(); i < j; i++)
		m_pDropDownListRoom->addLabel(Label::createWithSystemFont(StringUtils::format("%d", i + 1), "fonts/arial.ttf", 22));
}

bool LevelMakeScene::isJumpLineConflict(const Vec2& origin, const Vec2& control, const Vec2& destination)
{
	float dx = destination.x - origin.x;
	for (size_t i = 0, length = m_vAirPoints.size(); i < length; i++)
	{
		Point p = m_vAirPoints.at(i);
		if (p.x <= origin.x || p.x >= destination.x)
			continue;
		else
		{
			float t = (p.x - origin.x) / dx;
			float lineY = origin.y + 4 * control.y * t * (1 - t);
			if (p.y < lineY)
				return true;
		}
	}
	return false;
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
	structTmp->textField->setPlaceHolder(StringUtils::format("%d-%d", static_cast<int>(min), static_cast<int>(max)));
	structTmp->textField->setString(StringUtils::format("%d", static_cast<int>(number)));
	if (structTmp->slider)
		structTmp->slider->setPercent((number - min) * 100 / (max - min));
}

void LevelMakeScene::updateBlockTextFieldNumber(int tag, int number)
{
	TextFieldSliderBindInt* structBlock = &m_mTextFieldStructs.at(tag);
	structBlock->number = MAX(MIN(number, structBlock->max), structBlock->min);
	structBlock->textField->setString(StringUtils::format("%d", structBlock->number));
	if (structBlock->slider)
		structBlock->slider->setPercent((structBlock->number - structBlock->min) * 100 / (structBlock->max - structBlock->min));
}

void LevelMakeScene::updateBlockTextFieldMax(int tag, int max)
{
	TextFieldSliderBindInt* structBlock = &m_mTextFieldStructs.at(tag);
	structBlock->max = max;
	if (structBlock->number > structBlock->max)
	{
		structBlock->number = structBlock->max;
		structBlock->textField->setString(StringUtils::format("%d", structBlock->number));
	}
	if (structBlock->slider)
		structBlock->slider->setPercent((structBlock->number - structBlock->min) * 100 / (structBlock->max - structBlock->min));
}

void LevelMakeScene::updateBlockByLevel(int tag, int number)
{
	switch (tag)
	{
	case TAG_LEVEL_ROOM_WIDTH:
		this->updateBlockTextFieldMax(TAG_ENEMY_WIDTH, number);
		this->updateBlockTextFieldMax(TAG_ENEMY_POSITION_X, number);
		break;
	case TAG_LEVEL_ROOM_HEIGHT:
		this->updateBlockTextFieldMax(TAG_ENEMY_HEIGHT, number);
		this->updateBlockTextFieldMax(TAG_ENEMY_POSITION_Y, number);
		break;
	default:
		break;
	}
}

void LevelMakeScene::updateBlockType(int type)
{
	switch (type)
	{
	case TYPE_NORMAL:
	{
		this->setTextFieldStructEnable(TAG_ENEMY_ROTATE_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_X, false);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_Y, false);
		this->setTextFieldStructEnable(TAG_ENEMY_MOVE_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINK_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINKHIDE_TIME10, false);
		break;
	}
	case TYPE_ROTATE:
	{
		this->setTextFieldStructEnable(TAG_ENEMY_ROTATE_TIME10, true);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_X, false);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_Y, false);
		this->setTextFieldStructEnable(TAG_ENEMY_MOVE_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINK_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINKHIDE_TIME10, false);
		break;
	}
	case TYPE_ROTATE_REVERSE:
	{
		this->setTextFieldStructEnable(TAG_ENEMY_ROTATE_TIME10, true);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_X, false);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_Y, false);
		this->setTextFieldStructEnable(TAG_ENEMY_MOVE_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINK_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINKHIDE_TIME10, false);
		break;
	}
	case TYPE_MOVE:
	{
		this->setTextFieldStructEnable(TAG_ENEMY_ROTATE_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_X, true);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_Y, true);
		this->setTextFieldStructEnable(TAG_ENEMY_MOVE_TIME10, true);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINK_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINKHIDE_TIME10, false);
		break;
	}
	case TYPE_BLINK:
	{
		this->setTextFieldStructEnable(TAG_ENEMY_ROTATE_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_X, false);
		this->setTextFieldStructEnable(TAG_ENEMY_DESTINATION_Y, false);
		this->setTextFieldStructEnable(TAG_ENEMY_MOVE_TIME10, false);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINK_TIME10, true);
		this->setTextFieldStructEnable(TAG_ENEMY_BLINKHIDE_TIME10, true);
		break;
	}
	default:
		break;
	}
}

void LevelMakeScene::drawJumpLine(DrawNode* draw, const Vec2 &origin, const Vec2 &control, const Vec2 &destination, const Color4F &color)
{
	Color4F lineColor = color;
	if (this->isJumpLineConflict(origin, control, destination))
		lineColor = Color4F::RED;
	// draw line
	int segments = 20;
	Vec2* vertices = new (std::nothrow) Vec2[segments + 1];
	if (!vertices)
		return;
	float t = 0.0f;
	float dx = destination.x - origin.x;
	for (unsigned int i = 0; i < segments; i++)
	{
		vertices[i].x = origin.x + t * dx;
		vertices[i].y = origin.y + 4 * control.y * t * (1 - t);
		t += 1.0f / segments;
	}
	vertices[segments].x = destination.x;
	vertices[segments].y = destination.y;

	draw->drawPoly(vertices, segments + 1, false, lineColor);

	CC_SAFE_DELETE_ARRAY(vertices);
}

void LevelMakeScene::AddEstimateFrameText(Node* parent, float p1, float p2,
	int frameRate, float playerWidth, float speed, float posY, Color4B color) const
{
	int frameNumber = static_cast<int>(frameRate * (p2 - p1 - playerWidth) / speed);
	Text* text = Text::create(StringUtils::format("%d", frameNumber), "fonts/arial.ttf", 16);
	if (frameNumber <= 0)
		color = Color4B::RED;
	text->setTextColor(color);
	text->setPosition(Point(p1 + (p2 - p1) / 2, posY));
	parent->addChild(text);
}

void LevelMakeScene::setTextFieldStructEnable(int tag, bool isEnable)
{
	auto p = &m_mTextFieldStructs.at(tag);
	p->slider->setEnabled(isEnable);
	p->textField->setEnabled(isEnable);
}
