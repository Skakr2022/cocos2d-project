/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include <iostream>
#include <Levl2.h>

using namespace std; 


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    



    
    for (int i = 0 ; i < 10 ; i++) {
        for (int j = 0; j < 28 ; j++) {
            //if the array has X draw ground
            if (level_map1[i].at(j) == 'X') {
                drawTile(j * 64, 576 - (i * 64));
            }//if the array has P draw Player
            if (level_map1[i].at(j) == 'P')
            {
               player.setPlayer(j * 64 , 576 - (i * 64)   ,this);
                
            }//if the array has O draw Obstacle
            if (level_map1[i].at(j) == 'O')
            {
                drawTile1(j * 64, 576 - (i * 64), 64);

            }//if the array has E draw Ending flag
            if (level_map1[i].at(j) == 'E')
            {
                drawTileEnd(j * 64, 576 - (i * 64));

            }
        };
    };


    

   auto eventListener = EventListenerKeyboard::create();
   //event listener of keyboard
   eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
       
       //if we press left key
       if(keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
       {
           //schedule right method 
           this->schedule(SEL_SCHEDULE(&HelloWorld::right),0.01);
           player.direction.x = -1;
           player.facing_right = false;
       }//if we press right key
       else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
       {
           //schedule right method 
           this->schedule(SEL_SCHEDULE(&HelloWorld::right),0.01);
           player.direction.x = 1;
           player.facing_right = true;
       }
       else
       {
           player.direction.x = 0;
       }
       //if we press right key
       if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW && player.on_ground)
       {

           //call the jump method
           jump();
       }
       
   };
   //call this when we release the key of keyboard
   eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {


       if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
       {
           //unschedule the right method
           this->unschedule(SEL_SCHEDULE(&HelloWorld::right));
           player.direction.x = -1;
           player.facing_right = false;
       }
       else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
       {
           this->unschedule(SEL_SCHEDULE(&HelloWorld::right));
           player.direction.x = 1;
           player.facing_right = true;
       }
       else
       {
           player.direction.x = 0;
       }

   };

   this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);


   //update the scene 
   this->scheduleUpdate();





    return true;
}
//method to create the ground
void HelloWorld::drawTile(float x, float y)
{
    hero = Sprite::create("ground.png");
    
    hero->setAnchorPoint(Vec2(0,0));
    hero->setPosition(x, y);
   
    this->addChild(hero, 2);
    map.push_back(hero);

}
//method to create a ending flag
void HelloWorld::drawTileEnd(float x, float y)
{
    END = Sprite::create("endFlag.png");

    END->setAnchorPoint(Vec2(0, 0));
    END->setPosition(x, y);
    this->addChild(END, 2);

}

//method to draw a obstacle
void HelloWorld::drawTile1(float x, float y, float size)
{
    obs = Sprite::create("ground.png");

    obs->setAnchorPoint(Vec2(0, 0));
    //set the position
    obs->setPosition(x, y);
    //add to the scene(this)
    this->addChild(obs, 2);
    //push every obstacle created to a map (vector)
    mapObs.push_back(obs);

}

//method to move elements of the scene with the player
void HelloWorld::updateTile(float x)
{
    for (auto sprite : map) {
        Vec2 position = sprite->getPosition();
        sprite->setPositionX(position.x + x);
    }

    for (auto sprite : mapObs) {
        Vec2 position = sprite->getPosition();
        sprite->setPositionX(position.x + x);
    }
    for (auto sprite : mapKill) {
        Vec2 position = sprite->getPosition();
        sprite->setPositionX(position.x + x);
    }
    Vec2 position = END->getPosition();
    END->setPositionX(position.x + x);
    
}


//method to move the scene with the player
void HelloWorld::scroll()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto player_x = player.image->getPositionX();
    auto direction_x = player.direction.x;

    if (player_x < visibleSize.width /4 && direction_x < 0)
    {
        this->world_shift = 8;
        player.speed = 0;
    }
    else if (player_x > visibleSize.width - (visibleSize.width / 4) && direction_x > 0)
    {
        this->world_shift = -8;
        player.speed = 0;
    }
    else
    {
        this->world_shift = 0;
        player.speed = 8;
    }

}

//method to apply a gravity to player
void HelloWorld::apply_gravity()
{
    player.direction.y += player.gravity;
    float pos = player.image->getPositionY();
    player.image->setPositionY(pos + player.direction.y);
}
//method of jump player
void HelloWorld::jump()
{
    player.direction.y = player.jump_speed;
}

//method update repeat this method multipe time in seconde usually 60fps
void HelloWorld::update(float dt) {
    scroll();
    updateTile(world_shift);

    
    CCLOG("(%f,%f)", player.image->getPositionX(), player.image->getPositionY());
    CCLOG("(x : %f, y : %f)", player.direction.x, player.direction.y);

    horizontal_mouvement_collision();
    vertical_mouvement_collision();
    

}





void HelloWorld::horizontal_mouvement_collision()
{
    for (auto sprite : mapObs) {
        //creating rectangle of player and the obstacle
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if player intersect with a obstacle
        if (rect1.intersectsRect(rect2))
        {
            //if the player move to the left and the obstacle in the same height of player let the player stop 
            if (player.direction.x < 0 && player.image->getPositionY() == sprite->getPositionY() ) {
                player.image->setPositionX(sprite->getPositionX() +65);

            }//if the player move to the right and the obstacle in the same height of player let the player stop 
            else if (player.direction.x > 0 && player.image->getPositionY() == sprite->getPositionY() ) {
                player.image->setPositionX(sprite->getPositionX() -33);
            }

        }
    }
}

void HelloWorld::vertical_mouvement_collision()
{
    apply_gravity();
    for (auto sprite : map) {
        //creating rectangle of player and the obstacle
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if player intersect with the obstacle
        if (rect1.intersectsRect(rect2))
        {
            //and if the player is droping
            if (player.direction.y < 0) {
                //let the player above the ground
                player.image->setPositionY(sprite->getPositionY() + 64);
                player.direction.y = 0;
                player.on_ground = true;
            } 
        }
    }

    for (auto sprite : mapObs) {
        //creating rectangle of player and the obstacle
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if player intersect with the obstacle
        if (rect1.intersectsRect(rect2))
        {
            //and if the player is droping
            if (player.direction.y < 0) {
                //let the player above the ground
                player.image->setPositionY(sprite->getPositionY() + 64);
                player.direction.y = 0;
                player.on_ground = true;
            }
        }
    };

    //if player touch the flag move to next level
    Rect rect1 = player.image->getBoundingBox();
    Rect rect2 = END->getBoundingBox();
    //if ball intersect with a square
    if (rect1.intersectsRect(rect2))
    {
        auto scene = Levl2::createScene();
        Director::getInstance()->replaceScene(scene);
    }
}

//method of moving if x>0 move to left if x<0 move to right
void  HelloWorld::right(float dt) {
    auto player_xx = player.image->getPositionX();
    player.image->setPositionX(player_xx + player.direction.x * player.speed);

}






