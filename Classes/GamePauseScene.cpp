#include "GamePauseScene.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameOverScene.h"

Scene* GamePauseScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = GamePauseScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool GamePauseScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//////////////////////////////
	// add background screenshot
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* sprite = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey("GamePauseImage"));
	sprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(sprite);

	// load csb
	auto rootNode = CSLoader::createNode("GamePauseScene.csb");
	m_pAnimate = CSLoader::createTimeline("GamePauseScene.csb");
	m_pAnimate->setFrameEventCallFunc(CC_CALLBACK_1(GamePauseScene::onFrameEvent, this));
	rootNode->runAction(m_pAnimate);
	m_pAnimate->pause();
	m_pAnimate->play("scene_start", false);
	this->addChild(rootNode);

	m_pLayout = dynamic_cast<Layout*>(rootNode->getChildByName("Panel_GamePause"));
	// get button
	auto buttonResume = dynamic_cast<Button*>(m_pLayout->getChildByName("Button_Resume"));
	buttonResume->addClickEventListener(CC_CALLBACK_1(GamePauseScene::buttonCallback_Resume, this));
	auto buttonMainMenu = dynamic_cast<Button*>(m_pLayout->getChildByName("Button_MainMenu"));
	buttonMainMenu->addClickEventListener(CC_CALLBACK_1(GamePauseScene::buttonCallback_MainMenu, this));
	// get text
	m_pTextCountdown = dynamic_cast<Text*>(rootNode->getChildByName("Text_Countdown"));
	m_pTextCountdown->setVisible(false);
    return true;
}

void GamePauseScene::buttonCallback_Resume(Ref* pSender)
{
	m_pLayout->setVisible(false);
	m_pTextCountdown->setVisible(true);
	m_pTextCountdown->setString("3");
	m_pAnimate->play("countdown3", false);
}

void GamePauseScene::buttonCallback_MainMenu(Ref* pSender)
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("GamePauseImage");
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
}

void GamePauseScene::onFrameEvent(Frame* frame) const
{
	EventFrame* event = dynamic_cast<EventFrame*>(frame);
	if (!event)
		return;

	string str = event->getEvent();
	if (str == "countdown3_end")
	{
		m_pTextCountdown->setString("2");
		m_pAnimate->play("countdown2", false);
	}
	else if (str == "countdown2_end")
	{
		m_pTextCountdown->setString("1");
		m_pAnimate->play("countdown1", false);
	}
	else if (str == "countdown1_end")
	{
		Director::getInstance()->getTextureCache()->removeTextureForKey("GamePauseImage");
		Director::getInstance()->popScene();
	}
}
