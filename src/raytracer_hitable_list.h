#if !defined(RAYTRACER_HITABLE_LIST_H)

#include <raytracer_hitable.h>

class object_list: public object
{
	public:
		object_list() {}
		void add(object *Obj);
		virtual b32 ray_intersect(const ray &Ray, interval RayInterval, intersect_record &IntersectRecord) const;

		std::vector<object *> Objects;
};


void
object_list::add(object *Obj)
{
	Objects.push_back(Obj);
}

b32
object_list::ray_intersect(const ray &Ray, interval RayInterval, intersect_record &IntersectRecord) const
{
	b32 Result = false;

	intersect_record TempRecord = {};
	f32 t_closest = RayInterval.max;
	for(u32 list_index = 0; list_index < Objects.size(); list_index++)
	{
		if(Objects[list_index]->ray_intersect(Ray, interval(RayInterval.min, t_closest), TempRecord))
		{
			t_closest = TempRecord.t;
			IntersectRecord = TempRecord;
			Result = true;
		}
	}
	return(Result);
}

#define RAYTRACER_HITABLE_LIST_H
#endif
