#include "CoarseCollisionDetection.h"
#include <math.h>

BoundingSphere::BoundingSphere(const Vector3 & center, float radius):center(center), radius(radius)
{
}

BoundingSphere::BoundingSphere(const BoundingSphere & one, const BoundingSphere & two)
{
	Vector3 centerOffset = two.center - one.center;
	float distance = centerOffset.SquareMagnitude();
	float radiusDiff = two.radius - one.radius;

	// Check if the larger sphere encloses the small one
	if (radiusDiff*radiusDiff >= distance)
	{
		if (one.radius > two.radius)
		{
			center = one.center;
			radius = one.radius;
		}
		else
		{
			center = two.center;
			radius = two.radius;
		}
	}
	else
	{
		distance = sqrtf(distance);
		radius = (distance + one.radius + two.radius) * 0.5f;

		// The new centre is based on one's centre, moved towards
		// two's centre by an ammount proportional to the spheres'
		// radii.
		center = one.center;
		if (distance > 0)
		{
			center += centerOffset * ((radius - one.radius) / distance);
		}
	}
}

bool BoundingSphere::Overlaps(const BoundingSphere * other) const
{
	float distanceSquared = (center - other->center).SquareMagnitude();
	return distanceSquared < (radius + other->radius) * (radius + other->radius);
}

float BoundingSphere::GetGrowth(const BoundingSphere & other) const
{
	BoundingSphere newSphere(*this, other);

	return newSphere.radius*newSphere.radius - radius * radius;
}
