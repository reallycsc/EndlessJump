#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"

Scene* GameOverScene::createScene(int deadNum)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = new(std::nothrow) GameOverScene();
	if (layer && layer->init(deadNum))
	{
		layer->autorelease();
	}
	else
	{
		delete layer;
		layer = NULL;
	}

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GameOverScene::init(int deadNum)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	m_pGameMediator = GameMediator::getInstance();

	// add background screenshot
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("GameOverImage"));
	sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sprite);

	// load csb
	auto rootNode = CSLoader::createNode("GameOverScene.csb");
	auto animate = CSLoader::createTimeline("GameOverScene.csb");
	rootNode->runAction(animate);
	animate->gotoFrameAndPlay(0, false);
	this->addChild(rootNode);

	auto layout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_GameOver"));
	// get button
	auto buttonNext = dynamic_cast<Button*>(layout->getChildByName("Button_Next"));
	buttonNext->addClickEventListener(CC_CALLBACK_1(GameOverScene::buttonCallback_Next, this));
	auto buttonRetry = dynamic_cast<Button*>(layout->getChildByName("Button_Retry"));
	buttonRetry->addClickEventListener(CC_CALLBACK_1(GameOverScene::buttonCallback_Retry, this));
	auto buttonMainMenu = dynamic_cast<Button*>(layout->getChildByName("Button_MainMenu"));
	buttonMainMenu->addClickEventListener(CC_CALLBACK_1(GameOverScene::buttonCallback_MainMenu, this));
	// get text
	auto textTitle = dynamic_cast<Text*>(layout->getChildByName("Text_Title"));
	int curLevel = m_pGameMediator->getCurGameLevel();
	auto levelData = m_pGameMediator->getGameLevelData()->at(curLevel - 1);
	textTitle->setString(levelData->getLevelName());
	auto textDeadNumber = dynamic_cast<Text*>(layout->getChildByName("Node_EnlargeAnimation")->getChildByName("Text_DeadNumber"));
	textDeadNumber->setString(StringUtils::format("Dead %d times.", deadNum));
	int maxDeadTime = levelData->getMaxDeadTime();
	auto textNextRequirement = dynamic_cast<Text*>(layout->getChildByName("Text_NextRequirement"));
	textNextRequirement->setString(StringUtils::format("(Dead < %d times)", maxDeadTime));

	if (m_pGameMediator->getCurGameLevel() == m_pGameMediator->getMaxGameLevel()) // the last level, finish the game!
	{
		textNextRequirement->setVisible(false);
		buttonNext->setEnabled(false);
		buttonNext->setVisible(false);
	}
	else
	{
		if (deadNum <= maxDeadTime)
		{
			textNextRequirement->setTextColor(Color4B::GREEN);
		}
		else
		{
			textNextRequirement->setTextColor(Color4B::RED);
			buttonNext->setEnabled(false);
		}
	}

    return true;
}

void GameOverScene::buttonCallback_Next(Ref* pSender)
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("GameOverImage");
	m_pGameMediator->setCurGameLevel(m_pGameMediator->getCurGameLevel() + 1);
	m_pGameMediator->saveGameData();
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void GameOverScene::buttonCallback_Retry(Ref* pSender)
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("GameOverImage");
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void GameOverScene::buttonCallback_MainMenu(Ref* pSender)
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("GameOverImage");
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
}
