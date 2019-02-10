#ifndef _PhysicsEngineForFun_Particle_
#define _PhysicsEngineForFun_Particle_

#include "Vector3.h"

namespace PhysicsEngineForFun
{
	class Particle
	{
	public:
		void SetMass(real mass);
		real GetMass() const;
		void Integrate(real duration);
		void AddForce(const Vector3 &force);

		Vector3 position;
		Vector3 velocity;
		Vector3 acceleration;
		real damping;
		
	private:
		Vector3 force;
		real inverseMass;
	};
}

#endif
