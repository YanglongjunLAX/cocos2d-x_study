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

	//¿ÕÏÐ¶¯×÷
	CCArray *idleFrames=CCArray::create();
	idleFrames->retain();

	for(i=0;i<6;i++){
		CCSpriteFrame* frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("hero_idle_%02d.png",i)->getCString());
		idleFrames->addObject(frame);
	}
	CCAnimation* idleAnimation=CCAnimation::createWithSpriteFrames(idleFrames,0.3f);
	this->_idleAction=CCRepeatForever::create(CCAnimate::create(idleAnimation));


	this->_centerToBottom=39.0f;
	this->_centerToSides=29.0f;
	this->_hitPoints=100;
	this->_damage=20;
	this->_walkSpeed=80;

	return true;
}
