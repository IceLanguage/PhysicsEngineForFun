#ifndef _PhysicsEngineForFun_CoarseCollisionDetection_
#define _PhysicsEngineForFun_CoarseCollisionDetection_

#include "RigidBody.h"
#include <cstddef>

struct BoundingSphere
{
private:
	Vector3 center;
	float radius;
public:
	BoundingSphere(const Vector3 &center, float radius);
	BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

	/**
	* Checks if the bounding sphere overlaps with the other given
	* bounding sphere.
	*/
	bool Overlaps(const BoundingSphere *other) const;

	float GetGrowth(const BoundingSphere &other) const;

	float GetSize() const
	{
		return 4 / 3 * 3.14159f * radius * radius * radius;
	}
};

/**
* Stores a potential contact to check later.
*/
struct PotentialContact
{
	/**
	* Holds the bodies that might be in contact.
	*/
	RigidBody* body[2];
};

template<class BoundingVolumeClass>
class BVHNode
{
public:
	BVHNode * children[2];
	BVHNode * parent;
	//Only leaf nodes can have a rigid body defined (see isLeaf).
	RigidBody* body;

	BoundingVolumeClass BV;

	BVHNode(BVHNode *parent, const BoundingVolumeClass &volume,
		RigidBody* body = NULL)
		: parent(parent), BV(volume), body(body)
	{
		children[0] = children[1] = NULL;
	}

	~BVHNode();

	bool isLeaf() const
	{
		return (body != NULL);
	}

	/**
	param, contacts: Checks the potential contacts from this node downwards in the hierarchy, and write them in it
	param, limit   :
	return         : the number of potential contacts it founds
	**/
	unsigned int GetPotentialContacts(PotentialContact* contacts, unsigned int limit) const;

	/**
	param, other   : Checks the potential contacts between this node and the given other node
	param, contacts: write potential contacts in it
	param, limit   :
	return         : the number of potential contacts it founds
	**/
	unsigned int GetPotentialContactsWith(
		const BVHNode<BoundingVolumeClass> *other,
		PotentialContact* contacts,
		unsigned int limit) const;

	void Insert(RigidBody* newBody, const BoundingVolumeClass &newBV);
protected:
	bool Overlaps(const BVHNode<BoundingVolumeClass> *other) const;
	void RecalculateBoundingVolume();
};


template<class BoundingVolumeClass>
inline BVHNode<BoundingVolumeClass>::~BVHNode()
{
	if (parent)
	{
		BVHNode<BoundingVolumeClass> *sibling;
		if (parent->children[0] == this) sibling = parent->children[1];
		else sibling = parent->children[0];

		// Write its data to our parent
		parent->BV = sibling->BV;
		parent->body = sibling->body;
		parent->children[0] = sibling->children[0];
		parent->children[1] = sibling->children[1];

		sibling->parent = NULL;
		sibling->body = NULL;
		sibling->children[0] = NULL;
		sibling->children[1] = NULL;
		delete sibling;

		parent->RecalculateBoundingVolume();
	}

	if (children[0]) {
		children[0]->parent = NULL;
		delete children[0];
	}
	if (children[1]) {
		children[1]->parent = NULL;
		delete children[1];
	}
}

template<class BoundingVolumeClass>
inline unsigned int BVHNode<BoundingVolumeClass>::GetPotentialContacts(PotentialContact * contacts, unsigned int limit) const
{
	if (isLeaf() || limit == 0) return 0;

	return children[0]->GetPotentialContactsWith(
		children[1], contacts, limit
	);
}

template<class BoundingVolumeClass>
inline void BVHNode<BoundingVolumeClass>::Insert(RigidBody * newBody, const BoundingVolumeClass & newBV)
{
	if (isLeaf())
	{

		children[0] = new BVHNode<BoundingVolumeClass>(
			this, this->BV, this->body
			);

		children[1] = new BVHNode<BoundingVolumeClass>(
			this, newBV, newBody
			);

		this->body = NULL;

		RecalculateBoundingVolume();
	}

	else
	{
		if (children[0]->BV.GetGrowth(newBV) <
			children[1]->BV.GetGrowth(newBV))
		{
			children[0]->Insert(newBody, newBV);
		}
		else
		{
			children[1]->Insert(newBody, newBV);
		}
	}
}

template<class BoundingVolumeClass>
inline bool BVHNode<BoundingVolumeClass>::Overlaps(const BVHNode<BoundingVolumeClass>* other) const
{
	return BV->Overlaps(other->BV);
}

template<class BoundingVolumeClass>
inline void BVHNode<BoundingVolumeClass>::RecalculateBoundingVolume()
{
	if (isLeaf()) return;

	BV = BoundingVolumeClass(
		children[0]->BV,
		children[1]->BV
	);

	if (parent) parent->RecalculateBoundingVolume();
}

template<class BoundingVolumeClass>
inline unsigned int BVHNode<BoundingVolumeClass>::GetPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact * contacts, unsigned int limit) const
{
	if (!Overlaps(other) || limit == 0) return 0;

	if (isLeaf() && other->isLeaf())
	{
		contacts->body[0] = body;
		contacts->body[1] = other->body;
		return 1;
	}

	if (other->isLeaf() ||
		(!isLeaf() && BV->GetSize() >= other->BV->GetSize()))
	{
		unsigned count = children[0]->GetPotentialContactsWith(
			other, contacts, limit
		);

		if (limit > count) {
			return count + children[1]->GetPotentialContactsWith(
				other, contacts + count, limit - count
			);
		}
		else {
			return count;
		}
	}
	else
	{
		unsigned count = GetPotentialContactsWith(
			other->children[0], contacts, limit
		);

		if (limit > count) {
			return count + GetPotentialContactsWith(
				other->children[1], contacts + count, limit - count
			);
		}
		else {
			return count;
		}
	}
}

#endif