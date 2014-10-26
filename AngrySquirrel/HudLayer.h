#pragma once
#include "d:\cocos2d-2.0-x-2.0.4\cocos2dx\layers_scenes_transitions_nodes\cclayer.h"
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
};

