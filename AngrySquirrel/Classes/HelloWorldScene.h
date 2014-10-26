//
//  HelloWorldScene.h
//  AngrySquirrel
//
//  Created by xue on 13-5-19.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "MyContactListener.h"

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
    HelloWorld();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    // adds a new sprite at a given coordinate
    void addNewSpriteAtPosition(cocos2d::CCPoint p);

    virtual void draw();
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void createBullets(int count);
    bool attachBullet();
    void resetBullet();
    void resetGame();
    void update(float dt);
    void createTarget(char      *imageName,
                      cocos2d::CCPoint   position,
                      float     rotation,
                      bool      isCircle,
                      bool      isStatic,
                      bool      isEnemy);
    void createTarget();
    
    
private:
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    
    b2Body* groundBody;
    b2Fixture *m_armFixture;
    b2Body *m_armBody;
    b2RevoluteJoint *m_armJoint;
    b2MouseJoint *m_mouseJoint;
    
    std::vector<b2Body *>m_bullets;
    int m_currentBullet;
    b2Body *m_bulletBody;
    b2WeldJoint *m_bulletJoint;
    bool m_releasingArm;
    
    std::vector<b2Body *> targets;
    std::vector<b2Body *> enemies;
    
    MyContactListener *contactListener; //监听器
};

#endif // __HELLO_WORLD_H__
