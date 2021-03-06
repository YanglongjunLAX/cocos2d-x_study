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

void ActionSprite::attack()
{
	if(this->_actionState==kActionStateIdle || this->_actionState==kActionStateAttack || this->_actionState==kActionStateWalk){
		this->stopAllActions();
		this->_actionState=kActionStateAttack;
		this->runAction(this->_attackAction);
		
	}
}

void
ActionSprite::walkWithDirection(cocos2d::CCPoint direction)
{
	if(this->_actionState==kActionStateIdle){
		this->stopAllActions();
		this->runAction(this->_walkAction);
		this->_actionState=kActionStateWalk;
	}

	if(this->_actionState==kActionStateWalk)
    {
		this->_velocity=ccp(direction.x*this->_walkSpeed,direction.y*this->_walkSpeed);

		if(this->_velocity.x>=0){
			this->setScaleX(1.0);
		}else
        {
			this->setScaleX(-1.0);
        }
    }
}

void ActionSprite::updateDesiredPosition(float dt)
{
	if(this->_actionState==kActionStateWalk)
    {
		this->_desiredPosition=ccpAdd(this->getPosition(),ccpMult(this->_velocity,dt));
	}
}