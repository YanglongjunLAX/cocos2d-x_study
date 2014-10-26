//
//  HelloWorldScene.cpp
//  AngrySquirrel
//
//  Created by xue on 13-5-19.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32
#define FLOOR_HEIGHT    62.0f

enum {
    kTagParentNode = 1,
};

PhysicsSprite::PhysicsSprite()
: m_pBody(NULL)
{

}

void PhysicsSprite::setPhysicsBody(b2Body * body)
{
    m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void)
{
    return true;
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    b2Vec2 pos  = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;

    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_tAnchorPointInPoints.x;
        y += m_tAnchorPointInPoints.y;
    }

    // Make matrix
    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if( ! CCPoint::CCPointEqualToPoint(m_tAnchorPointInPoints, CCPointZero) ){
        x += c*-m_tAnchorPointInPoints.x + -s*-m_tAnchorPointInPoints.y;
        y += s*-m_tAnchorPointInPoints.x + c*-m_tAnchorPointInPoints.y;
    }

    // Rot, Translate Matrix
    m_tTransform = CCAffineTransformMake( c,  s,
        -s,    c,
        x,    y );

    return m_tTransform;
}

HelloWorld::HelloWorld()
{
    setTouchEnabled( true );
    setAccelerometerEnabled( true );

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    // init physics
    this->initPhysics();//初始化这个世界

    CCSprite *sprite = CCSprite::create("bg.png");  //背景图
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(0.0, 0.0));
    this->addChild(sprite, -1);
    
    sprite = CCSprite::create("catapult_base_2.png"); //投射器底部后面那块
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(181.0, FLOOR_HEIGHT));
    this->addChild(sprite, 0);
    
    sprite = CCSprite::create("squirrel_1.png");        //左边松鼠
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(11.0, FLOOR_HEIGHT));
    this->addChild(sprite, 0);
    
    sprite = CCSprite::create("catapult_base_1.png");   //投射器底部前面那块
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(181.0, FLOOR_HEIGHT));
    this->addChild(sprite, 9);
    
    sprite = CCSprite::create("squirrel_2.png");    //右边松鼠
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(240.0, FLOOR_HEIGHT));
    this->addChild(sprite, 9);
    
    sprite = CCSprite::create("fg.png");    //带冰的地面
    sprite->setAnchorPoint(CCPointZero);
    this->addChild(sprite, 10);
    
    CCSprite *arm = CCSprite::create("catapult_arm.png");//增加弹弓臂
    this->addChild(arm, 1);
    b2BodyDef armBodyDef;            //设置手臂物理定义
    armBodyDef.type = b2_dynamicBody;//设置为动态物体
    armBodyDef.linearDamping = 1;    //线性阻尼
    armBodyDef.angularDamping = 1;   //角阻抗
    armBodyDef.position.Set(230.0f/PTM_RATIO, (FLOOR_HEIGHT+91.0f)/PTM_RATIO);
    armBodyDef.userData = arm;
    m_armBody = world->CreateBody(&armBodyDef);//创建手臂
    
    b2PolygonShape armBox;  //定义手臂的多边形形状
    b2FixtureDef armBoxDef; //定义夹具
    armBoxDef.shape = &armBox;
    armBoxDef.density = 0.3F;//密度
    armBox.SetAsBox(11.0f/PTM_RATIO, 91.0f/PTM_RATIO);
    m_armFixture = m_armBody->CreateFixture(&armBoxDef);//创建夹具
    
    b2RevoluteJointDef armJointDef; //旋转关节定义
    armJointDef.Initialize(groundBody, m_armBody, b2Vec2(233.0f/PTM_RATIO, FLOOR_HEIGHT/PTM_RATIO));//将地面物体和手臂物体在一个点连接起来
    armJointDef.enableMotor = true;//马达
    armJointDef.enableLimit = true;//限制
    armJointDef.motorSpeed  = -10; //-1260;
    armJointDef.lowerAngle  = CC_DEGREES_TO_RADIANS(9); //最小角
    armJointDef.upperAngle  = CC_DEGREES_TO_RADIANS(75);//最大角
    armJointDef.maxMotorTorque = 700;//最大马达扭矩
    m_armJoint = (b2RevoluteJoint*)world->CreateJoint(&armJointDef);
    
    
    m_mouseJoint = NULL;
    m_releasingArm = false;
    m_currentBullet = 0;
    
    CCDelayTime *delayAction = CCDelayTime::create(0.2f);
    CCCallFunc *callSelectorAction = CCCallFunc::create(this, callfunc_selector(HelloWorld::resetGame));
    this->runAction(CCSequence::create(delayAction, callSelectorAction,NULL));
    
    //setPosition(ccp(-480, 0));
    
    scheduleUpdate();
    
    //监听器实现
    contactListener = new MyContactListener();
    world->SetContactListener(contactListener);
    
}

HelloWorld::~HelloWorld()
{
    delete world;
    world = NULL;
    
    //delete m_debugDraw;
}

void HelloWorld::initPhysics()
{

    CCSize s = CCDirector::sharedDirector()->getWinSize();

    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

//     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
//     world->SetDebugDraw(m_debugDraw);

    //uint32 flags = 0;
    //flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);


    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.

    b2EdgeShape groundBox;
    // bottom
    groundBox.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO), b2Vec2(s.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
    groundBody->CreateFixture(&groundBox, 0);
    
    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width*2.0f/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox, 0);
    
    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO));
    groundBody->CreateFixture(&groundBox, 0);
    
    // right
    /*
    groundBox.Set(b2Vec2(s.width*2.0f/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO)); 
    groundBody->CreateFixture(&groundBox, 0);
     */
}

void HelloWorld::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();

    kmGLPopMatrix();
}

void HelloWorld::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }    
    }
    
    
    if (m_releasingArm && m_bulletJoint != NULL)
    {
        // Check if the arm reached the end so we can return the limits
        if (m_armJoint->GetJointAngle() <= CC_DEGREES_TO_RADIANS(10))
        {
            m_releasingArm = false;
            
            // Destroy joint so the bullet will be free
            world->DestroyJoint(m_bulletJoint);
            m_bulletJoint = NULL;
            CCDelayTime *delayAction = CCDelayTime::create(5.0f);
            CCCallFunc *callSelectorAction = CCCallFunc::create(this, callfunc_selector(HelloWorld::resetBullet));
            this->runAction(CCSequence::create(delayAction, callSelectorAction, NULL));
        } 
    }
    
    if (m_bulletBody && m_bulletJoint == NULL)
    {
        b2Vec2 position = m_bulletBody->GetPosition();
        CCPoint myPosition = this->getPosition();
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        
        // Move the camera.
        if (position.x > screenSize.width / 2.0f / PTM_RATIO)
        {
            myPosition.x = -MIN(screenSize.width * 2.0f - screenSize.width, position.x * PTM_RATIO - screenSize.width / 2.0f);
            this->setPosition(myPosition);
        }
    }
    /*
    std::set<b2Body*>::iterator pos;
	for(pos = contactListener->contacts.begin(); pos != contactListener->contacts.end(); ++pos)
	{
		b2Body *body = *pos;
        
		for (vector<b2Body*>::iterator iter = targets.begin(); iter !=targets.end(); ++iter)
		{
			if (body == *iter)
			{
				iter = targets.erase(iter);
				break;
			}
		}
		for (vector<b2Body*>::iterator iter = enemies.begin(); iter !=enemies.end(); ++iter)
		{
			if ( body == *iter)
			{
				iter = enemies.erase(iter);
				break;
			}
		}
        
		CCNode *contactNode = (CCNode*)body->GetUserData();
		//
		CCPoint position = contactNode->getPosition();
        
		removeChild(contactNode, true);
		world->DestroyBody(body);
        
        
		//2-5-2
		CCParticleSun *explosion = CCParticleSun::node();
		explosion->retain();
		explosion->setTexture(CCTextureCache::sharedTextureCache()->addImage("fire.png"));
		explosion->initWithTotalParticles(200);
		explosion->setAutoRemoveOnFinish(true);
		explosion->setStartSizeVar(10.0f);
		explosion->setSpeed(70.0f);
		explosion->setAnchorPoint(ccp(0.5f, 0.5f));
		explosion->setPosition(position);
		explosion->setDuration(1.0f);
		addChild(explosion, 11);
		explosion->release();
        
	}
    */
    
	// remove everything from the set
	contactListener->contacts.clear();
 
     
}

    
void HelloWorld::createBullets(int count)
{
    m_currentBullet = 0;
    float pos = 62.0f;
    
    if (count > 0) {
        float delta = (count > 1)?((165.0f - 62.0f - 30.0f) / (count - 1)):0.0f;
        for (int i=0; i<count; i++, pos += delta)
        {
            // Create the bullet
            CCSprite *sprite = CCSprite::create("acorn.png");
            this->addChild(sprite, 1);
            
            b2BodyDef bulletBodyDef;
            bulletBodyDef.type = b2_dynamicBody;
            bulletBodyDef.bullet = true;//子弹效果
            bulletBodyDef.position.Set(pos/PTM_RATIO,(FLOOR_HEIGHT+15.0f)/PTM_RATIO);
            bulletBodyDef.userData = sprite;
            b2Body *bullet = world->CreateBody(&bulletBodyDef);
            bullet->SetActive(false);
            
            b2CircleShape circle;
            circle.m_radius = 15.0/PTM_RATIO;
            
            b2FixtureDef ballShapeDef;
            ballShapeDef.shape = &circle;
            ballShapeDef.density = 0.8f;
            ballShapeDef.restitution = 0.2f;
            ballShapeDef.friction = 0.99f;
            bullet->CreateFixture(&ballShapeDef);
            
            m_bullets.push_back(bullet);
        }
    }
}

bool HelloWorld::attachBullet()
{
    if (m_currentBullet < m_bullets.size())
    {
        m_bulletBody = (b2Body*)m_bullets.at(m_currentBullet++);
        m_bulletBody->SetTransform(b2Vec2(230.0f/PTM_RATIO, (155.0f+FLOOR_HEIGHT)/PTM_RATIO), 0.0f);
        m_bulletBody->SetActive(true);
        
        b2WeldJointDef weldJointDef;
        weldJointDef.Initialize(m_bulletBody, m_armBody, b2Vec2(230.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO));
        weldJointDef.collideConnected = false;
        
        m_bulletJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);
        return true;
    }
    
    return false;
}

void HelloWorld::resetBullet(){
    if (enemies.size() == 0)
    {
        //game over
        CCDelayTime *delayAction = CCDelayTime::create(2.0f);
        CCCallFunc *callSelectorAction = CCCallFunc::create(this, callfunc_selector(HelloWorld::resetGame));
        this->runAction(CCSequence::create(delayAction, callSelectorAction,  NULL));
    }
    else if (attachBullet())
    {
        CCAction *action = CCMoveTo::create(0.2f, CCPointZero);
        runAction(action);
    }
    else
    {
        //We can reset the whole scene here
        CCDelayTime *delayAction = CCDelayTime::create(2.0f);
        CCCallFunc *callSelectorAction = CCCallFunc::create(this, callfunc_selector(HelloWorld::resetGame));
        this->runAction(CCSequence::create(delayAction, callSelectorAction,  NULL));
    }
}

void HelloWorld::resetGame()
{
    if (m_bullets.size() != 0)
    {
        for (vector<b2Body*>::iterator bulletPointer = m_bullets.begin(); bulletPointer != m_bullets.end(); ++bulletPointer)
        {
            b2Body *bullet = (b2Body*)*bulletPointer;
            CCNode *node = (CCNode*)bullet->GetUserData();
            removeChild(node, true);
            world->DestroyBody(bullet);
            //      bulletPointer= m_bullets.erase(bulletPointer);
        }
        //  [bullets release];
        m_bullets.clear();
    }
    
    if (targets.size() !=0)
    {
        for (vector<b2Body*>::iterator targetPointer = targets.begin(); targetPointer != targets.end(); targetPointer++)
        {
            b2Body *target = (b2Body*)*targetPointer;
            CCNode *node = (CCNode*)target->GetUserData();
            removeChild(node, true);
            world->DestroyBody(target);
        }
        //  [bullets release];
        targets.clear();
        enemies.clear();
    }
    this->createBullets(4);
    this->attachBullet();
    this->createTarget();
    
    CCFiniteTimeAction *action1 = CCMoveTo::create(1.5f, ccp(-480.0f, 0.0f));
    CCCallFuncN *action2 = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::attachBullet));
    CCDelayTime *action3 = CCDelayTime::create(1.0f);
    CCFiniteTimeAction *action4 = CCMoveTo::create(1.5f, CCPointZero);
    runAction(CCSequence::create(action1, /*action2, */action3, action4, NULL));
}

void HelloWorld::createTarget(char      *imageName,
                              CCPoint   position,
                              float     rotation,
                              bool      isCircle,
                              bool      isStatic,
                              bool      isEnemy)
{
    CCSprite *sprite = CCSprite::create(imageName);
    this->addChild(sprite, 1);
    
    b2BodyDef bodyDef;
    bodyDef.type = isStatic ? b2_staticBody : b2_dynamicBody;
    bodyDef.position.Set((position.x+sprite->getContentSize().width/2.0f)/PTM_RATIO,
                         (position.y+sprite->getContentSize().height/2.0f)/PTM_RATIO);
    bodyDef.angle = CC_DEGREES_TO_RADIANS(rotation);
    bodyDef.userData = sprite;
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    b2FixtureDef boxDef;
    b2Fixture *fixtureTemp;
    
    if (isCircle){
        b2CircleShape circle;
        boxDef.shape = &circle;
        circle.m_radius = sprite->getContentSize().width/2.0f/PTM_RATIO;
        
        fixtureTemp = body->CreateFixture(&circle, 0.5f);
        targets.push_back(body);

    }
    else{
        b2PolygonShape box;
        boxDef.shape = &box;
        box.SetAsBox(sprite->getContentSize().width/2.0f/PTM_RATIO, sprite->getContentSize().height/2.0f/PTM_RATIO);
        body->CreateFixture(&box, 0.5f);
        //targets->push_back(body);
        targets.push_back(body);
    }
    
    if (isEnemy){
        fixtureTemp->SetUserData((void*)1);     //  boxDef.userData = (void*)1;
        enemies.push_back(body);
    }
}

void HelloWorld::createTarget(){
   
    char a[12] = "brick_1.png";
    char b[12] = "brick_2.png";
    char c[12] = "brick_3.png";
    char brick_platfor[19] = "brick_platform.png";
    char dog[13] = "head_dog.png";
    char cat[13] = "head_cat.png";
    createTarget(b, CCPointMake(675.0, FLOOR_HEIGHT), 0.0f, false, false, false);
    createTarget(a, CCPointMake(741.0, FLOOR_HEIGHT), 0.0f, false, false, false);
    createTarget(a, CCPointMake(741.0, FLOOR_HEIGHT+23.0), 0.0f, false, false, false);
    createTarget(c, CCPointMake(673.0, FLOOR_HEIGHT+46.0), 0.0f, false, false, false);
    createTarget(a, CCPointMake(707.0, FLOOR_HEIGHT+58.0), 0.0f, false, false, false);
    createTarget(a, CCPointMake(707.0, FLOOR_HEIGHT+81.0), 0.0f, false, false, false);
    
    createTarget(dog, CCPointMake(704, FLOOR_HEIGHT), 0.0f, true, false, true);
    createTarget(cat, CCPointMake(680.0, FLOOR_HEIGHT+58.0), 0.0f, true, false, true);
    createTarget(dog, CCPointMake(740.0, FLOOR_HEIGHT+58.0), 0.0f, true, false, true);
    
    // 2 bricks at the right of the first block
    createTarget(b, CCPointMake(770.0, FLOOR_HEIGHT), 0.0f, false, false, false);
    createTarget(b, CCPointMake(770.0, FLOOR_HEIGHT+46.0), 0.0f, false, false, false);
    
    // The dog between the blocks
    createTarget(dog, CCPointMake(830.0, FLOOR_HEIGHT), 0.0f, true, false, true);
    
    // Second block
    createTarget(brick_platfor, CCPointMake(839.0, FLOOR_HEIGHT), 0.0f, false, true, false);
    createTarget(b, CCPointMake(854.0, FLOOR_HEIGHT+28.0), 0.0f, false, false, false);
    createTarget(b, CCPointMake(854.0, FLOOR_HEIGHT+28.0+46.0), 0.0f, false, false, false);
    createTarget(cat, CCPointMake(881.0, FLOOR_HEIGHT+28.0), 0.0f, true, false, true);
    createTarget(b, CCPointMake(909.0, FLOOR_HEIGHT+28.0), 0.0f, false, false, false);
    createTarget(a, CCPointMake(909.0, FLOOR_HEIGHT+28.0+46.0), 0.0f, false, false, false);
    createTarget(a, CCPointMake(909.0, FLOOR_HEIGHT+28.0+46.0+23.0), 0.0f, false, false, false);
    createTarget(b, CCPointMake(882.0, FLOOR_HEIGHT+108.0), 90.0f, false, false, false);
    //CCDirector::sharedDirector()->pause();
    
}


void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* even)
{
    if (m_mouseJoint != NULL)
    {
        return;
    }
    
    CCTouch *touch = (CCTouch *)touches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
    if (locationWorld.x < m_armBody->GetWorldCenter().x + 150.0/PTM_RATIO)
    {
        b2MouseJointDef md;
        md.bodyA = groundBody;
        md.bodyB = m_armBody;
        md.target = locationWorld;//设置鼠标的单机点
        md.maxForce = 2000;
        
        m_mouseJoint = (b2MouseJoint *)world->CreateJoint(&md);
    }
    
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    if (m_mouseJoint == NULL)
    {
        return;
    }
    
    CCTouch *touch = (CCTouch *)touches->anyObject();
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    m_mouseJoint->SetTarget(locationWorld);
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    if (m_mouseJoint != NULL)
    {
        if (m_armJoint->GetJointAngle() >= CC_DEGREES_TO_RADIANS(5))
        {
            m_releasingArm = true;
        }
        world->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
        return;
    }
    
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new HelloWorld();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}
