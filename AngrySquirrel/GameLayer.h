#pragma once
#include "d:\cocos2d-2.0-x-2.0.4\cocos2dx\layers_scenes_transitions_nodes\cclayer.h"
#include "cocos2d.h"
#include "Hero.h"

class GameLayer :
	public cocos2d::CCLayer
{
public:
	GameLayer(void);
	~GameLayer(void);
	virtual bool init();

	void initTiledmap();

	CREATE_FUNC(GameLayer); 

	cocos2d::CCSpriteBatchNode* _actors;

	Hero *_hero;

	void initHero();
};

