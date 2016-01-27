#include "StoryScene.h"
#include "GameMediator.h"
#include "CSCClass/CSCAction/TextTypeWriting.h"

Scene* StoryScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = StoryScene::create();
	if (!layer)
		return nullptr;

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool StoryScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//////////////////////////////
	GameMediator* pGameMediator = GameMediator::getInstance();
	m_bIsFinished = false;

	// add storyline
	m_pvStorylines = pGameMediator->getLevelStorylines(pGameMediator->getCurGameLevel());
	if (!m_pvStorylines)
		return false;

	this->showStoryline(NULL, 0);

	// bind touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(StoryScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(StoryScene::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(StoryScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool StoryScene::onTouchBegan(Touch* touch, Event* event)
{
	if (m_bIsFinished)
	{
		Director::getInstance()->popScene();
		return true;
	}
	return false;
}

void StoryScene::onTouchMoved(Touch* touch, Event* event)
{
}

void StoryScene::onTouchEnded(Touch* touch, Event* event)
{
}

void StoryScene::showStoryline(Node* pSender, int index)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto interval = FONT_SIZE * 2;
	auto lines = m_pvStorylines->size();
	if (index >= lines)
	{
		m_bIsFinished = true;
		auto touchLabel = Label::createWithTTF("Continue", "fonts/fzzj.ttf", 24);
		touchLabel->setPosition(visibleSize.width / 2, MIN(visibleSize.height * 0.2, visibleSize.height * 0.8 - (index + 1) * interval));
		this->addChild(touchLabel);
		touchLabel->setOpacity(0);
		touchLabel->runAction(RepeatForever::create(Sequence::createWithTwoActions(
			FadeIn::create(1.5f),
			FadeOut::create(1.5f))));
		return;
	}

	auto str = m_pvStorylines->at(index);
	auto storyline = Label::createWithTTF(str, "fonts/fzzj.ttf", FONT_SIZE);
	
	
	auto stencil = DrawNode::create();
	stencil->drawSolidRect(Point::ZERO, storyline->getContentSize(), Color4F::BLACK);
	auto clippingNode = ClippingNode::create(stencil);
	clippingNode->setInverted(true);
	clippingNode->addChild(storyline);
	clippingNode->setPosition(visibleSize.width / 2, visibleSize.height * 0.8 - index * interval);
	this->addChild(clippingNode);

	auto storylineSize = storyline->getContentSize();
	auto storylineHalfWidth = storylineSize.width / 2;
	auto storylineHalfHeight = storylineSize.height / 2;
	stencil->setPosition(-storylineHalfWidth, -storylineHalfHeight);
	stencil->runAction(Sequence::create(
		MoveTo::create(str.size() * WORD_DISPLAY_SPEED, Point(storylineHalfWidth, -storylineHalfHeight)),
		DelayTime::create(0.5f),
		CallFuncN::create(CC_CALLBACK_1(StoryScene::showStoryline, this, index + 1)),
		NULL
		));
}
