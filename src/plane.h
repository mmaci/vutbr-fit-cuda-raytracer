#ifndef PLANE_H
#define PLANE_H

#include "phong.h"

struct Plane
{	
	Plane(){}

	Plane(float3 n, float3 p, uint32 matId)
	{
		materialId = matId;
		normal = normalize(n);
		d = -1.f * dot(normal, p);
	}	

	__device__ HitInfo intersect(Ray const& ray) {
		float np = CUDA::dot(normal, ray.direction);
		HitInfo hit;
		if (np != 0) {
			float t = -1.f * (d + CUDA::dot(normal, ray.origin)) / np;
			if (t > 0) {
				hit.hit = true;
				hit.t = t;
				return hit;
			}
		}

		hit.hit = false;
		return hit;
	}			

	uint32 id;
	float3 normal;
	float d;
	uint32 materialId;
};

#endif
