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
	// load csb
	auto rootNode = CSLoader::createNode("MainMenuScene.csb");
	this->addChild(rootNode);

	auto buttonPlay = dynamic_cast<Button*>(rootNode->getChildByName("Button_Play"));
	buttonPlay->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_Play, this));
	auto buttonEditor = dynamic_cast<Button*>(rootNode->getChildByName("Button_LevelEditor"));
	buttonEditor->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_LevelEditor, this));

	// set level select boxes
	auto scrollView = dynamic_cast<ScrollView*>(rootNode->getChildByName("ScrollView_Levels"));
	float scrollHeight = scrollView->getContentSize().height;
	auto pGameMediator = GameMediator::getInstance();
	// get level data
	int maxLevel = pGameMediator->getMaxGameLevel();
	auto levelDeadCounts = pGameMediator->getLevelMinDeadCount();
	m_vLevelButtons.clear();
	for (size_t i = 0, j = pGameMediator->getGameLevelCount(); i < j; i++)
	{
		auto levelNode = CSLoader::createNode("MainMenuScene_NodeLevel.csb");
		scrollView->addChild(levelNode);
		// button
		auto buttonLevel = dynamic_cast<Button*>(levelNode->getChildByName("Button_Level"));
		levelNode->setPosition((buttonLevel->getContentSize().width / 2 + 100) * i + 100, scrollHeight / 2);
		buttonLevel->setTitleText(StringUtils::format("%d", i + 1));
		buttonLevel->setTag(i + 1);
		buttonLevel->addClickEventListener(CC_CALLBACK_1(MainMenuScene::buttonCallback_LevelSelect, this));
		if (i > maxLevel - 1)
		{
			buttonLevel->setEnabled(false);
		}
		m_vLevelButtons.pushBack(buttonLevel);
		// text
		auto textDeadCount = dynamic_cast<Text*>(levelNode->getChildByName("Text_DeadTime"));
		int deadCount = levelDeadCounts->at(i);
		if (deadCount < 0)
		{
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
	}

#if (DEBUG_FLAG == 0)
	buttonEditor->setEnabled(false);
	buttonEditor->setVisible(false);
#endif

	// draw select rect
	m_nSelectLevel = pGameMediator->getCurGameLevel();
	m_cButtonNormalColor = m_vLevelButtons.at(m_nSelectLevel - 1)->getColor();
	m_vLevelButtons.at(m_nSelectLevel - 1)->setColor(Color3B::ORANGE);

    return true;
}

void MainMenuScene::buttonCallback_LevelSelect(Ref* pSender)
{
	Button*	button = static_cast<Button*>(pSender);
	if (m_nSelectLevel != button->getTag())
	{
		m_nSelectLevel = button->getTag();
		button->setColor(Color3B::ORANGE);
		for (size_t i = 0, j = m_vLevelButtons.size(); i < j; i++)
		{
			Button* tmpBtn = m_vLevelButtons.at(i);
			if (tmpBtn->isEnabled() && tmpBtn->getTag() != m_nSelectLevel)
			{
				tmpBtn->setColor(m_cButtonNormalColor);
			}
		}
		GameMediator::getInstance()->setCurGameLevel(m_nSelectLevel);
		GameMediator::getInstance()->saveIntegerGameDataForKey("CurLevel", m_nSelectLevel);
	}
}

void MainMenuScene::buttonCallback_Play(Ref* pSender)
{
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void MainMenuScene::buttonCallback_LevelEditor(Ref* pSender)
{
	Director::getInstance()->replaceScene(LevelMakeScene::createScene());
}
