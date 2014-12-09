

#include "ControllLayer.h"
using namespace cocos2d;


Layer* ControllLayer::createLayer(){
    
    auto layer=new ControllLayer;
    if(layer&&layer->init()){
        
        return layer;
    }
};

bool ControllLayer::init(){
    
    Size visibleSize=Director::getInstance()->getVisibleSize();
    CCLayer *layer = CCLayerColor::create(ccc4(180, 180, 180, 255), 500, 400);
    layer->ignoreAnchorPointForPosition(false);
    layer->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    addChild(layer);
    
    _label1=LabelTTF::create("苹果被偷吃光啦", "Arial", 45);
    _label1->setColor(Color3B::RED);
    auto item1=MenuItemLabel::create(_label1);
    item1->setPosition(0,40);
    _label2=LabelTTF::create("再来一次吧", "Arial", 45);
    _label2->setColor(Color3B::GREEN);
    auto item2=MenuItemLabel::create(_label2,CC_CALLBACK_0(ControllLayer::restart,this));
    item2->setPosition(0,-40);
    
    auto menu=Menu::create(item1,item2,NULL);
    menu->setPosition(visibleSize.width/2, visibleSize.height/2);
    addChild(menu,1);

    return true;
};


void ControllLayer::restart(){
    
    Director::sharedDirector()->resume();
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
    
       
};



