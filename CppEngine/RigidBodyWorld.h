#ifndef _PhysicsEngineForFun_RigidBodyWorld_
#define _PhysicsEngineForFun_RigidBodyWorld_

#include "RigidBody.h"

class RigidBodyWorld
{
private:
	struct BodyRegistration
	{
		RigidBody *body;
		BodyRegistration * next;
	};
	BodyRegistration* headBody;
public:
	void Start();
	void RunPhysics(float duration, bool needRestart = true);
};

#endif