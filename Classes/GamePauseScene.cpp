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
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
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
    
    // play animate
    auto layout_half_width = m_pLayout->getContentSize().width / 2;
    m_pLayout->setPositionX(-layout_half_width);
    m_pLayout->runAction(MoveTo::create(0.25f, Point(layout_half_width, m_pLayout->getPositionY())));
    
    return true;
}

void GamePauseScene::buttonCallback_Resume(Ref* pSender)
{
	m_pLayout->setVisible(false);
	m_pTextCountdown->setVisible(true);
	m_pTextCountdown->setString("3");
    m_pTextCountdown->setScale(2);
    m_pTextCountdown->runAction(Sequence::create(ScaleTo::create(0.5f, 1),
                                                 CallFunc::create([=](){
        m_pTextCountdown->setString("2");
        m_pTextCountdown->setScale(2);}),
                                                 ScaleTo::create(0.5f, 1),
                                                 CallFunc::create([=](){
        m_pTextCountdown->setString("1");
        m_pTextCountdown->setScale(2);}),
                                                 ScaleTo::create(0.5f, 1),
                                                 CallFunc::create([=](){
        Director::getInstance()->getTextureCache()->removeTextureForKey("GamePauseImage");
        Director::getInstance()->popScene();}),
                                                 NULL));
}

void GamePauseScene::buttonCallback_MainMenu(Ref* pSender)
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("GamePauseImage");
	Director::getInstance()->popScene();
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
}