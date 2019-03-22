#include "RigidBodyContact.h"
#include <cstddef>
#include <float.h>
#include <math.h>

static inline float transformToAxis(
	const CollisionBox &box,
	const Vector3 &axis
)
{
	return
		box.halfSize.x * fabsf(Vector3::Dot(axis , box.transform.GetColumn(0))) +
		box.halfSize.y * fabsf(Vector3::Dot(axis , box.transform.GetColumn(1)))+
		box.halfSize.z * fabsf(Vector3::Dot(axis , box.transform.GetColumn(2)));
}

unsigned int CollisionDetector::SphereAndSphere(const CollisionSphere & one, const CollisionSphere & two, CollisionData * data)
{
	if (data->contactsLeft <= 0) return 0;

	Vector3 positionOne = one.GetPos();
	Vector3 positionTwo = two.GetPos();

	Vector3 midline = positionOne - positionTwo;
	float size = midline.Magnitude();

	if (size <= 0.0f || size >= one.radius + two.radius)
	{
		return 0;
	}

	Vector3 normal = Vector3::Normalize(midline);

	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->contactPoint = positionOne + midline * 0.5f;
	contact->penetrationDepth = (one.radius + two.radius - size);
	
	contact->body[0] = one.body;
	contact->body[1] = two.body;
	contact->restitution = data->restitution;
	contact->friction = data->friction;

	data->AddContacts(1);

	return 1;
}

unsigned int CollisionDetector::SphereAndHalfSpace(const CollisionSphere & sphere, const CollisionPlane & plane, CollisionData * data)
{
	if (data->contactsLeft <= 0) return 0;

	Vector3 position = sphere.GetPos();

	float ballDistance =
		Vector3::Dot(plane.normal, position) -
		sphere.radius - plane.offset;

	if (ballDistance >= 0) return 0;

	Contact* contact = data->contacts;
	contact->contactNormal = plane.normal;
	contact->penetrationDepth = -ballDistance;
	contact->contactPoint =
		position - plane.normal * (ballDistance + sphere.radius);

	contact->body[0] = sphere.body;
	contact->body[1] = NULL;
	contact->restitution = data->restitution;
	contact->friction = data->friction;

	data->AddContacts(1);
	return 1;
}

unsigned int CollisionDetector::SphereAndTruePlane(const CollisionSphere & sphere, const CollisionPlane & plane, CollisionData * data)
{
	if (data->contactsLeft <= 0) return 0;

	Vector3 position = sphere.GetPos();

	float centreDistance = Vector3::Dot(plane.normal, position) - plane.offset;

	if (centreDistance * centreDistance > sphere.radius * sphere.radius)
	{
		return 0;
	}

	Vector3 normal = plane.normal;
	float penetration = -centreDistance;
	if (centreDistance < 0)
	{
		normal *= -1;
		penetration = -penetration;
	}
	penetration += sphere.radius;

	Contact* contact = data->contacts;
	contact->contactNormal = normal;
	contact->penetrationDepth = penetration;
	contact->contactPoint = position - plane.normal * centreDistance;

	contact->body[0] = sphere.body;
	contact->body[1] = NULL;
	contact->restitution = data->restitution;
	contact->friction = data->friction;

	data->AddContacts(1);
	return 1;
}

unsigned int CollisionDetector::BoxAndHalfSpace(const CollisionBox & box, const CollisionPlane & plane, CollisionData * data)
{
	if (data->contactsLeft <= 0) return 0;

	Vector3 position = box.GetPos();

	
	float projectedRadius = transformToAxis(box, plane.normal);

	float boxDistance = Vector3::Dot(plane.normal, position) -
		projectedRadius;

	if (boxDistance > plane.offset) return 0;

	static const float mults[8][3] = { { 1,1,1 },{ -1,1,1 },{ 1,-1,1 },{ -1,-1,1 },
	{ 1,1,-1 },{ -1,1,-1 },{ 1,-1,-1 },{ -1,-1,-1 } };

	Contact* contact = data->contacts;
	unsigned int contactsUsed = 0;
	for (unsigned i = 0; i < 8; i++) 
	{
		Vector3 vertexPos(mults[i][0], mults[i][1], mults[i][2]);
		vertexPos *= box.halfSize;
		vertexPos = box.transform.Transform(vertexPos);

		float vertexDistance = Vector3::Dot(vertexPos , plane.normal);

		if (vertexDistance <= plane.offset)
		{
			contact->contactPoint = plane.normal;
			contact->contactPoint *= (vertexDistance - plane.offset);
			contact->contactPoint += vertexPos;
			contact->contactNormal = plane.normal;
			contact->penetrationDepth = plane.offset - vertexDistance;

			contact->body[0] = box.body;
			contact->body[1] = NULL;
			contact->restitution = data->restitution;
			contact->friction = data->friction;

			contact++;
			contactsUsed++;
			if (contactsUsed == (unsigned int)data->contactsLeft) return contactsUsed;
		}
	}

	data->AddContacts(contactsUsed);
	return contactsUsed;
}

unsigned int CollisionDetector::BoxAndSphere(const CollisionBox & box, const CollisionSphere & sphere, CollisionData * data)
{
	Vector3 centre = sphere.GetPos();
	Vector3 relCentre = box.transform.TransformInverse(centre);

	// Early out check to see if we can exclude the contact
	if (fabsf(relCentre.x) - sphere.radius > box.halfSize.x ||
		fabsf(relCentre.y) - sphere.radius > box.halfSize.y ||
		fabsf(relCentre.z) - sphere.radius > box.halfSize.z)
	{
		return 0;
	}

	Vector3 closestPt(0, 0, 0);
	float dist;

	// Clamp each coordinate to the box.
	dist = relCentre.x;
	if (dist > box.halfSize.x) dist = box.halfSize.x;
	if (dist < -box.halfSize.x) dist = -box.halfSize.x;
	closestPt.x = dist;

	dist = relCentre.y;
	if (dist > box.halfSize.y) dist = box.halfSize.y;
	if (dist < -box.halfSize.y) dist = -box.halfSize.y;
	closestPt.y = dist;

	dist = relCentre.z;
	if (dist > box.halfSize.z) dist = box.halfSize.z;
	if (dist < -box.halfSize.z) dist = -box.halfSize.z;
	closestPt.z = dist;

	// Check we're in contact
	dist = (closestPt - relCentre).SquareMagnitude();
	if (dist > sphere.radius * sphere.radius) return 0;

	// Compile the contact
	Vector3 closestPtWorld = box.transform.Transform(closestPt);

	Contact* contact = data->contacts;
	contact->contactNormal = (closestPtWorld - centre);
	contact->contactNormal.Normalize();
	contact->contactPoint = closestPtWorld;
	contact->penetrationDepth = sphere.radius - sqrtf(dist);

	contact->body[0] = box.body;
	contact->body[1] = sphere.body;
	contact->restitution = data->restitution;
	contact->friction = data->friction;

	data->AddContacts(1);
	return 1;
}

static inline float penetrationOnAxis(
	const CollisionBox &one,
	const CollisionBox &two,
	const Vector3 &axis,
	const Vector3 &toCentre
)
{
	// Project the half-size of one onto axis
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(two, axis);

	// Project this onto the axis
	float distance = fabsf(Vector3::Dot(toCentre, axis));

	// Return the overlap (i.e. positive indicates
	// overlap, negative indicates separation).
	return oneProject + twoProject - distance;
}

static inline bool tryAxis(
	const CollisionBox &one,
	const CollisionBox &two,
	Vector3 axis,
	const Vector3& toCentre,
	unsigned index,

	// These values may be updated
	float& smallestPenetration,
	unsigned &smallestCase
)
{
	// Make sure we have a normalized axis, and don't check almost parallel axes
	if (axis.SquareMagnitude() < 0.0001) return true;
	axis.Normalize();

	float penetration = penetrationOnAxis(one, two, axis, toCentre);

	if (penetration < 0) return false;
	if (penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}
	return true;
}

static inline void fillPointFaceBoxBox(
	const CollisionBox &one,
	const CollisionBox &two,
	const Vector3 &toCentre,
	CollisionData *data,
	unsigned int best,
	float pen
)
{
	// This method is called when we know that a vertex from
	// box two is in contact with box one.

	Contact* contact = data->contacts;

	// We know which axis the collision is on (i.e. best),
	// but we need to work out which of the two faces on
	// this axis.
	Vector3 normal = one.transform.GetColumn(best);
	if (Vector3::Dot(one.transform.GetColumn(best) , toCentre)> 0)
	{
		normal = normal * -1.0f;
	}

	// Work out which vertex of box two we're colliding with.
	// Using toCentre doesn't work!
	Vector3 vertex = two.halfSize;
	if (Vector3::Dot(two.transform.GetColumn(0) , normal) < 0) vertex.x = -vertex.x;
	if (Vector3::Dot(two.transform.GetColumn(1) , normal) < 0) vertex.y = -vertex.y;
	if (Vector3::Dot(two.transform.GetColumn(2) , normal) < 0) vertex.z = -vertex.z;

	// Create the contact data
	contact->contactNormal = normal;
	contact->penetrationDepth = pen;
	contact->contactPoint = two.transform * vertex;

	contact->body[0] = one.body;
	contact->body[1] = two.body;
	contact->restitution = data->restitution;
	contact->friction = data->friction;
}

static inline Vector3 contactPoint(
	const Vector3 &pOne,
	const Vector3 &dOne,
	float oneSize,
	const Vector3 &pTwo,
	const Vector3 &dTwo,
	float twoSize,

	// If this is true, and the contact point is outside
	// the edge (in the case of an edge-face contact) then
	// we use one's midpoint, otherwise we use two's.
	bool useOne)
{
	Vector3 toSt, cOne, cTwo;
	float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	float denom, mua, mub;

	smOne = dOne.SquareMagnitude();
	smTwo = dTwo.SquareMagnitude();
	dpOneTwo = Vector3::Dot(dTwo , dOne);

	toSt = pOne - pTwo;
	dpStaOne = Vector3::Dot(dOne , toSt);
	dpStaTwo = Vector3::Dot(dTwo , toSt);

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	// Zero denominator indicates parrallel lines
	if (fabsf(denom) < 0.0001f) {
		return useOne ? pOne : pTwo;
	}

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

	// If either of the edges has the nearest point out
	// of bounds, then the edges aren't crossed, we have
	// an edge-face contact. Our point is on the edge, which
	// we know from the useOne parameter.
	if (mua > oneSize ||
		mua < -oneSize ||
		mub > twoSize ||
		mub < -twoSize)
	{
		return useOne ? pOne : pTwo;
	}
	else
	{
		cOne = pOne + dOne * mua;
		cTwo = pTwo + dTwo * mub;

		return cOne * 0.5 + cTwo * 0.5;
	}
}

unsigned int CollisionDetector::BoxAndBox(const CollisionBox & one, const CollisionBox & two, CollisionData * data)
{
	Vector3 toCentre = two.GetPos() - one.GetPos();
	float pen = FLT_MAX;
	unsigned best = 0xffffff;
#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;

	// Now we check each axes, returning if it gives us
	// a separating axis, and keeping track of the axis with
	// the smallest penetration otherwise.
	CHECK_OVERLAP(one.transform.GetColumn(0), 0);
	CHECK_OVERLAP(one.transform.GetColumn(1), 1);
	CHECK_OVERLAP(one.transform.GetColumn(2), 2);

	CHECK_OVERLAP(two.transform.GetColumn(0), 3);
	CHECK_OVERLAP(two.transform.GetColumn(1), 4);
	CHECK_OVERLAP(two.transform.GetColumn(2), 5);

	// Store the best axis-major, in case we run into almost
	// parallel edge collisions later
	unsigned bestSingleAxis = best;

	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(0) , two.transform.GetColumn(0)), 6);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(0) , two.transform.GetColumn(1)), 7);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(0) , two.transform.GetColumn(2)), 8);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(1) , two.transform.GetColumn(0)), 9);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(1) , two.transform.GetColumn(1)), 10);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(1) , two.transform.GetColumn(2)), 11);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(2) , two.transform.GetColumn(0)), 12);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(2) , two.transform.GetColumn(1)), 13);
	CHECK_OVERLAP(Vector3::Cross(one.transform.GetColumn(2) , two.transform.GetColumn(2)), 14);

#undef CHECK_OVERLAP

	if (best < 3)
	{
		// We've got a vertex of box two on a face of box one.
		fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
		data->AddContacts(1);
		return 1;
	}
	else if (best < 6)
	{
		// We've got a vertex of box one on a face of box two.
		// We use the same algorithm as above, but swap around
		// one and two (and therefore also the vector between their
		// centres).
		fillPointFaceBoxBox(two, one, toCentre*-1.0f, data, best - 3, pen);
		data->AddContacts(1);
		return 1;
	}
	else
	{
		// We've got an edge-edge contact. Find out which axes
		best -= 6;
		unsigned oneAxisIndex = best / 3;
		unsigned twoAxisIndex = best % 3;
		Vector3 oneAxis = one.transform.GetColumn(oneAxisIndex);
		Vector3 twoAxis = two.transform.GetColumn(twoAxisIndex);
		Vector3 axis = Vector3::Cross(oneAxis, twoAxis);
		axis.Normalize();

		// The axis should point from box one to box two.
		if (Vector3::Dot( axis , toCentre )> 0) axis = axis * -1.0f;

		// We have the axes, but not the edges: each axis has 4 edges parallel
		// to it, we need to find which of the 4 for each object. We do
		// that by finding the point in the centre of the edge. We know
		// its component in the direction of the box's collision axis is zero
		// (its a mid-point) and we determine which of the extremes in each
		// of the other axes is closest.
		Vector3 ptOnOneEdge = one.halfSize;
		Vector3 ptOnTwoEdge = two.halfSize;
		for (unsigned int i = 0; i < 3; i++)
		{
			if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
			else if (Vector3::Dot(one.transform.GetColumn(i) , axis) > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

			if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
			else if (Vector3::Dot(two.transform.GetColumn(i) , axis) < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		// Move them into world coordinates (they are already oriented
		// correctly, since they have been derived from the axes).
		ptOnOneEdge = one.transform * ptOnOneEdge;
		ptOnTwoEdge = two.transform * ptOnTwoEdge;

		// So we have a point and a direction for the colliding edges.
		// We need to find out point of closest approach of the two
		// line-segments.
		Vector3 vertex = contactPoint(
			ptOnOneEdge, oneAxis, one.halfSize[oneAxisIndex],
			ptOnTwoEdge, twoAxis, two.halfSize[twoAxisIndex],
			bestSingleAxis > 2
		);

		// We can fill the contact.
		Contact* contact = data->contacts;

		contact->penetrationDepth = pen;
		contact->contactNormal = axis;
		contact->contactPoint = vertex;

		contact->body[0] = one.body;
		contact->body[1] = two.body;
		contact->restitution = data->restitution;
		contact->friction = data->friction;

		data->AddContacts(1);
		return 1;
	}
	return 0;
}

Vector3 CollisionPrimitive::GetPos() const
{
	return transform.GetColumn(3);
}

void CollisionData::AddContacts(int count)
{
	contactsLeft -= count;
	contactCount += count;

	contacts += count;
}
