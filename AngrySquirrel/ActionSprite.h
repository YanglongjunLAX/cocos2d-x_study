#pragma once

#include "cocos2d.h"

typedef enum _ActionState{
	kActionStateNone=0,
	kActionStateIdle,
	kActionStateAttack,
	kActionStateWalk,
	kActionStateHurt,
	kActionDead
}ActionState;

class ActionSprite : public cocos2d::CCSprite
{
public:
	ActionSprite(void);
	~ActionSprite(void);

	//5�ֶ����ķ���
	void idle();  //����
	void attack();  //����
	void hurtWithDamage(float damage);  //����
	void dead(); //����
	void walkWithDirection(cocos2d::CCPoint direction);  //����

	void updateDesiredPosition(float dt);  //����desiredPosition
	//����
	cocos2d::CCAction* _idleAction;
	cocos2d::CCAction* _attackAction;
	cocos2d::CCAction* _walkAction;
	cocos2d::CCAction* _hurtAction;
	cocos2d::CCAction* _deadAction;

	ActionState _actionState;  //��ǰ������״̬

	float _walkSpeed;  //��������

	float _hitPoints;  //����ֵ

	float _damage;  //�����˺�ֵ

	cocos2d::CCPoint _velocity;  //������ƶ��ٶ�

	cocos2d::CCPoint _desiredPosition;  //Ҫ����λ��

	float _centerToSides;  //�������ĵ��ߵľ���
	float _centerToBottom;  //�������ĵ��ײ��ľ���

};

