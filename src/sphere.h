#ifndef SPHERE_H
#define SPHERE_H

#include "phong.h"
#include "mathematics.h"
#include "ray.h"
#include "hitinfo.h"

struct Sphere
{
	float3	center;
	float	radius;
	uint32	materialId;
	uint32	id;
		
	__host__ __device__
	Sphere() { }

	__host__ __device__
	Sphere(float3 const& ce, float const& r, uint32 const& matId) : center(ce), radius(r), materialId(matId)
	{}	

	__device__ HitInfo intersect(Ray const& ray) {
		float aa = CUDA::dot(ray.direction, ray.direction);
		float bb = 2.f * (CUDA::dot(ray.direction, float3_sub(ray.origin, center)));
		float cc = CUDA::dot(float3_sub(ray.origin, center), float3_sub(ray.origin, center)) - (radius * radius);

		float D = bb * bb - 4 * aa * cc;
		HitInfo hit;
		if (D > 0) {
			float sD = sqrt(D);
			float t0 = (-bb - sD) / (2 * aa);
			if (t0 > 0){
				hit.hit = true;
				hit.t = t0;
				hit.sphereId = id;
				return hit;
			}
			float t1 = (-bb + sD) / (2 * aa);
			if (t1 > 0){
				hit.hit = true;
				hit.t = t1;
				hit.sphereId = id;
				return hit;
			}
		}
		hit.hit = false;
		return hit;
	}

	__device__ float3 getNormal(float3 const& position) const {
		float3 n = float3_sub(position, center);
		return normalize(n);		
	}
	
};


#endif
