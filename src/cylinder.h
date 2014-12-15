#ifndef CYLINDER_H
#define CYLINDER_H

#include "phong.h"

#include "mathematics.h"

#include "ray.h"

struct Cylinder
{
	float3	center;
	float	radius;
	float3  direction;
	uint32	materialId;
	uint32	id;
		
	void set(float3 const& ce, float const& r, float3 const& dir, uint32 const& matId)
	{
		materialId = matId;
		center = ce;
		radius = r;
		direction = dir;
		direction = normalize(direction);
	}	

	__device__ HitInfo intersect(Ray const& ray) {
		const float3 c = CUDA::float3_sub(ray.origin, center);
		const float3 b = ray.direction;
		
		float bd = CUDA::dot(b,direction);
		float b2 = CUDA::dot(b, b);
		float cb = CUDA::dot(c, b);
		float cd = CUDA::dot(c, direction);
		float c2 = CUDA::dot(c, c);

		float A = b2 - bd*bd;
		float B = 2 * cb - 2 * cd*bd;
		float C = c2 - cd*cd - radius*radius;
		float D = B*B - 4 * A*C;
		HitInfo hit;
		if (D > 0)
		{
			float sD = sqrt(D);
			float t0 = (-B - sD) / (2 * A);			
			float t1 = (-B + sD) / (2 * A);

			if (t0 > 0){
				hit.hit = true;
				hit.t = t0;
				return hit;
			}
			
			if (t1 > 0){
				hit.hit = true;
				hit.t = t1;
				return hit;
			}
		}
		hit.hit = false;
		return hit;
		
	}

	__device__ float3 getNormal(float3 const& position) const {
		float3 proj = CUDA::float3_add(center, CUDA::cross((CUDA::cross(CUDA::float3_sub(position, center), direction)) ,direction));		
		float3 n = CUDA::float3_sub(position, proj);
		return CUDA::normalize(n);		
	}
	
};


#endif
