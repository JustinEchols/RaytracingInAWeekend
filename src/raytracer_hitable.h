#if !defined(RAYTRACER_HITABLE_H)

#include <raytracer_ray.h>

class material;

struct intersect_record
{
	f32 t;
	v3f Pos;
	v3f Normal;
	b32 front_face;
	material *Material;
};

class object
{
	public:
		virtual b32 ray_intersect(const ray &Ray, interval RayInterval, intersect_record &IntersectRecord) const = 0;
		void normal_out_set(const ray &Ray, const v3f &NormalOut, intersect_record &IntersectRecord) const;
};

void
object::normal_out_set(const ray &Ray, const v3f &NormalOut, intersect_record &IntersectRecord) const
{
	b32 is_front_face = false;

	if(dot(Ray.Direction, NormalOut) < 0.0f)
	{
		// NOTE(Justin): Surface normals always point away from the surface. If
		// the dot product with the ray direction and surface normal is
		// negative, the vectors are pointing in the opposite direction. If the
		// vectors are pointing in opposite directions and the normal points
		// out, then the ray is intersecting the surface from the outside.

		is_front_face = true;
	}

	IntersectRecord.front_face = is_front_face;
	IntersectRecord.Normal = (is_front_face ? NormalOut : -(NormalOut));
}

#define RAYTRACER_HITABLE_H
#endif
