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

#include "Levl2.h"
#include <iostream>
#include <win.h>

using namespace std;


USING_NS_CC;

Scene* Levl2::createScene()
{
    return Levl2::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Levl2::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();






    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 28; j++) {
            if (level2[i].at(j) == 'X') {
                drawTile(j * 64, 576 - (i * 64), 64);
            };
            if (level2[i].at(j) == 'P')
            {
                player.setPlayer(j * 64, 576 - (i * 64), this);

            }
            if (level2[i].at(j) == 'O')
            {
                drawTile1(j * 64, 576 - (i * 64), 64);

            }
            if (level2[i].at(j) == 'D')
            {
                drawTileDrop(j * 64, 576 - (i * 64));

            }
            if (level2[i].at(j) == 'E')
            {
                drawTileEnd(j * 64, 576 - (i * 64), 64);

            }
        };
    };




    auto eventListener = EventListenerKeyboard::create();

    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {


        if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            this->schedule(SEL_SCHEDULE(&Levl2::right), 0.01);
            player.direction.x = -1;
            player.facing_right = false;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            this->schedule(SEL_SCHEDULE(&Levl2::right), 0.01);
            player.direction.x = 1;
            player.facing_right = true;
        }
        else
        {
            player.direction.x = 0;
        }

        if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW && player.on_ground)
        {
            jump();
        }

    };

    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {


        if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
        {
            this->unschedule(SEL_SCHEDULE(&Levl2::right));
            player.direction.x = -1;
            player.facing_right = false;
        }
        else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
        {
            this->unschedule(SEL_SCHEDULE(&Levl2::right));
            player.direction.x = 1;
            player.facing_right = true;
        }
        else
        {
            player.direction.x = 0;
        }

    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);

    this->scheduleUpdate();





    return true;
}

void Levl2::drawTile(float x, float y, float size)
{
    hero = Sprite::create("ground.png");

    hero->setAnchorPoint(Vec2(0, 0));
    hero->setPosition(x, y);

    this->addChild(hero, 2);
    map.push_back(hero);




}

void Levl2::drawTileEnd(float x, float y, float size)
{
    END = Sprite::create("endFlag.png");

    END->setAnchorPoint(Vec2(0, 0));
    END->setPosition(x, y);
    this->addChild(END, 2);

}
//method to create a drop map
void Levl2::drawTileDrop(float x, float y)
{
    drop = Sprite::create("drop.png");

    drop->setAnchorPoint(Vec2(0, 0));
    drop->setPosition(x, y + 32);

    this->addChild(drop, 2);
    mapDrop.push_back(drop);


    kill = Sprite::create("fire.png");

    kill->setAnchorPoint(Vec2(0, 0));
    kill->setPosition(x, y);


    this->addChild(kill, 2);
    mapKill.push_back(kill);

}

void Levl2::drawTile1(float x, float y, float size)
{
    obs = Sprite::create("ground.png");

    obs->setAnchorPoint(Vec2(0, 0));
    obs->setPosition(x, y);


    this->addChild(obs, 2);
    mapObs.push_back(obs);

}


void Levl2::updateTile(float x)
{
    for (auto sprite : map) {
        Vec2 position = sprite->getPosition();
        sprite->setPositionX(position.x + x);
    }

    for (auto sprite : mapObs) {
        Vec2 position = sprite->getPosition();
        sprite->setPositionX(position.x + x);
    }
    for (auto sprite : mapDrop) {
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

void Levl2::scroll()
{
    cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
    cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto player_x = player.image->getPositionX();
    auto direction_x = player.direction.x;

    if (player_x < visibleSize.width / 4 && direction_x < 0)
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


void Levl2::apply_gravity()
{
    player.direction.y += player.gravity;
    float pos = player.image->getPositionY();
    player.image->setPositionY(pos + player.direction.y);


}

void Levl2::jump()
{
    player.direction.y = player.jump_speed;
}

void Levl2::update(float dt) {


    scroll();
    updateTile(world_shift);


    CCLOG("(%f,%f)", player.image->getPositionX(), player.image->getPositionY());
    CCLOG("(x : %f, y : %f)", player.direction.x, player.direction.y);

    horizontal_mouvement_collision();
    vertical_mouvement_collision();


}





void Levl2::horizontal_mouvement_collision()
{

    /*auto player_xx = player.image->getPositionX();
    player.image->setPositionX(player_xx + player.direction.x * player.speed );*/


    for (auto sprite : mapObs) {

        //creating rectangle of ball and the square
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if ball intersect with a square
        if (rect1.intersectsRect(rect2))
        {

            if (player.direction.x < 0 && player.image->getPositionY() == sprite->getPositionY()) {
                player.image->setPositionX(sprite->getPositionX() + 65);

            }
            else if (player.direction.x > 0 && player.image->getPositionY() == sprite->getPositionY()) {
                player.image->setPositionX(sprite->getPositionX() - 33);
            }

        }


    }


}

void Levl2::vertical_mouvement_collision()
{
    apply_gravity();
    for (auto sprite : map) {

        //creating rectangle of ball and the square
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if ball intersect with a square
        if (rect1.intersectsRect(rect2))
        {
            if (player.direction.y < 0) {
                player.image->setPositionY(sprite->getPositionY() + 64);
                player.direction.y = 0;
                player.on_ground = true;
            }

        }
    }

    for (auto sprite : mapObs) {

        //creating rectangle of ball and the square
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if ball intersect with a square
        if (rect1.intersectsRect(rect2))
        {
            if (player.direction.y < 0) {
                player.image->setPositionY(sprite->getPositionY() + 64);
                player.direction.y = 0;
                player.on_ground = true;
            }
        }
    }

    for (auto sprite : mapDrop) {

        //creating rectangle of ball and the square
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if ball intersect with a square
        if (rect1.intersectsRect(rect2))
        {
            DelayTime* delayAction = DelayTime::create(0.1);

            MoveBy* action = MoveBy::create(1, Vec2(0, -100));
            auto sequenceAction = Sequence::create(delayAction, action, NULL);
            sprite->runAction(sequenceAction);
            player.image->setPositionY(sprite->getPositionY() + 32);


        }
    }

    for (auto sprite : mapKill) {

        //creating rectangle of ball and the square
        Rect rect1 = player.image->getBoundingBox();
        Rect rect2 = sprite->getBoundingBox();
        //if ball intersect with a square
        if (rect1.intersectsRect(rect2))
        {
            auto scene = Levl2::createScene();
            Director::getInstance()->replaceScene(scene);


        }
    }

    Rect rect1 = player.image->getBoundingBox();
    Rect rect2 = END->getBoundingBox();
    //if ball intersect with a square
    if (rect1.intersectsRect(rect2))
    {
        auto scene = Win::createScene();
        Director::getInstance()->replaceScene(scene);


    }


}


void  Levl2::right(float dt) {
    /*float pos = player.image->getPositionX();
    player.image->setPositionX(pos += player.direction.x * 250 * dt);*/

    auto player_xx = player.image->getPositionX();
    player.image->setPositionX(player_xx + player.direction.x * player.speed);

}






