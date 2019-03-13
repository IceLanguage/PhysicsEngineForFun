#include "RigidBody.h"

void RigidBody::CalculateDerivedData()
{
	transformMatrix.SetOrientationAndPos(orientation, position);
	inverseInertiaTensorWorld = transformMatrix * inverseInertiaTensor * transformMatrix.GetTransposeMatrix3();
}

void RigidBody::SetInertiaTensor(const Matrix3 & inertiaTensor)
{
	inverseInertiaTensor.SetInverse(inertiaTensor);
}
