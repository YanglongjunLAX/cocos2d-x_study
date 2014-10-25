#include "ActionSprite.h"

USING_NS_CC;

ActionSprite::ActionSprite(void)
{
	this->_actionState=kActionStateNone;
}


ActionSprite::~ActionSprite(void)
{
}

void ActionSprite::idle()
{
	if(this->_actionState!=kActionStateIdle){
		this->stopAllActions();
		this->runAction(this->_idleAction);

		this->_actionState=kActionStateIdle;
		this->_velocity=CCPointZero;
	}
}