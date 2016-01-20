#ifndef __CREDIT_SCENE_H__
#define __CREDIT_SCENE_H__

#include "CommonHeader.h"

class CreditScene : public Layer
{
public:
    static Scene* createScene();

    virtual bool init();
   
private:
	void buttonCallback_MainMenu(Ref* pSender);
};

#endif // __CREDIT_SCENE_H__
