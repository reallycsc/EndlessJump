#include "CreditScene.h"
#include "MainMenuScene.h"
#include "GameMediator.h"
#include "CSCClass/AudioCtrl.h"

Scene* CreditScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = new(std::nothrow) CreditScene();
	if (layer && layer->init())
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

bool CreditScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//////////////////////////////
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// load csb
	auto rootNode = CSLoader::createNode("CreditScene.csb");
    rootNode->setContentSize(visibleSize);
    ui::Helper::doLayout(rootNode);
	this->addChild(rootNode);

	auto scrollView = dynamic_cast<ScrollView*>(rootNode->getChildByName("ScrollView_Credit"));

	auto layout_finished = dynamic_cast<Layout*>(scrollView->getChildByName("Panel_Credit_Finished"));
	auto layout_unfinished = dynamic_cast<Layout*>(scrollView->getChildByName("Panel_Credit_Unfinished"));
	auto pGameMediator = GameMediator::getInstance();
	Layout* layout = nullptr;
	if (pGameMediator->getCurGameLevel() == pGameMediator->getGameLevelCount()) // the last level, finish the game!
	{
		layout = layout_finished;
		layout_unfinished->removeFromParent();
		if (pGameMediator->getTotalDeadCount() == 0)
			dynamic_cast<Text*>(layout_finished->getChildByName("Text_Try"))->removeFromParent();
	}
	else
	{
		layout = layout_unfinished;
		layout_finished->removeFromParent();
	}
	
	// get child 
	auto textTitle = dynamic_cast<Text*>(layout->getChildByName("Text_Title"));
	auto nodeCreditList = dynamic_cast<Node*>(layout->getChildByName("Node_CreditList"));
	auto textMusic = dynamic_cast<Text*>(nodeCreditList->getChildByName("Text_Music"));

	// get button
	auto buttonMainMenu = dynamic_cast<Button*>(layout->getChildByName("Button_MainMenu"));
	buttonMainMenu->addClickEventListener(CC_CALLBACK_1(CreditScene::buttonCallback_MainMenu, this));

	// set music text
	auto musicList = CSCClass::AudioCtrl::getInstance()->getBackgroundMusicList();
	auto offset = 20;
	for (size_t i = 0, length = musicList->size(); i < length; i++)
	{
		auto music_fullname = musicList->at(i);
		auto music_name = music_fullname.substr(0, music_fullname.find_last_of("."));
		auto label = Label::createWithTTF(music_name, "fonts/fzzj.ttf", 32);
		label->setAnchorPoint(Vec2(0.5f, 1));
		label->setPosition(textMusic->getContentSize().width / 2, -offset);
		label->enableShadow();
		textMusic->addChild(label);
		offset += 10 + label->getContentSize().height;
	}

	// change size and position due to offset
	auto layout_height = layout->getContentSize().height + offset;
	layout->setContentSize(Size(layout->getContentSize().width, layout_height));
	textTitle->setPositionY(textTitle->getPositionY() + offset);
	nodeCreditList->setPositionY(nodeCreditList->getPositionY() + offset);

	// roll up
	auto real_height = MAX(visibleSize.height, layout_height);
	auto duration = real_height / 200;
	layout->setEnabled(false);
	scrollView->setEnabled(false);
	layout->runAction(Sequence::createWithTwoActions(
		MoveTo::create(duration, Point(scrollView->getPositionX(), real_height)),
		CallFunc::create([=]
	{
		scrollView->setInnerContainerSize(Size(scrollView->getInnerContainerSize().width, real_height));
		scrollView->jumpToBottom();
		scrollView->setScrollBarEnabled(false);
		scrollView->setEnabled(true);
		layout->setEnabled(true);
	})));

    return true;
}

void CreditScene::buttonCallback_MainMenu(Ref* pSender)
{
	Director::getInstance()->replaceScene(MainMenuScene::createScene());
}
