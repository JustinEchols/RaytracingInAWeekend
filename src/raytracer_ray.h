#if !defined(RAYTRACER_RAY_H)

#include <raytracer_math.h>

class ray
{ 
	public:
		ray() {}
		ray(const v3f &O, const v3f &D);
		v3f lerp(f32 t) const;

		v3f Origin;
		v3f Direction;
};

ray::ray(const v3f &O, const v3f &D)
{
	Origin = O;
	Direction = unit_vector(D);
}

v3f
ray::lerp(f32 t) const
{
	return(Origin + t * Direction);
}

#define RAYTRACER_RAY_H
#endif
