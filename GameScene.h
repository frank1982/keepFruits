#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "Bird.h"
#include "cocos2d.h"
using namespace cocos2d;

class GameScene : public cocos2d::Layer
{
public:

    static Scene* createScene();
    virtual bool init();  
    CREATE_FUNC(GameScene);
    
private:
    
    //void loadTexture();
    void loadHero();
    void loadBirds(float dt);
    void loadTree();
    void loadTextureCallback(Ref *pSender);
    void loadBowl();
    void shootArrow();
    void setPhyWorld(PhysicsWorld* world){m_world = world;}
    void debugCallback(Ref *pSender);
    void update(float dt);
    void eatApple();
    bool onContactBegin(const PhysicsContact& contact);
    Sprite *_runner,*_hero,*_tree,*_arrow,*_bowl;
    Array *_birdArray;
    Array *_arrowArray;
    
    int _loadProcess=0;
    PhysicsWorld* m_world;
    
    Point _pointBowl,_pointTouch;
    float _angle0=45;
    float _arrowSpeed=800;
    float arrow_scale=0.05;
    float bowl_scale=0.3;
    float TREE_SCALE;
    float FRUIT_SCALE=0.1;
    
    int SCORE=0;
    int FRUITS_NUM=3;
    
    LabelTTF *_scoreLabel,*_fruitNum;
    long _curTouchTime=0,_preTouchTime=0;
    
};

#endif // __HELLOWORLD_SCENE_H__
