#pragma once
#include "d:\cocos2d-2.0-x-2.0.4\cocos2dx\layers_scenes_transitions_nodes\ccscene.h"
#include "cocos2d.h"
#include "GameLayer.h"

class GameScene : public cocos2d::CCScene
{
public:
	GameScene(void);
	~GameScene(void);

	virtual bool init();

	CREATE_FUNC(GameScene);  //create函数  是无参 返回值GameScene类型的static函数，

	GameLayer* _gameLayer;
};

