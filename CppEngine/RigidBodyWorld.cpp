#include "RigidBodyWorld.h"

void RigidBodyWorld::Start()
{
	BodyRegistration *reg = headBody;

	while (reg)
	{
		reg->body->ClearAccumulator();
		reg->body->CalculateDerivedData();

		reg = reg->next;
	}
}

void RigidBodyWorld::RunPhysics(float duration, bool needRestart)
{
	if (needRestart) Start();

	BodyRegistration *reg = headBody;

	while (reg)
	{
		reg->body->Integrate(duration);

		reg = reg->next;
	}
}
