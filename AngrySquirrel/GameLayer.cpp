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

	return true;
}

void GameLayer::initTiledmap()
{
	CCTMXTiledMap* _tilemp=CCTMXTiledMap::create("pd_tilemap.tmx");

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