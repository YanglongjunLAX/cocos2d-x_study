#include "GameScene.h"

USING_NS_CC;

GameScene::GameScene(void)
{
	this->_gameLayer=NULL;
}


GameScene::~GameScene(void)
{
}

bool GameScene::init()
{
	if(!CCScene::init()){
		return false;
	}

	_gameLayer=GameLayer::create();

	this->addChild(_gameLayer);

	return true;
}
