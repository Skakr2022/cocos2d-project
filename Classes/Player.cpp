#include "Player.h"



USING_NS_CC;



void Player::setPlayer(float x , float y , cocos2d::Scene* scene)
{
    image = Sprite::create("player.png");
    image->setAnchorPoint(Vec2(0,0));
    image->setPosition(x, y);
    scene->addChild(image);
    direction = Vec2(0,0);
    status = "idle";
    speed = 2;
    gravity = -0.8;
    jump_speed = 16;

}







