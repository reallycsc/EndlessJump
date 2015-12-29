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

#if (DEBUG_FLAG == 0)
	buttonEditor->setEnabled(false);
	buttonEditor->setVisible(false);
#endif

    return true;
}

void MainMenuScene::buttonCallback_Play(Ref* pSender)
{
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void MainMenuScene::buttonCallback_LevelEditor(Ref* pSender)
{
	Director::getInstance()->replaceScene(LevelMakeScene::createScene());
}
