

#ifndef __KeepFruits__HelloLayer__
#define __KeepFruits__HelloLayer__

#include "cocos2d.h"
#include "GameScene.h"
using namespace cocos2d;

class HelloScene:public Layer{
    
public:
    
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloScene);
    
    
private:
    
    void playCallback(Ref* pSender);
    void loadTexture();
    void loadTextureCallback(Ref *pSender);
    float _loadProcess=0;
    Sprite *_runner;
    
    
    //float TREE_SCALE=0.26;
};






#endif /* defined(__KeepFruits__HelloLayer__) */
