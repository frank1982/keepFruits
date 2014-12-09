#include "GameScene.h"
#include "Math.h"
#include "ControllLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{

    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDraw(true); //此句仅3.0 alpha0 有效
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music/bg.mp3");
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music/bg.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("shoot/shoot.mp3");
    this->scheduleUpdate();
    this->schedule(schedule_selector(GameScene::loadBirds), 2);
    
    _arrowArray=Array::create();
    _arrowArray->retain();
    _birdArray=Array::create();
    _birdArray->retain();
    
    auto layer=LayerColor::create(Color4B::WHITE);
    addChild(layer,0);
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /*
    auto debugItem=MenuItemSprite::create(Sprite::create("CloseSelected.png"), Sprite::create("CloseSelected.png"), CC_CALLBACK_1(GameScene::debugCallback, this));
    debugItem->setPosition(debugItem->getContentSize().width,visibleSize.height-debugItem->getContentSize().height);
    auto menu=Menu::create();
    menu->addChild(debugItem);
    menu->setPosition(Point::ZERO);
    addChild(menu,1);
    */
    
    /*
    auto edgeSp = Sprite::create();
    //auto body = PhysicsBody::createEdgeBox(visibleSize,3); //此句仅3.0 alpha0 有效
    auto body = PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT,3);
    edgeSp->setPosition(Point(visibleSize.width/2,visibleSize.height/2));
    edgeSp->setPhysicsBody(body);
    this->addChild(edgeSp);
    edgeSp->setTag(0);
    */
     
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    auto contactListener=EventListenerPhysicsContact::create();
    //设置监听器的碰撞开始函数
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
    //添加到事件分发器中
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    loadTree();
    loadBowl();
 
    
    //show score;
    auto str=String::createWithFormat("SCORE: %d",SCORE);
    _scoreLabel=LabelTTF::create(str->getCString(), "Arial", 40);
    _scoreLabel->setPosition(visibleSize.width/2,visibleSize.height-_scoreLabel->getContentSize().height);
    _scoreLabel->setColor(Color3B::GREEN);
    addChild(_scoreLabel,2);
    
    
    auto fruit=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Fruit.png"));
    fruit->setScale(FRUIT_SCALE);
    fruit->setPosition(_scoreLabel->getPosition()+Point(_scoreLabel->getContentSize().width/2+fruit->getContentSize().width/2,0));
    addChild(fruit,2);
    
    auto str2=String::createWithFormat("X %d",FRUITS_NUM);
    _fruitNum=LabelTTF::create(str2->getCString(), "Arial", 40);
    _fruitNum->setPosition(fruit->getPosition()+Point(50,0));
    _fruitNum->setColor(Color3B::RED);
    addChild(_fruitNum,2);

    
    
    
   
    listener->onTouchBegan=[&](Touch* touch, Event* event){
        
        struct  timeval tv;
        gettimeofday(&tv,NULL);
        _curTouchTime=tv.tv_sec * 1000 + tv.tv_usec / 1000;
        
        if(_curTouchTime-_preTouchTime>=500){
            
            _pointTouch=touch->getLocation();
            Point shootVector = _pointTouch-_bowl->getPosition();
            Point normalizedVector = ccpNormalize(shootVector) ;
            float radians = atan2(normalizedVector.y, - normalizedVector.x);
            float degree = CC_RADIANS_TO_DEGREES(radians);
            _bowl->setRotation(degree);
            _preTouchTime=_curTouchTime;
            return true;
            
        }else{
            
            CCLOG("too much touch");
            return false;
        }
        
        
        
    };
    listener->onTouchMoved=[&](Touch* touch, Event* event){};
    listener->onTouchEnded=[&](Touch* touch, Event* event){
        
        //shoot the arrow
        SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
        SimpleAudioEngine::sharedEngine()->playEffect("music/shoot.mp3");
        Point shootVector = _pointTouch-_bowl->getPosition();
        Point normalizedVector = ccpNormalize(shootVector) ;
        CCLog("x speed is: %f, y speed is: %f",shootVector.x,shootVector.y);
        _arrow=Sprite::create("Arrow.png");
        _arrow->setTag(1);
        addChild(_arrow);
        
        
        _arrow->setPosition(_bowl->getPosition());
        _arrow->setScale(arrow_scale);
        _arrow->setRotation(_bowl->getRotation());
        auto body=PhysicsBody::createBox(_arrow->getContentSize()*arrow_scale);
        body->setMass(1.0f);
        body->setCategoryBitmask(0x00000010);
        body->setContactTestBitmask(0x00000001);
        body->setCollisionBitmask(0x00000001);
        _arrow->setPhysicsBody(body);
        _arrow->getPhysicsBody()->setVelocity(Vect(normalizedVector.x*_arrowSpeed,normalizedVector.y*_arrowSpeed));
        _arrow->getPhysicsBody()->setGravityEnable(true);
        
        //_arrowArray=Array::create();
        _arrowArray->addObject(_arrow);
        CCLog("_arrowArray length is %zd",_arrowArray->count());
        
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    return true;
}




void GameScene::loadBirds(float dt){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    enum birdType type[7]={Blue,Yellow,Green,Grey,Red,Orange,Coffee};
    int index=(int)(CCRANDOM_0_1()*7);
    auto birdSp=Bird::createBird(type[index]);
    
    //random enter position;
    float PY=(float)(CCRANDOM_0_1()*visibleSize.height);
    birdSp->setPosition(0,PY);
    
    
    addChild(birdSp,1);
    _birdArray->addObject(birdSp);
    auto fruit=this->getChildByTag(21);
    
    //random speed;
    float Speed=(float)(CCRANDOM_0_1()*2.5+3);
    auto seq=Sequence::create(MoveTo::create(Speed, fruit->getPosition()), DelayTime::create(1.2),CallFunc::create(CC_CALLBACK_0(GameScene::eatApple, this)),
                              MoveTo::create(2, Point(visibleSize.width,fruit->getPosition().y)),NULL);
    birdSp->runAction(seq);
    CCLOG("bird number is %zd",_birdArray->count());

}
void GameScene::eatApple(){
    
    CCLOG("eat apple");
    FRUITS_NUM--;
    auto str2=String::createWithFormat("X %d",FRUITS_NUM)->getCString();
    _fruitNum->setString(str2);
    if(FRUITS_NUM==0){
        
        this->removeChildByTag(21);
        Director::getInstance()->pause();
        auto layer=ControllLayer::createLayer();
        this->addChild(layer,6);
       
    }else if (FRUITS_NUM==1){
        this->removeChildByTag(22);
    }else if (FRUITS_NUM==2){
        this->removeChildByTag(23);
    }
};

void GameScene::loadBowl(){
    

    Size visibleSize = Director::getInstance()->getVisibleSize();
    _bowl=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("bowl.png"));
    _bowl->setScale(bowl_scale);
    _bowl->setRotation(_angle0);
    auto tree=static_cast<Sprite*>(this->getChildByTag(6));
    auto hero=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Runner.png"));
    hero->setPosition(tree->getPosition().x-tree->getContentSize().width/2*TREE_SCALE-hero->getContentSize().width/2,hero->getContentSize().height/2);
    _bowl->setPosition(hero->getPosition().x-_bowl->getContentSize().width/2*bowl_scale-hero->getContentSize().width/2+10,hero->getPosition().y+_bowl->getContentSize().height*bowl_scale-50);
    
    addChild(hero,1);
    addChild(_bowl,1);
    
}
void GameScene::update(float dt){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto rectWin=CCRectMake(0, 0, visibleSize.width, visibleSize.height);

    for(int i=0;i<_arrowArray->count();i++){
        
        auto sp=dynamic_cast<Sprite *>(_arrowArray->getObjectAtIndex(i));
        
        //check if out of window;
        if(sp->getPosition().x<=0||sp->getPosition().y>=visibleSize.height||sp->getPosition().x>=visibleSize.width||sp->getPosition().y<=0){
            
            //sp->setVisible(false);
            sp->removeAllChildren();
            this->removeChild(sp);
            _arrowArray->removeObjectAtIndex(i);
            CCLog("_arrowArray length is %zd",_arrowArray->count());
        }
    }
    for(int j=0;j<_birdArray->count();j++){
        
        auto sp2=dynamic_cast<Sprite *>(_birdArray->getObjectAtIndex(j));
        
        //check if out of window;
        if(sp2->getPosition().x>=visibleSize.width||sp2->getPosition().y<=0){
            
            sp2->removeAllChildren();
            this->removeChild(sp2);
            _birdArray->removeObjectAtIndex(j);
            CCLog("_birdArray length is %zd",_birdArray->count());
        }
    }
    
}
bool GameScene::onContactBegin(const PhysicsContact& contact){
    
    CCLOG("get bound");
    auto cnt = const_cast<PhysicsContact*>(&contact);
    auto spA = cnt->getShapeA()->getBody()->getNode();
    auto spB = cnt->getShapeB()->getBody()->getNode();
    int tagA = spA->getTag();
    int tagB = spB->getTag();
    if(tagA == 2) {
        
        spA->getPhysicsBody()->setGravityEnable(true);
        spA->getPhysicsBody()->setVelocity(Point(0,-300));
        spB->removeAllChildren();
        this->removeChild(spB);
        _arrowArray->removeObject(spB);
        CCLog("_arrowArray length is %zd",_arrowArray->count());
        
    }else if(tagB == 2){
        
        spB->getPhysicsBody()->setGravityEnable(true);
        spB->getPhysicsBody()->setVelocity(Point(0,-300));
        this->removeChild(spA);
        _arrowArray->removeObject(spA);
        CCLog("_arrowArray length is %zd",_arrowArray->count());
        
    }
    SCORE+=5;
    auto str=String::createWithFormat("SCORE: %d",SCORE)->getCString();
    _scoreLabel->setString(str);

    return true;
}

void GameScene::loadTree(){
    
    //add tree;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto treeSP=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Tree.png"));
    TREE_SCALE=(visibleSize.height-30)/treeSP->getContentSize().height;
    treeSP->setScale(TREE_SCALE);
    treeSP->setPosition(visibleSize.width-treeSP->getContentSize().width/2*TREE_SCALE,treeSP->getContentSize().height/2*TREE_SCALE);
    treeSP->setTag(6);
    addChild(treeSP,1);
    
   
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
    
    
    auto seq1=Sequence::create(ScaleTo::create(1, 1.1*FRUIT_SCALE),ScaleTo::create(1, 0.9*FRUIT_SCALE), NULL);
    auto seq2=Sequence::create(ScaleTo::create(1, 1.1*FRUIT_SCALE),ScaleTo::create(1, 0.9*FRUIT_SCALE), NULL);
    auto seq3=Sequence::create(ScaleTo::create(1, 1.1*FRUIT_SCALE),ScaleTo::create(1, 0.9*FRUIT_SCALE), NULL);
    fruitSP1->runAction(RepeatForever::create(seq1));
    fruitSP2->runAction(RepeatForever::create(seq2));
    fruitSP3->runAction(RepeatForever::create(seq3));
    fruitSP1->setTag(21);
    fruitSP2->setTag(22);
    fruitSP3->setTag(23);
    
}

void GameScene::debugCallback(Ref *pSender){

    
    if(m_world->getDebugDrawMask() != PhysicsWorld::DEBUGDRAW_NONE)
    {
        m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);
    }
    else
    {
        m_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    }
}

