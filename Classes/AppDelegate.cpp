#include "AppDelegate.h"
#include "CommonHeader.h"
#include "MainMenuScene.h"
#include "GameMediator.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size smallResolutionSize = cocos2d::Size(320, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(768, 1024);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1536, 2048);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director

#ifdef DEBUG_MODE
	designResolutionSize = Size(designResolutionSize.width * 2, designResolutionSize.height);
	smallResolutionSize = Size(smallResolutionSize.width * 2, smallResolutionSize.height);
	mediumResolutionSize = Size(mediumResolutionSize.width * 2, mediumResolutionSize.height);
	largeResolutionSize = Size(largeResolutionSize.width * 2, largeResolutionSize.height);
#endif
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("EndlessJump", Rect(0, 0, designResolutionSize.width, designResolutionSize.height));

#else
        glview = GLViewImpl::create("EndlessJump");
#endif
        director->setOpenGLView(glview);
    }

#ifdef DEBUG_MODE
    // turn on display FPS
    director->setDisplayStats(true);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
    Size frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	///////////////////////////////////////////////////////// My Code
	//glEnable(GL_LINE_SMOOTH);
	FileUtils::getInstance()->addSearchPath("res");
	// set random seed
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned int time = ((unsigned int)tv.tv_sec) * 1000 + tv.tv_usec / 1000;
	srand(time);

	GameMediator::getInstance();

    // run
    director->runWithScene(MainMenuScene::createScene());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
