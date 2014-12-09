

#include "Bird.h"

using namespace cocos2d;



Sprite* Bird::createBird(enum birdType mBirdType){
    
    std::string str="";
    switch(mBirdType){
        
        case Blue:
            str="Blue.png";
            CCLOG("load Blue.png");
            break;
            
        case Yellow:
            str="Yellow.png";
            break;

        case Red:
            str="Red.png";
            break;
            
        case Green:
            str="Green.png";
            break;
            
        case Coffee:
            str="Coffee.png";
            break;
            
        case Grey:
            str="Grey.png";
            break;
            
        case Orange:
            str="Orange.png";
            CCLOG("load Orange.png");
            break;

    }
    auto birdSprite=Sprite::createWithTexture(Director::getInstance()->getTextureCache()->addImage(str));
    birdSprite->setScale(0.08);
    birdSprite->setTag(2);
    auto body=PhysicsBody::createBox(birdSprite->getContentSize()*0.08);
    body->setGravityEnable(false);
    body->setMass(0.0f);
    body->setCategoryBitmask(0x00000001);
    body->setContactTestBitmask(0x00000010);
    body->setCollisionBitmask(0x00000010);
    birdSprite->setPhysicsBody(body);
    
    return birdSprite;
    
}


