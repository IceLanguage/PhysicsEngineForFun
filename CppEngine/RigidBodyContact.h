#ifndef _PhysicsEngineForFun_RigidBodyContact_
#define _PhysicsEngineForFun_RigidBodyContact_

#include "RigidBody.h"

class Contact
{
public:
	Vector3 contactPoint;// in world coordinates
	Vector3 contactNormal;
	float penetrationDepth;

	/**
	* Holds the lateral friction coefficient at the contact.
	*/
	float friction;

	/**
	* Holds the normal restitution coefficient at the contact.
	*/
	float restitution;

	/**
	* Holds the bodies that are involved in the contact. The
	* second of these can be NULL, for contacts with the scenery.
	*/
	RigidBody* body[2];
};

struct CollisionData
{
public:
	Contact * contacts;

	/** Holds the maximum number of contacts the array can take. */
	int contactsLeft;

	/** Holds the number of contacts found so far. */
	unsigned int contactCount;

	/** Holds the friction value to write into any collisions. */
	float friction;

	/** Holds the restitution value to write into any collisions. */
	float restitution;

	void AddContacts(int count);
};

/**
* Represents a primitive to detect collisions against.
*/
class CollisionPrimitive
{
public:
	/**
	* The rigid body that is represented by this primitive.
	*/
	RigidBody * body;

	/**
	* The offset of this primitive from the given rigid body.
	*/
	Matrix4 offset;
	Matrix4 transform;
	Vector3 GetPos() const;
	
};

class CollisionSphere : public CollisionPrimitive
{
public:
	float radius;
};

class CollisionPlane : public CollisionPrimitive
{
public:
	Vector3 normal;
	float offset;//  The distance of the plane from the origin.
};

class CollisionBox : public CollisionPrimitive
{
public:
	Vector3 halfSize;
};

class CollisionDetector
{
public:
	static unsigned int SphereAndSphere(
		const CollisionSphere &one,
		const CollisionSphere &two,
		CollisionData *data
	);

	static unsigned int SphereAndHalfSpace(
		const CollisionSphere &sphere,
		const CollisionPlane &plane,
		CollisionData *data
	);

	static unsigned int SphereAndTruePlane(
		const CollisionSphere &sphere,
		const CollisionPlane &plane,
		CollisionData *data
	);

	static unsigned int BoxAndHalfSpace(
		const CollisionBox &box,
		const CollisionPlane &plane,
		CollisionData *data
	);

	static unsigned int BoxAndSphere(
		const CollisionBox &box,
		const CollisionSphere &sphere,
		CollisionData *data
	);

	static unsigned int BoxAndBox(
		const CollisionBox &one,
		const CollisionBox &two,
		CollisionData *data
	);
};
#endif
