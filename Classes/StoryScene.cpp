#include "StoryScene.h"
#include "GameMediator.h"

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
	m_fPositionY = 0;

	// add storyline
	auto story = pGameMediator->getLevelStoryLines(pGameMediator->getCurGameLevel());
	if (!story)
		return false;
	if (story->end == 0)
		this->showStoryline(NULL, &story->line_data, 0);
	else
	{
		auto total_dead_count = GameMediator::getInstance()->getTotalDeadCount();
		auto end_story = GameMediator::getInstance()->getEndStoryLines(story->end);
		if (total_dead_count > 500)
			this->showStoryline(NULL, &end_story->at("deadCount>500"), 0);
		else if (total_dead_count > 0)
			this->showStoryline(NULL, &end_story->at("deadCount>0"), 0);
		else
			this->showStoryline(NULL, &end_story->at("deadCount==0"), 0);
	}

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

void StoryScene::showStoryline(Node* pSender, vector<LineData>* storylines, int index)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (index >= storylines->size()) // storyline is end, add 'continue'
	{
		m_bIsFinished = true;
		auto touchLabel = Label::createWithTTF("Continue", "fonts/fzzj.ttf", 24);
		touchLabel->setPosition(visibleSize.width / 2, visibleSize.height * 0.1);
		this->addChild(touchLabel);
		touchLabel->setOpacity(0);
		touchLabel->runAction(RepeatForever::create(Sequence::createWithTwoActions(
			FadeIn::create(1.5f),
			FadeOut::create(1.5f))));
		return;
	}

	auto story_data = storylines->at(index);

	// make label
	auto textWidthNew = visibleSize.width - 20;
#ifdef LEVEL_MAKER_MODE
	textWidthNew = visibleSize.width / 2 - 20;
#endif
	auto storyline = Label::createWithTTF(story_data.text, "fonts/fzzj.ttf", story_data.size, Size(textWidthNew, 0));
	storyline->setColor(story_data.color);
	auto storylineSize = storyline->getContentSize();
	auto storylineHalfWidth = storylineSize.width / 2;
	auto storylineHalfHeight = storylineSize.height / 2;
	
	// clipping moveing
	auto stencil = Node::create();
	auto wrapLines = storyline->getStringNumLines();
	auto singleLineHieght = storylineSize.height / wrapLines;
	auto delay = 0.0f;
	for (size_t i = 0; i < wrapLines; i++)
	{
		auto block = DrawNode::create();
		block->drawSolidRect(Point::ZERO, Size(storylineSize.width, singleLineHieght), Color4F::BLACK);
		auto position_y = storylineHalfHeight - singleLineHieght*(i + 1);
		block->setPosition(-storylineHalfWidth, position_y);
		stencil->addChild(block);

		auto duration = 1.5f;
		if (i == wrapLines - 1) // the last one
			block->runAction(Sequence::create(
				DelayTime::create(delay),
				MoveTo::create(duration, Point(storylineHalfWidth, position_y)),
				CallFuncN::create(CC_CALLBACK_1(StoryScene::showStoryline, this, storylines, index + 1)),
				NULL
				));
		else
			block->runAction(Sequence::createWithTwoActions(
				DelayTime::create(delay),
				MoveTo::create(duration, Point(storylineHalfWidth, position_y))
				));
		delay += duration;
	}
	
	// set new position y
	auto interval = story_data.interval;
	if (interval == 0)
		interval = story_data.size;
	if (index == 0)
		m_fPositionY = visibleSize.height * 0.9 - storylineHalfHeight;
	else
		m_fPositionY = m_fPositionY - interval - storylineHalfHeight;

	auto clippingNode = ClippingNode::create(stencil);
	clippingNode->setInverted(true);
	clippingNode->addChild(storyline);
	clippingNode->setPosition(20 + storylineHalfWidth, m_fPositionY);
	this->addChild(clippingNode);

	m_fPositionY -= storylineHalfHeight;
}