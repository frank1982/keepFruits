

#ifndef __KeepFruits__Bird__
#define __KeepFruits__Bird__

#include "cocos2d.h"
using namespace cocos2d;

enum birdType{
    
    Blue,Coffee,Green,Grey,Orange,Red,Yellow
};

class Bird:public Sprite{
    
public:
    
    
    static Sprite* createBird(enum birdType mBirdType);
   
private:
    
    float _blood=100;
    float _speed=120;
    float _score;
    const float _birdScale=0.08;
    
    
};





#endif /* defined(__KeepFruits__Bird__) */
