#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"



class Player : public cocos2d::Sprite
{
    

    

public:

    cocos2d::Sprite* image;
    float x;
    float y;
    cocos2d::Vec2 direction;
    float speed;
    float gravity;
    float jump_speed;
    std::string status;
    bool facing_right = true;
    bool on_ground = false;
    bool on_ceiling = false;
    bool on_left = false;
    bool on_right = false;
    
    void setPlayer(float x,float y, cocos2d::Scene* scene);

    //void getInput(Player* player, cocos2d::EventListenerKeyboard* eventListener, cocos2d::Scene* scene);


    //void right(float dt);
    ////method for moving left
    //void left(float dt);
    ////method for moving up
    //void up(float dt);
    ////method for moving down
    //void down(float dt);

    //void update(float dt);



 

};

#endif // __PLAYER_H__
