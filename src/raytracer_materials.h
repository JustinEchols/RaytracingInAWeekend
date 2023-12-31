#if !defined(RAYTRACER_MATERIALS_H)

#include <raytracer_ray.h>

class material
{
	public:
		virtual b32 ray_scatter(const ray &RayIn, const intersect_record &IntersectRecord, v3f &Atten, ray &RayScattered) const = 0;
};

class lambertian: public material
{
	public:
		lambertian(const v3f &A) {Albedo = A;}

		virtual b32 ray_scatter(const ray &RayIn, const intersect_record &IntersectRecord, v3f &Atten, ray &RayScattered) const;

		v3f Albedo;
};

b32
lambertian::ray_scatter(const ray &RayIn, const intersect_record &IntersectRecord, v3f &Atten, ray &RayScattered) const
{
	v3f Dir = IntersectRecord.Normal + v3f_rand_unit_vector();
	if(v3f_near_zero(Dir))
	{
		Dir = IntersectRecord.Normal;
	}

	RayScattered = ray(IntersectRecord.Pos, Dir);
	Atten = Albedo;
	return(true);
}

class metal: public material
{
	public:
		metal(const v3f &A, f32 f);

		virtual b32 ray_scatter(const ray &RayIn, const intersect_record &IntersectRecord, v3f &Atten, ray &RayScattered) const;

		v3f Albedo;
		f32 fuzz;
};

metal::metal(const v3f &A, f32 f)
{
	Albedo = A;
	fuzz = ((f < 1.0f) ? f : 1.0f);
}

b32
metal::ray_scatter(const ray &RayIn, const intersect_record &IntersectRecord, v3f &Atten, ray &RayScattered) const
{
	v3f R = reflect(RayIn.Direction, IntersectRecord.Normal);
	R += fuzz * v3f_rand_unit_vector();
	RayScattered = ray(IntersectRecord.Pos, R);
	Atten = Albedo;
	return(true);
}

class dielectric: public material
{
	public:
		dielectric(f32 refract_index);

		virtual b32 ray_scatter(const ray &RayIn, const intersect_record &IntersectRecord, v3f &Atten, ray &RayScattered) const;

		f32 refraction_index;
};

dielectric::dielectric(f32 refract_index)
{
	refraction_index = refract_index;
}

b32
dielectric::ray_scatter(const ray &RayIn, const intersect_record &IntersectRecord, v3f &Atten, ray &RayScattered) const
{
	Atten = v3f(1.0f, 1.0f, 1.0f);

	f32 ratio = (IntersectRecord.front_face ? (1.0f / refraction_index) : refraction_index);

	f32 cos_theta = MIN(dot(-RayIn.Direction, IntersectRecord.Normal), 1.0f);
	f32 sin_theta = sqrt(1.0f - SQUARE(cos_theta));

	b32 cannot_refract = ((sin_theta * ratio) > 1.0f);

	v3f Dir;
	if((cannot_refract) || (reflectance(cos_theta, ratio) > random_unit()))
	{
		Dir = reflect(RayIn.Direction, IntersectRecord.Normal);
	}
	else
	{
		Dir = refract(RayIn.Direction, IntersectRecord.Normal, ratio);
	}

	RayScattered = ray(IntersectRecord.Pos, Dir);

	return(true);


}
#define RAYTRACER_MATERIALS_H
#endif
