

#include "HelloScene.h"

using namespace cocos2d;

Scene* HelloScene::createScene(){
    
    auto scene=Scene::create();
    auto layer=HelloScene::create();
    scene->addChild(layer);

    return scene;
}

//
bool HelloScene::init(){

    Size visibleSize=Director::getInstance()->getVisibleSize();
    auto layer=LayerColor::create(Color4B::WHITE);
    addChild(layer,0);
    
    //load resource;
    _runner=Sprite::create("Runner.png");
    _runner->setTag(01);
    _runner->setPosition(_runner->getContentSize().width,_runner->getContentSize().height);
    addChild(_runner,1);
    
    std::string res[11]={"Arrow.png","Hero.png","Fruit.png","Tree.png","Blue.png","Coffee.png","Green.png",
        "Grey.png","Orange.png","Red.png","Yellow.png"};
    for(int i=0;i<11;i++){
        
        Director::getInstance()->getTextureCache()->addImageAsync(res[i], CC_CALLBACK_1(HelloScene::loadTextureCallback, this));
    }
 
    return true;
}

void HelloScene::loadTextureCallback(Ref *pSender){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _loadProcess+=9;
    auto fullDistance=visibleSize.width-_runner->getContentSize().width*2;
    auto moveDistance=fullDistance/11;
    
    _runner->setPosition(_runner->getPosition()+Point(moveDistance,0));
    
    if(_loadProcess>=99){
        
        _runner->getParent()->removeChildByTag(01);
        auto labelItem=MenuItemLabel::create(LabelTTF::create("开始游戏吧", "Arial", 40), CC_CALLBACK_1(HelloScene::playCallback, this));
        labelItem->setPosition(visibleSize.width/2,visibleSize.height/2);
        labelItem->setColor(Color3B::RED);
        auto menu=Menu::create();
        menu->addChild(labelItem);
        menu->setPosition(Point::ZERO);
        addChild(menu,1);
        
        
        //add tree;
        auto treeSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Tree.png"));
        float TREE_SCALE=(visibleSize.height-30)/treeSP->getContentSize().height;
        treeSP->setScale(TREE_SCALE);
        treeSP->setPosition(visibleSize.width-treeSP->getContentSize().width/2*TREE_SCALE,treeSP->getContentSize().height/2*TREE_SCALE);
        addChild(treeSP,2);
        
        //add birds;
        float Bird_SCALE=0.1;
        auto birdBlueSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Blue.png"));
        birdBlueSP->setScale(Bird_SCALE);
        birdBlueSP->setPosition(birdBlueSP->getContentSize().width/2*Bird_SCALE,visibleSize.height/2);
        addChild(birdBlueSP,2);
        
        auto birdYellowSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Yellow.png"));
        birdYellowSP->setScale(Bird_SCALE);
        birdYellowSP->setPosition(birdBlueSP->getPosition()+Point(75,75));
        addChild(birdYellowSP,2);
        
        auto birdRedSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Red.png"));
        birdRedSP->setScale(Bird_SCALE);
        birdRedSP->setPosition(birdYellowSP->getPosition()+Point(75,75));
        addChild(birdRedSP,2);
        
        auto birdGreySP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Grey.png"));
        birdGreySP->setScale(Bird_SCALE);
        birdGreySP->setPosition(birdRedSP->getPosition()+Point(75,75));
        addChild(birdGreySP,2);
        
        auto birdOrangeSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Orange.png"));
        birdOrangeSP->setScale(Bird_SCALE);
        birdOrangeSP->setPosition(birdGreySP->getPosition()+Point(75,-75));
        addChild(birdOrangeSP,2);
        
        auto birdCoffeeSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Coffee.png"));
        birdCoffeeSP->setScale(Bird_SCALE);
        birdCoffeeSP->setPosition(birdOrangeSP->getPosition()+Point(-75,-75));
        addChild(birdCoffeeSP,2);
        
        auto birdGreenSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Green.png"));
        birdGreenSP->setScale(Bird_SCALE);
        birdGreenSP->setPosition(birdOrangeSP->getPosition()+Point(75,75));
        addChild(birdGreenSP,2);
        
        //add fruits;
        float FRUIT_SCALE=0.1;
        auto fruitSP1=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Fruit.png"));
        fruitSP1->setScale(FRUIT_SCALE);
        fruitSP1->setPosition(treeSP->getPosition()+Point(0,200));
        addChild(fruitSP1,3);
        
        auto fruitSP2=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Fruit.png"));
        fruitSP2->setScale(FRUIT_SCALE);
        fruitSP2->setPosition(fruitSP1->getPosition()+Point(-30,-30));
        addChild(fruitSP2,3);
        
        auto fruitSP3=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Fruit.png"));
        fruitSP3->setScale(FRUIT_SCALE);
        fruitSP3->setPosition(fruitSP1->getPosition()+Point(30,-30));
        addChild(fruitSP3,3);
    }
    
    
};
void HelloScene::playCallback(Ref* pSender){
    
    auto scene=GameScene::createScene();
    Director::getInstance()->replaceScene(scene);
    
}


/*
void HelloScene::playCallback(Ref* pSender){
    
    _runner=Sprite::create("Runner.png");
    _runner->setTag(01);
    _runner->setPosition(_runner->getContentSize().width,_runner->getContentSize().height);
    addChild(_runner,1);

    std::string res[11]={"Arrow.png","Hero.png","Fruit.png","Tree.png","Blue.png","Coffee.png","Green.png",
        "Grey.png","Orange.png","Red.png","Yellow.png"};
    for(int i=0;i<11;i++){
        
        Director::getInstance()->getTextureCache()->addImageAsync(res[i], CC_CALLBACK_1(HelloScene::loadTextureCallback, this));
    }
    
}


void HelloScene::loadTextureCallback(Ref *pSender){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    _loadProcess+=9;
    auto fullDistance=visibleSize.width-_runner->getContentSize().width*2;
    auto moveDistance=fullDistance/11;
    
    _runner->setPosition(_runner->getPosition()+Point(moveDistance,0));
    
    if(_loadProcess>=99){
        
        _runner->getParent()->removeChildByTag(01);
        auto scene=GameScene::createScene();
        Director::getInstance()->replaceScene(scene);

    }
    
    
};
*/

