#include<fstream>
#include <stdint.h>

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u32 b32;

typedef float f32;
typedef double f64;

#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <raytracer_hitable.h>
#include <raytracer_hitable_list.h>
#include <raytracer_geometry.h>
#include <raytracer_materials.h>
#include <raytracer_renderer.h>

object_list
world_list_final()
{
	object_list Result;

	material *MaterialGround = new lambertian(v3f(0.5f, 0.5f, 0.5f));
	Result.add(new sphere(v3f(0.0f, -1000.0f, 0.0f), 1000.0f, MaterialGround));

	for(s32 x = -11; x < 11; x++)
	{
		for(s32 y = -11; y < 11; y++) {
			f32 choose_mat = random_unit();
			v3f Center = v3f(x + 0.9f * random_unit(), 0.2f, y + 0.9f * random_unit());

			if((Center - v3f(4.0f, 0.2f, 0.0f)).length() > 0.9f)
			{
				material *SphereMaterial;
				if(choose_mat < 0.8f)
				{
					v3f Albedo = v3f_hadamard(v3f_rand(), v3f_rand());
					SphereMaterial = new lambertian(Albedo);
					Result.add(new sphere(Center, 0.2f, SphereMaterial));
				}
				else if(choose_mat < 0.95f)
				{
					v3f Albedo = v3f(random_interval(0.5f, 1.0f), random_interval(0.5f, 1.0f), random_interval(0.5f, 1.0f));
					f32 fuzzy = random_interval(0.0f, 0.5f);
					SphereMaterial = new metal(Albedo, fuzzy);
					Result.add(new sphere(Center, 0.2f, SphereMaterial));
				}
				else
				{
					SphereMaterial = new dielectric(1.5);
					Result.add(new sphere(Center, 0.2f, SphereMaterial));
				}
			}
		}
	}

	material *Material1 = new dielectric(1.5f);
	material *Material2 = new lambertian(v3f(0.4f, 0.2f, 0.1f));
	material *Material3 = new metal(v3f(0.7f, 0.6f, 0.5f), 0.0f);

	Result.add(new sphere(v3f(0.0f, 1.0f, 0.0f), 1.0f, Material1));
	Result.add(new sphere(v3f(-4.0f, 1.0f, 0.0f), 1.0f, Material2));
	Result.add(new sphere(v3f(4.0f, 1.0f, 0.0f), 1.0f, Material3));

	return(Result);
}

object_list
world_list_defocus_blur()
{
	object_list Result;

	material *MaterialGround = new lambertian(v3f(0.8f, 0.8f, 0.5f));
	material *MaterialCenter = new lambertian(v3f(0.1f, 0.2f, 0.5f));
	material *MaterialLeft = new dielectric(1.5f);
	material *MaterialRight = new metal(v3f(0.8f, 0.6f, 0.2f), 0.0f);

	Result.add(new sphere(v3f(0.0f, -100.5f, -1.0f), 100.0f, MaterialGround));
	Result.add(new sphere(v3f(0.0f, 0.0f, -1.0f), 0.5f, MaterialCenter));
	Result.add(new sphere(v3f(-1.0f, 0.0f, -1.0f), 0.5f, MaterialLeft));
	Result.add(new sphere(v3f(-1.0f, 0.0f, -1.0f), -0.4f, MaterialLeft));
	Result.add(new sphere(v3f(1.0f, 0.0f, -1.0f), 0.5f, MaterialRight));

	return(Result);
}

object_list
world_two_spheres_diffuse()
{
	object_list Result;

	material *SphereMaterial = new lambertian(v3f(0.5f, 0.5f, 0.5f));

	Result.add(new sphere(v3f(0.0f, 0.0f, -1.0f), 0.5f, SphereMaterial));
    Result.add(new sphere(v3f(0.0f, -100.5f, -1.0f), 100.0f, SphereMaterial));

	return(Result);

}

int main(int argc, char **argv)
{
	char *filename = "test.ppm";
	
	char scene = *(*(argv + argc - 1));

	object_list World;
	camera Camera;
	switch(scene)
	{
		case '1':
		{
			World = world_two_spheres_diffuse();

			Camera.image_width = 400;
			Camera.aspect_ratio = 16.0f / 9.0f;

			Camera.fov_vertical = 60.0f;

			Camera.LookFrom = v3f(0.0f, 0.0f, 1.0f);
			Camera.LookAt = v3f(0.0f, 0.0f, 0.0f);
			Camera.Up = v3f(0.0f, 1.0f, 0.0f);

			Camera.defocus_angle = 0.0f;
			Camera.focus_dist = 1.0f;

			Camera.sample_count = 100;
			Camera.scatter_count = 50;
			
			Camera.init();
		} break;
		case '2':
		{
			World = world_list_defocus_blur();

			Camera.image_width = 400;
			Camera.aspect_ratio = 16.0f / 9.0f;

			Camera.fov_vertical = 20.0f;

			Camera.LookFrom = v3f(-2.0f, 2.0f, 1.0f);
			Camera.LookAt = v3f(0.0f, 0.0f, -1.0f);
			Camera.Up = v3f(0.0f, 1.0f, 0.0f);

			Camera.defocus_angle = 0.0f;
			Camera.focus_dist = 1.0f;

			Camera.sample_count = 100;
			Camera.scatter_count = 50;
			
			Camera.init();
		} break;
		case '3':
		{
			World = world_list_final();

			Camera.image_width = 1200;
			Camera.aspect_ratio = 16.0f / 9.0f;

			Camera.fov_vertical = 20.0f;

			Camera.LookFrom = v3f(13.0f, 2.0f, 3.0f);
			Camera.LookAt = v3f(0.0f, 0.0f, 0.0f);
			Camera.Up = v3f(0.0f, 1.0f, 0.0f);

			Camera.defocus_angle = 0.6f;
			Camera.focus_dist = 10.0f;

			Camera.sample_count = 500;
			Camera.scatter_count = 50;
			
			Camera.init();
		} break;
	}

	Camera.image_render(&World, filename);

	return 0;
}

