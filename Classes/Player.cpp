#include "Player.h"

Player::Player(void)
{
}

Player::~Player(void)
{
}

Player* Player::create(const Color3B &color, GameLevelData* levelData)
{
	Player *pRet = new(std::nothrow) Player();
	if (pRet && pRet->init(color, levelData))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

bool Player::init(const Color3B &color, GameLevelData* levelData)
{  
    bool bRet = false;  
    do   
    {  
        CC_BREAK_IF(!Node::init());  

		Size visibleSize = Director::getInstance()->getVisibleSize();

		// set attribute
		m_playerColor = Color4F(color);
		m_fSpeed = visibleSize.width / 5;
		Size playerSize = Size(32, 32);
		m_fJumpHeight = playerSize.height * 4;
		m_fJumpDuration = 0.8f;
		if (levelData)
		{
			m_fSpeed = levelData->getPlayerSpeed();
			m_fJumpDuration = levelData->getPlayerJumpDuration();
		}

		// draw player rect
		auto draw = DrawNode::create();
		RenderTexture *renderTexture = RenderTexture::create(playerSize.width, playerSize.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
		renderTexture->begin();
		draw->drawSolidRect(Vec2(0, 0), playerSize, m_playerColor);
		draw->visit();
		renderTexture->end();
		Director::getInstance()->getRenderer()->render();
		Director::getInstance()->getTextureCache()->addImage(renderTexture->newImage(), "playerImage");
		this->addChild(draw);
		this->setContentSize(playerSize);
		this->setAnchorPoint(Vec2(0.5f, 0.5f));

		// set physcis body
		PhysicsBody* body = PhysicsBody::createBox(playerSize);
		body->setGravityEnable(false);
		body->setCategoryBitmask(MASK_PLAYER);
		body->setCollisionBitmask(MASK_ENEMY);
		body->setContactTestBitmask(MASK_ENEMY);
		this->setPhysicsBody(body);

		// stop previous actions
		this->stopAllActions();

        bRet = true;
    } while (0);
    
    return bRet;
}

void Player::jump()
{
	auto action = this->getActionByTag(ACTIONTAG_JUMP);
	if (!action || action->isDone())
	{
		// make jump action
		auto actionJump = Spawn::createWithTwoActions(
			JumpBy::create(m_fJumpDuration, Vec2(0, 0), m_fJumpHeight, 1),
			RotateBy::create(m_fJumpDuration, 180)
			);
		actionJump->setTag(ACTIONTAG_JUMP);
		this->runAction(actionJump);
	}
}

void Player::die()
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("playerImage");
	this->stopAllActions();
	this->unscheduleAllCallbacks();
	this->removeFromParent();
}