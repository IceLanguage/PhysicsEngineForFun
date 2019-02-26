#ifndef _PhysicsEngineForFun_Particle_
#define _PhysicsEngineForFun_Particle_

#include "Vector3.h"

class Particle
{
public:
	void SetMass(float mass);
	float GetMass() const;
	void Integrate(float duration);
	void AddForce(const Vector3 &force);
	bool hasFiniteMass() const;//Returns true if the mass of the particle is not-infinite.
	float GetInverseMass() const;
	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	float damping;

private:
	Vector3 force;
	float inverseMass;
};

#endif
