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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <Player.h>

class HelloWorld : public cocos2d::Scene
{
public:

    std::string level_map1[10] = {
            "                            ",
            "                            ",
            "                            ",
            "                            ",
            "                            ",
            "O                          O",
            "O P                     E  O",
            "XXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            "XXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            "XXXXXXXXXXXXXXXXXXXXXXXXXXXX" };

    
    float current_x;

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void drawTile(float x , float y );

    void updateTile(float x);

    void drawTile1(float x, float y, float size);

    void drawTileEnd(float x, float y);

    cocos2d::Sprite* hero;

    cocos2d::Sprite* obs;

    cocos2d::Sprite* kill;

    cocos2d::Sprite* END;

    std::vector<cocos2d::Sprite*> mapKill;

    std::vector<cocos2d::Sprite*> mapObs;

    cocos2d::DelayTime* delayAction;

    void update(float up);

    Player player;

    float world_shift;

    std::vector<cocos2d::Sprite*> map;

    void horizontal_mouvement_collision();

    void vertical_mouvement_collision();

    void scroll();

    void apply_gravity();

    void jump();

    void  right(float dt);

    bool ismoving = false;
    bool uppressed = false;
    bool rightpressed = false;
    bool leftpressed = false;
    bool downpressed = false;


};

#endif // __HELLOWORLD_SCENE_H__
