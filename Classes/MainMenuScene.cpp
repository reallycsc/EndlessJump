#include "MainMenuScene.h"
#include "GameScene.h"
#include "LevelMakeScene.h"

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
	auto pGameMediator = GameMediator::getInstance();
	// load csb
	auto rootNode = CSLoader::createNode("MainMenuScene.csb");
	this->addChild(rootNode);
	// get button
	auto buttonEditor = dynamic_cast<Button*>(rootNode->getChildByName("Button_LevelEditor"));
	buttonEditor->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_LevelEditor, this));
	// get text
	auto textTotalDead = dynamic_cast<Text*>(rootNode->getChildByName("Text_TotalDead"));
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
	auto levelsData = pGameMediator->getGameLevelData();
	int interval = 50;
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
		buttonLevel->setColor(levelData->getRoomsData()->at(0).color);
		buttonLevel->setTag(i + 1);
		buttonLevel->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_LevelPlay, this));
		// if the next level of curLevel open condition < total dead, need open the next level (when update new level will have this situation)
		if (i > 0 && i == curLevel && 
			levelDeadCounts->at(i - 1) >= 0 && 
			totalDead < levelData->getMaxDeadTime())
		{
			pGameMediator->gotoNextGameLevel();
		}
		else if (i > maxLevel - 1)
		{
			buttonLevel->setEnabled(false);
			buttonLevel->setColor(Color3B::GRAY);
		}
		else if (i == maxLevel - 1)
			maxLevelHeight = innerHeight;
		// text
		auto textDeadCount = dynamic_cast<Text*>(levelNode->getChildByName("Text_DeadTime"));
		int deadCount = levelDeadCounts->at(i);
		if (deadCount < 0)
			textDeadCount->setVisible(false);
		else
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
	int scrollInnerHeight = scrollView->getInnerContainerSize().height;
	innerHeight = MAX(scrollInnerHeight, innerHeight + interval);
	scrollView->setInnerContainerSize(Size(scrollView->getInnerContainerSize().width, innerHeight));
	//scrollView->setInnerContainerPosition(Point::ZERO);
	scrollView->setInnerContainerPosition(Point(0,-maxLevelHeight + scrollInnerHeight / 2));

#ifndef DEBUG_MODE
	buttonEditor->setEnabled(false);
	buttonEditor->setVisible(false);
#endif

    return true;
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
