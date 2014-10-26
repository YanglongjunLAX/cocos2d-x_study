#include "GameLayer.h"

USING_NS_CC;

GameLayer::GameLayer(void)
{
	this->_hero=NULL;
}


GameLayer::~GameLayer(void)
{
}

bool GameLayer::init()
{
	if(!CCLayer::init()){
		return false;
	}

	this->initTiledmap();


	//加载精灵表单
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pd_sprites.plist");

	_actors=CCSpriteBatchNode::create("pd_sprites.pvr.ccz");
	_actors->getTexture()->setAliasTexParameters();

	this->addChild(_actors);

	this->initHero();

	
	this->setTouchEnabled(true);

	this->scheduleUpdate();

	return true;
}

void GameLayer::initTiledmap()
{
	_tilemp=CCTMXTiledMap::create("pd_tilemap.tmx");

	CCObject*  obj=NULL;
	CCARRAY_FOREACH(_tilemp->getChildren(),obj){
		CCTMXLayer* _child=(CCTMXLayer*)obj;

		_child->getTexture()->setAliasTexParameters();  //关闭抗锯齿
	}

	this->addChild(_tilemp);

}

void GameLayer::initHero()
{
	this->_hero=Hero::create();
	this->_actors->addChild(this->_hero);

	this->_hero->setPosition(ccp(this->_hero->_centerToSides,80));

	this->_hero->_desiredPosition=this->_hero->getPosition();

	this->_hero->idle();
}

void GameLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	this->_hero->attack();
}
void GameLayer::registerWithTouchDispatcher(void)
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);  //注册多点触摸
}

//执行 行走动作
void GameLayer::didChangeDirectorTo(SimpleDPad* simpleDPad,cocos2d::CCPoint direction)
{
	this->_hero->walkWithDirection(direction);
}

void GameLayer::isHoldingDirector(SimpleDPad* simpleDPad,cocos2d::CCPoint direction)
{
	this->_hero->walkWithDirection(direction);
}

//触摸方向键结束调用的函数
void GameLayer::simpleDPadTouchEnded(SimpleDPad* simpleDpad)
{
	if(this->_hero->_actionState==kActionStateWalk){
		this->_hero->idle();
	}
}

void GameLayer::update(float dt)
{
	this->_hero->updateDesiredPosition(dt);


	//设置英雄位置
	float posx=MIN(this->_tilemp->getMapSize().width*this->_tilemp->getTileSize().width-this->_hero->_centerToSides,MAX(this->_hero->_centerToSides,this->_hero->_desiredPosition.x));
	float posY=MIN(3*this->_tilemp->getTileSize().height+this->_hero->_centerToBottom,MAX(_hero->_centerToBottom,_hero->_desiredPosition.y));

	this->_hero->setPosition(ccp(posx,posY));

}