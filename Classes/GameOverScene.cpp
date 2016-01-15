#include "GameOverScene.h"
#include "GameMediator.h"
#include "MainMenuScene.h"
#include "GameScene.h"

Scene* GameOverScene::createScene(int deadCount)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = new(std::nothrow) GameOverScene();
	if (layer && layer->init(deadCount))
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

bool GameOverScene::init(int deadCount)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//////////////////////////////
	m_bIsNextLevelAvailable = false;

	GameMediator* pGameMediator = GameMediator::getInstance();
	pGameMediator->setDeadCount(deadCount);
	int deadCountAll = pGameMediator->getDeadCountAll(pGameMediator->getMaxGameLevel());

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
	int curLevel = pGameMediator->getCurGameLevel();
	auto levelData = pGameMediator->getGameLevelData()->at(curLevel - 1);
	textTitle->setString(levelData->getLevelName());
	auto nodeEnlargeAnimation = dynamic_cast<Node*>(layout->getChildByName("Node_EnlargeAnimation"));
	auto textDeadNumber = dynamic_cast<Text*>(nodeEnlargeAnimation->getChildByName("Text_DeadNumber"));
	if (deadCount == 0)
	{
		textDeadNumber->setString(StringUtils::format("Perfect this level!"));
		textDeadNumber->setTextColor(Color4B::ORANGE);
	}
	else if (deadCount == 1)
		textDeadNumber->setString(StringUtils::format("%d dead this level.", deadCount));
	else
		textDeadNumber->setString(StringUtils::format("%d deads this level.", deadCount));
	auto textDeadNumberAll = dynamic_cast<Text*>(nodeEnlargeAnimation->getChildByName("Text_DeadNumberAll"));
	if (deadCountAll == 0 && curLevel > 1)
	{
		textDeadNumberAll->setString(StringUtils::format("Perfect till now!"));
		textDeadNumberAll->setTextColor(Color4B::ORANGE);
	}
	else if (deadCountAll == 1)
		textDeadNumberAll->setString(StringUtils::format("%d dead total.", deadCountAll));
	else
		textDeadNumberAll->setString(StringUtils::format("%d deads total.", deadCountAll));
	int maxDeadTime = levelData->getMaxDeadTime();
	auto textNextRequirement = dynamic_cast<Text*>(layout->getChildByName("Text_NextRequirement"));
	textNextRequirement->setString(StringUtils::format("(Total deads < %d)", maxDeadTime));
	auto textFinish = dynamic_cast<Text*>(layout->getChildByName("Text_Finish"));

	if (pGameMediator->getCurGameLevel() == pGameMediator->getGameLevelCount()) // the last level, finish the game!
	{
		textNextRequirement->setVisible(false);
		buttonNext->setEnabled(false);
		buttonNext->setVisible(false);
	}
	else
	{
		textFinish->setVisible(false);
		if (pGameMediator->getCurGameLevel() < pGameMediator->getMaxGameLevel())
		{
			textNextRequirement->setVisible(false);
			m_bIsNextLevelAvailable = true;
		}
		else
		{
			if (deadCountAll < maxDeadTime)
			{
				textNextRequirement->setTextColor(Color4B::GREEN);
				pGameMediator->setMaxGameLevel();
				m_bIsNextLevelAvailable = true;
			}
			else
			{
				textNextRequirement->setTextColor(Color4B::RED);
				buttonNext->setEnabled(false);
				m_bIsNextLevelAvailable = false;
			}
		}
	}

    return true;
}

void GameOverScene::buttonCallback_Next(Ref* pSender)
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("GameOverImage");
	GameMediator::getInstance()->gotoNextGameLevel();
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
	if (m_bIsNextLevelAvailable)
	{
		GameMediator::getInstance()->gotoNextGameLevel();
	}
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
}
