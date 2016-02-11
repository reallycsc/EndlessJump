#include "GameOverScene.h"
#include "GameMediator.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "CreditScene.h"
#include "CSCClass/CSC_IOSHelper.h"

Scene* GameOverScene::createScene(int deadCount)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = new(std::nothrow) GameOverScene();
	layer->init(deadCount);
	layer->autorelease();

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
	GameMediator* pGameMediator = GameMediator::getInstance();
	int totalDeadCount = pGameMediator->getTotalDeadCount();

	// add background screenshot
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("GameOverImage"));
	sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sprite);

	// load csb
	auto rootNode = CSLoader::createNode("GameOverScene.csb");
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
    this->addChild(rootNode);

	auto layout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_GameOver"));
	// get button
	auto buttonNext = dynamic_cast<Button*>(layout->getChildByName("Button_Next"));
	buttonNext->addClickEventListener(CC_CALLBACK_1(GameOverScene::buttonCallback_Next, this));
	auto buttonUnlockNow = dynamic_cast<Button*>(layout->getChildByName("Button_UnlockNow"));
	buttonUnlockNow->addClickEventListener(CC_CALLBACK_1(GameOverScene::buttonCallback_UnlockNow, this));
	buttonUnlockNow->setVisible(false);
	buttonUnlockNow->setEnabled(false);
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
	if (totalDeadCount == 0 && curLevel > 1)
	{
		textDeadNumberAll->setString(StringUtils::format("Perfect till now!"));
		textDeadNumberAll->setTextColor(Color4B::ORANGE);
	}
	else if (totalDeadCount == 1)
		textDeadNumberAll->setString(StringUtils::format("%d dead total.", totalDeadCount));
	else
		textDeadNumberAll->setString(StringUtils::format("%d deads total.", totalDeadCount));
	int levelMinDeadCount = levelData->getMaxDeadTime();
	auto textNextRequirement = dynamic_cast<Text*>(layout->getChildByName("Text_NextRequirement"));
	textNextRequirement->setString(StringUtils::format("(Total deads < %d)", levelMinDeadCount));

	if (pGameMediator->getCurGameLevel() == pGameMediator->getGameLevelCount()) // the last level, finish the game!
	{
		textNextRequirement->setVisible(false);
		buttonNext->setTitleText("Finish");
	}
	else
	{
		if (pGameMediator->getCurGameLevel() < pGameMediator->getMaxGameLevel())
		{
			textNextRequirement->setVisible(false);
		}
		else
		{
			if (totalDeadCount < levelMinDeadCount)
			{
				textNextRequirement->setTextColor(Color4B::GREEN);
				pGameMediator->setMaxGameLevel();
			}
			else
			{
				textNextRequirement->setTextColor(Color4B::RED);
				buttonUnlockNow->setVisible(true);
				buttonUnlockNow->setEnabled(true);
				buttonNext->setVisible(false);
				buttonNext->setEnabled(false);
			}
		}
	}

	// add custom listener
	auto listener = EventListenerCustom::create(EVENT_PURCHASED + "com.reallycsc.lifeishard.unlocklevel", [=](EventCustom* event) {
		Director::getInstance()->getTextureCache()->removeTextureForKey("GameOverImage");
		GameMediator::getInstance()->gotoNextGameLevel();
		Director::getInstance()->replaceScene(GameScene::createScene());
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // play animate
    auto layout_half_width = layout->getContentSize().width / 2;
    layout->setPositionX(-layout_half_width);
    layout->runAction(MoveTo::create(0.25f, Point(layout_half_width, layout->getPositionY())));

    return true;
}

void GameOverScene::buttonCallback_Next(Ref* pSender)
{
	GameMediator* pGameMediator = GameMediator::getInstance();
	Director::getInstance()->getTextureCache()->removeTextureForKey("GameOverImage");
	if (pGameMediator->getCurGameLevel() == pGameMediator->getGameLevelCount()) // the last level, finish the game!
		Director::getInstance()->replaceScene(CreditScene::createScene());
	else
	{
		GameMediator::getInstance()->gotoNextGameLevel();
		Director::getInstance()->replaceScene(GameScene::createScene());
	}
}

void GameOverScene::buttonCallback_UnlockNow(Ref* pSender)
{
#ifdef IAP_TEST
	CSCClass::CSC_IOSHelper::getInstance()->IAP_purchaseProduct(false, "com.reallycsc.lifeishard.unlocklevel");
#else
	CSCClass::CSC_IOSHelper::getInstance()->IAP_purchaseProduct(false, "com.reallycsc.lifeishard.unlocklevel");
#endif
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
