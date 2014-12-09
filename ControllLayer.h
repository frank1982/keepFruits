
#ifndef __KeepFruits__ControllLayer__
#define __KeepFruits__ControllLayer__
#include "cocos2d.h"
#include "GameScene.h"

using namespace cocos2d;

class ControllLayer:public Layer{

public:
    
    static Layer* createLayer();
    bool init();
    LabelTTF *_label1,*_label2;
    
private:
    
    void restart();
};





#endif /* defined(__CloudCity__ControllLayer__) */
