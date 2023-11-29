#if !defined(RAYTRACER_GEOMETRY_H)

#include <raytracer_hitable.h>

class sphere: public object
{
	public:
		sphere() {}
		sphere(v3f C, f32 r, material *M);
		virtual b32 ray_intersect(const ray &Ray, interval RayInterval, intersect_record &IntersectRecord) const;
		void normal_out_set(const ray &Ray, const v3f &NormalOut, intersect_record &IntersectRecord) const;

		v3f Center;
		f32 radius;
		material *Material;
};

sphere::sphere(v3f C, f32 r, material *M)
{
	Center = C;
	radius = r;
	Material = M;
}

b32
sphere::ray_intersect(const ray &Ray, interval RayInterval, intersect_record &IntersectRecord) const
{
	b32 Result = false;

	v3f CenterToOrigin = Ray.Origin - Center;
	f32 a = dot(Ray.Direction, Ray.Direction);
	f32 b = dot(CenterToOrigin, Ray.Direction);
	f32 c = dot(CenterToOrigin, CenterToOrigin) - SQUARE(radius);

	f32 discriminant = SQUARE(b) - a * c;
	if(discriminant > 0.0f)
	{
		f32 t = ((-b - sqrt(discriminant)) / a);
		if(RayInterval.surrounds(t))
		{
			IntersectRecord.t = t;
			IntersectRecord.Pos = Ray.lerp(IntersectRecord.t);
			v3f NormalOut = (1.0f / radius) * (IntersectRecord.Pos- Center);
			normal_out_set(Ray, NormalOut, IntersectRecord);
			IntersectRecord.Material = Material;
			Result = true;
		}
		else
		{
			t = ((-b + sqrt(discriminant)) / a);
			if(RayInterval.surrounds(t))
			{
				IntersectRecord.t = t;
				IntersectRecord.Pos = Ray.lerp(IntersectRecord.t);
				v3f NormalOut = (1.0f / radius) * (IntersectRecord.Pos- Center);
				normal_out_set(Ray, NormalOut, IntersectRecord);
				IntersectRecord.Material = Material;
				Result = true;
			}

		}
	}
	return(Result);
}

void
sphere::normal_out_set(const ray &Ray, const v3f &NormalOut, intersect_record &IntersectRecord) const
{
	object::normal_out_set(Ray, NormalOut, IntersectRecord);
}

class plane: public object
{
	public:
		plane(const v3f &Center, const v3f &N, f32 w, f32 d) {P0 = Center; Normal = N; width = w; depth = d;}
		virtual b32 ray_intersect(const ray &Ray, interval RayInterval, intersect_record &IntersectRecord) const;

		v3f P0;
		v3f Normal;
		f32 distance;
		f32 width;
		f32 depth;
};

b32
plane::ray_intersect(const ray &Ray, interval RayInterval, intersect_record &IntersectRecord) const
{
	b32 Result = false;

	f32 t_denom = dot(Ray.Direction, Normal);
	if(ABS(t_denom) > 0.0001f)
	{

		f32 t = dot((Ray.Origin - P0), Normal) / t_denom;
		if(RayInterval.surrounds(t))//(t < t_max) && (t > t_min))
		{
			v3f P = Ray.lerp(t);

			// NOTE(Justin): To render a finite plane, find the delta vector
			// between the intersection point P and the plane center P0. Then
			// check the absolute value of the x component of the delta vector.
			// If the absolute value of the x component is <= the plane width,
			// then the x component of the intersection point P is within the
			// plane. The same holds true for the z component. If the abs value
			// of the z component of the delta vector is <= the plane depth then
			// the intersection point P is within the depth range of the plane.

			// NOTE(Justin): This only works for planes that are laying flat. If
			// the need to support arbtrary plane orientation arises, more work
			// needs to be done.

			v3f Test = P - P0;

			if((ABS(Test[0]) <= width) &&
			   (ABS(Test[2]) <= depth))
			{
				IntersectRecord.t = t;
				IntersectRecord.Pos = P;
				IntersectRecord.Normal = Normal;
				Result = true;
			}
		}
	}
	return(Result);
}

#define RAYTRACER_GEOMETRY_H
#endif
