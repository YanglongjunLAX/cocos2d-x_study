#include "Hero.h"

USING_NS_CC;

Hero::Hero(void)
{
}


Hero::~Hero(void)
{
}

bool Hero::init()
{
	if(!ActionSprite::initWithSpriteFrameName("hero_idle_00.png"))
	{
		return false;
	}
	int i;

	//空闲动作
	CCArray *idleFrames=CCArray::create();
	idleFrames->retain();

	for(i=0;i<6;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("hero_idle_%02d.png",i)->getCString());
		idleFrames->addObject(frame);
	}
	CCAnimation* idleAnimation=CCAnimation::createWithSpriteFrames(idleFrames,0.3f);
	this->_idleAction=CCRepeatForever::create(CCAnimate::create(idleAnimation));
	this->_idleAction->retain();


	//攻击动作
	CCArray* attackFrames=CCArray::create();
	attackFrames->retain();
	for(i=0;i<3;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("hero_attack_00_%02d.png",i)->getCString());
		attackFrames->addObject(frame);
	}
	CCAnimation *attackAnimation=CCAnimation::createWithSpriteFrames(attackFrames,0.1f);
	this->_attackAction=CCSequence::create(CCAnimate::create(attackAnimation),CCCallFunc::create(this,callfunc_selector(ActionSprite::idle)),NULL);
	this->_attackAction->retain();

	//行走动作
	CCArray* walkFrames=CCArray::createWithCapacity(8);
	for(i=0;i<8;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("hero_walk_%02d.png",i)->getCString());
		walkFrames->addObject(frame);
	}
	CCAnimation* walkAnimation=CCAnimation::createWithSpriteFrames(walkFrames,0.1f);
	this->_walkAction=CCRepeatForever::create(CCAnimate::create(walkAnimation));
	this->_walkAction->retain();

	this->_centerToBottom=39.0f;
	this->_centerToSides=29.0f;
	this->_hitPoints=100;
	this->_damage=20;
	this->_walkSpeed=80;

	return true;
}
