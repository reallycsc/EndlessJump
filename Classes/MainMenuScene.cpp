#include "MainMenuScene.h"
#include "GameScene.h"
#include "LevelMakeScene.h"
#include "CreditScene.h"

Scene* MainMenuScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = MainMenuScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool MainMenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto pGameMediator = GameMediator::getInstance();
	auto levelsData = pGameMediator->getGameLevelData();
	// add two color layer
	m_fLayerColorSpeed = visibleSize.height / 2;
	m_vLevelColors.clear();
	for (size_t i = 0, length = pGameMediator->getGameLevelCount(); i < length; i++)
	{
		auto roomData = levelsData->at(i)->getRoomsData()->at(0);
		m_vLevelColors.push_back(pair<Color3B, Color3B>(roomData.color, roomData.enemy_color));
	}
	if (m_vLevelColors.size() > 0)
	{
		random_shuffle(m_vLevelColors.begin(), m_vLevelColors.end());
		m_nCurColorIndex = 0;

		m_pLayerColor1 = LayerGradient::create(Color4B::BLACK, Color4B(m_vLevelColors.at(m_nCurColorIndex).first));
		m_pLayerColor1->setPositionY(-visibleSize.height);
		this->addChild(m_pLayerColor1, 1);

		m_pLayerColor2 = this->createLayerColor();
		m_pLayerColor2->setPositionY(-visibleSize.height * 2);
		this->addChild(m_pLayerColor2, 1);
	}

	// load csb
	auto rootNode = CSLoader::createNode("MainMenuScene.csb");
	this->addChild(rootNode, 2);
	// get button
	auto buttonEditor = dynamic_cast<Button*>(rootNode->getChildByName("Button_LevelEditor"));
	buttonEditor->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_LevelEditor, this));
	auto buttonCredit = dynamic_cast<Button*>(rootNode->getChildByName("Button_Credit"));
	buttonCredit->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_Credit, this));
	// get text
	auto textTotalDead = dynamic_cast<Text*>(rootNode->getChildByName("Text_TotalDead"));
	auto textSubtitle = dynamic_cast<Text*>(rootNode->getChildByName("Text_Subtitle"));
	textSubtitle->setVisible(false);
	textSubtitle->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		DelayTime::create(10.0f),
		Blink::create(0.1f, 1))));
	int maxLevel = pGameMediator->getMaxGameLevel();
	int curLevel = pGameMediator->getCurGameLevel();
	int totalDead = pGameMediator->getDeadCountAll(maxLevel);
	textTotalDead->setString(StringUtils::format("Total Dead: %d", totalDead));

	// set level select boxes
	auto scrollView = dynamic_cast<ScrollView*>(rootNode->getChildByName("ScrollView_Levels"));
	float scrollWidth = scrollView->getContentSize().width;
	// get level data
	auto levelDeadCounts = pGameMediator->getLevelMinDeadCount();
	int innerHeight = 0;
	int maxLevelHeight = 0;
	int interval = 50;
	m_vLevelDeadCount.clear();
	for (size_t i = 0, length = pGameMediator->getGameLevelCount(); i < length; i++)
	{
		auto levelData = levelsData->at(i);
		auto levelNode = CSLoader::createNode("MainMenuScene_NodeLevel.csb");
		scrollView->addChild(levelNode);
		// button
		auto buttonLevel = dynamic_cast<Button*>(levelNode->getChildByName("Button_Level"));
		int posY = (buttonLevel->getContentSize().height / 2 + interval) * i + interval;
		levelNode->setPosition(scrollWidth / 2 , posY);
		innerHeight = posY + buttonLevel->getContentSize().height / 2;
		buttonLevel->setTitleText(StringUtils::format("%d", i + 1));
		auto roomData = levelData->getRoomsData()->at(0);
		auto levelColor = roomData.color;
		buttonLevel->setColor(levelColor);
		buttonLevel->setTag(i + 1);
		buttonLevel->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_LevelPlay, this));
		buttonLevel->setTitleText(StringUtils::format("%d:", i + 1) + levelData->getLevelName());
		buttonLevel->setTitleFontSize(26);
		// if the next level of curLevel open condition < total dead, need open the next level (when update new level will have this situation)
		int maxDeadCount = levelData->getMaxDeadTime();
		if (i > 0 && i == curLevel - 1 && 
			levelDeadCounts->at(i - 1) >= 0 && 
			totalDead < maxDeadCount)
		{
			pGameMediator->gotoNextGameLevel();
			maxLevel = pGameMediator->getMaxGameLevel();
		}
		else if (i > maxLevel - 1)
		{
			buttonLevel->setTitleText(StringUtils::format("%d", i + 1));
			buttonLevel->setTitleFontSize(46);
			buttonLevel->setEnabled(false);
			buttonLevel->setColor(Color3B::GRAY);
		}
		else if (i == maxLevel - 1)
			maxLevelHeight = innerHeight;
		// text
		auto textDeadCount = dynamic_cast<Text*>(levelNode->getChildByName("Text_DeadTime"));
		int deadCount = levelDeadCounts->at(i);
		if (deadCount < 0)
		{
			if (i == maxLevel)
			{
				textDeadCount->setString(StringUtils::format("<%d deads", maxDeadCount));
				textDeadCount->setTextColor(Color4B::GRAY);
			}
			else
				textDeadCount->setVisible(false);
		}
		else
		{
			if (deadCount == 0)
			{
				textDeadCount->setString(StringUtils::format("perfect"));
				textDeadCount->setTextColor(Color4B::ORANGE);
			}
			else if (deadCount == 1)
				textDeadCount->setString(StringUtils::format("%d dead", deadCount));
			else
				textDeadCount->setString(StringUtils::format("%d deads", deadCount));
		}
		m_vLevelDeadCount.pushBack(textDeadCount);
	}
	int scrollInnerHeight = scrollView->getInnerContainerSize().height;
	innerHeight = MAX(scrollInnerHeight, innerHeight + interval);
	scrollView->setInnerContainerSize(Size(scrollView->getInnerContainerSize().width, innerHeight));
	scrollView->setInnerContainerPosition(Point(0,-maxLevelHeight + scrollInnerHeight / 2));

#ifndef LEVEL_MAKER_MODE
	buttonEditor->setEnabled(false);
	buttonEditor->setVisible(false);
#endif

	this->scheduleUpdate();

    return true;
}

void MainMenuScene::update(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto dy = m_fLayerColorSpeed * dt;
	auto new_y1 = m_pLayerColor1->getPositionY() + dy;
	auto new_y2 = m_pLayerColor2->getPositionY() + dy;
	if (new_y1 >= visibleSize.height) // remove & add layer1
	{
		auto offset = new_y1 - visibleSize.height;
		m_pLayerColor1->removeFromParent();
		m_pLayerColor1 = nullptr;
		m_pLayerColor1 = this->createLayerColor();
		m_pLayerColor1->setPositionY(offset - visibleSize.height);
		this->addChild(m_pLayerColor1, 1);
	}
	else
		m_pLayerColor1->setPositionY(new_y1);

	if (new_y2 >= visibleSize.height) // remove & add layer2
	{
		auto offset = new_y2 - visibleSize.height;
		m_pLayerColor2->removeFromParent();
		m_pLayerColor2 = nullptr;
		m_pLayerColor2 = this->createLayerColor();
		m_pLayerColor2->setPositionY(offset - visibleSize.height);
		this->addChild(m_pLayerColor2, 1);
	}
	else
		m_pLayerColor2->setPositionY(new_y2);
}

void MainMenuScene::buttonCallback_LevelPlay(Ref* pSender)
{
	Button*	button = static_cast<Button*>(pSender);
	int curLevel = button->getTag();
	GameMediator::getInstance()->setCurGameLevel(curLevel);
	GameMediator::getInstance()->saveIntegerGameDataForKey("CurLevel", curLevel);
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void MainMenuScene::buttonCallback_LevelEditor(Ref* pSender)
{
	Director::getInstance()->replaceScene(LevelMakeScene::createScene());
}

void MainMenuScene::buttonCallback_Credit(Ref* pSender)
{
	Director::getInstance()->replaceScene(CreditScene::createScene());
}

LayerGradient* MainMenuScene::createLayerColor()
{
	LayerGradient* layer = nullptr;
	if (m_nCurColorIndex + 1 < m_vLevelColors.size())
	{
		layer = LayerGradient::create(Color4B(m_vLevelColors.at(m_nCurColorIndex).first), Color4B(m_vLevelColors.at(m_nCurColorIndex + 1).first));
		m_nCurColorIndex++;
	}
	else
	{
		layer = LayerGradient::create(Color4B(m_vLevelColors.at(m_nCurColorIndex).first), Color4B(m_vLevelColors.at(0).first));
		m_nCurColorIndex = 0;
	}
	return layer;
}
