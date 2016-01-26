#include "Player.h"

Player::Player(void): m_fSpeed(0), m_fJumpHeight(0), m_fJumpDuration(0)
{
}

Player::~Player(void)
{
}

Player* Player::create(const Color3B &color, float speed, float jumpTime)
{
	Player *pRet = new(std::nothrow) Player();
	pRet->init(color, speed, jumpTime);
	pRet->autorelease();
	return pRet;
}

bool Player::init(const Color3B &color, float speed, float jumpTime)
{  
    bool bRet = false;  
    do   
    {  
        CC_BREAK_IF(!Node::init());  

		// set attribute
		m_playerColor = Color4F(color);
		Size playerSize = Size(32, 32);
		m_fJumpHeight = playerSize.height * 3;
		m_fSpeed = speed;
		m_fJumpDuration = jumpTime;

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

bool Player::jump()
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
		return true;
	}
	return false;
}

void Player::die()
{
	Director::getInstance()->getTextureCache()->removeTextureForKey("playerImage");
	this->stopAllActions();
	this->unscheduleAllCallbacks();
	this->removeFromParent();
}