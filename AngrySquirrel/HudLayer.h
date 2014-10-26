#pragma once

#include "cocos2d.h"
#include "SimpleDPad.h"

class HudLayer : public cocos2d::CCLayer
{
public:
	HudLayer(void);
	~HudLayer(void);

	virtual bool init();

	SimpleDPad* _dPad;

	CREATE_FUNC(HudLayer);

	void update(float dt);
};

