#include "HudLayer.h"

USING_NS_CC;

HudLayer::HudLayer(void)
{
	this->_dPad=NULL;
}


HudLayer::~HudLayer(void)
{
}

bool HudLayer::init()
{
	if(!CCLayer::init()){
		return false;
	}

	this->_dPad=SimpleDPad::dPadWithFile(CCString::create("pd_dpad.png"),64);
	this->_dPad->setPosition(ccp(64,64));
	this->_dPad->setOpacity(100); //����͸����
	this->addChild(_dPad);

	return true;
}