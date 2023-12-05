#if !defined(RAYTRACER_MATH_H)

#include <random>

#define SQUARE(x) ((x) * (x))
#define ABS(x) (((x) < 0) ? -(x) : (x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define PI32 3.14159265358979f

#define DEG_TO_RAD(deg) (((deg) * PI32) / 180.f)

const f32 inf = std::numeric_limits<f32>::infinity();

class interval
{
	public:
		f32 min, max;

		interval() {}
		interval(f32 _min, f32 _max);
		b32 contains(f32 x) const;
		b32 surrounds(f32 x) const;

};

interval::interval(f32 _min, f32 _max)
{
	min = _min;
	max = _max;
}

static const interval empty(inf, -inf);
static const interval universe(-inf, inf);


b32
interval::contains(f32 x) const
{
	b32 Result = false;

	Result = ((min <= x) && (x <= max));

	return(Result);
}

b32
interval::surrounds(f32 x) const
{
	// NOTE(JE): This check whether or not x is an interior point of the
	// interval.
	b32 Result = false;

	Result = ((min < x) && (x < max));

	return(Result);
}

class v3f
{
	public:
		v3f() {}
		v3f(f32 e0, f32 e1, f32 e2) {e[0] = e0; e[1] = e1; e[2] = e2;}

		inline f32 x() const {return e[0]; }
		inline f32 y() const {return e[1]; }
		inline f32 z() const {return e[2]; }
		inline f32 r() const {return e[0]; }
		inline f32 g() const {return e[1]; }
		inline f32 b() const {return e[2]; }

		inline const v3f& operator+() const {return *this; }
		inline v3f operator-() const {return v3f(-e[0], -e[1], -e[2]);}
		inline f32 operator[](int i) const {return e[i]; }
		inline f32& operator[](int i) {return e[i]; }

		inline v3f& operator+=(const v3f &V);
		inline v3f& operator-=(const v3f &V);
		inline v3f& operator*=(const v3f &V);
		inline v3f& operator/=(const v3f &V);
		inline v3f& operator*=(const f32 t);
		inline v3f& operator/=(const f32 t);

		inline f32 length() const {return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
		inline f32 length_squared() const {return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];}
		inline void normalize();

		f32 e[3];
};

inline std::istream&
operator>>(std::istream &is, v3f &V)
{
	is >> V.e[0] >> V.e[1] >> V.e[2];
	return is;
}

inline std::ostream&
operator<<(std::ostream &os, const v3f &V)
{
	os << V.e[0] << " " << V.e[1] << " " << V.e[2];
	return os;
}

inline void
v3f::normalize()
{
	f32 k = (1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]));
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

inline v3f
operator+(const v3f &U, const v3f &V)
{
	return v3f(U.e[0] + V.e[0], U.e[1] + V.e[1], U.e[2] + V.e[2]);
}

inline v3f
operator-(const v3f &U, const v3f &V)
{
	return v3f(U.e[0] - V.e[0], U.e[1] - V.e[1], U.e[2] - V.e[2]);
}

inline v3f
operator*(f32 t, const v3f &V)
{
	return v3f(t * V.e[0], t * V.e[1], t * V.e[2]);
}

inline v3f
operator*(const v3f &V, f32 t)
{
	return(t * V);
}

#if 0
inline v3f
operator*(const v3f &U, const v3f &V)
{
	return(v3f(U.e[0] * V.e[0], U.e[1] * V.e[1], U.e[2] * V.e[2]));
}
#endif

inline v3f&
v3f::operator+=(const v3f &V)
{
	e[0] += V.e[0];
	e[1] += V.e[1];
	e[2] += V.e[2];
	return *this;
}

inline v3f&
v3f::operator*=(const v3f &V)
{
	e[0] *= V.e[0];
	e[1] *= V.e[1];
	e[2] *= V.e[2];
	return *this;
}

inline v3f&
v3f::operator-=(const v3f& V)
{
	e[0] -= V.e[0];
	e[1] -= V.e[1];
	e[2] -= V.e[2];
	return *this;
}

inline v3f&
v3f::operator*=(const f32 t)
{
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline f32
dot(const v3f &U, const v3f &V)
{
	return(U.e[0] * V.e[0] + U.e[1] * V.e[1] + U.e[2] * V.e[2]);
}

inline v3f
cross(const v3f &U, const v3f &V)
{
	return v3f(U.e[1] * V.e[2] - U.e[2] * V.e[1],
			   U.e[2] * V.e[0] - U.e[0] * V.e[2],
			   U.e[0] * V.e[1] - U.e[1] * V.e[0]);
}

inline v3f
unit_vector(v3f V)
{
	return (1.0f / V.length()) * V;
}



inline v3f
reflect(const v3f &V, const v3f &N)
{
	v3f Result = {};

	Result = V - 2 * dot(V, N) * N;

	return(Result);
}

inline v3f
refract(const v3f & V, const v3f &Normal, f32 etai_over_etat)
{
	v3f Result;

	f32 cos_theta = MIN(dot(-V, Normal), 1.0f);
	v3f Perp = etai_over_etat * (V + cos_theta * Normal);
	v3f Parallel = -sqrt(ABS(1.0f - Perp.length_squared())) * Normal;
	
	Result = Perp + Parallel;

	return(Result);
}

inline f32
reflectance(f32 cosine, f32 ref_idx)
{
	f32 Result = 0.0f;

	f32 r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 = SQUARE(r0);

	Result = r0 + (1.0f - r0) * pow((1.0f - cosine), 5);

	return(Result);
}

inline f32
random_unit()
{
	static std::uniform_real_distribution<f32> distribution(0.0f, 1.0f);
	static std::mt19937 Generator;

	return(distribution(Generator));
}

#if 0
inline f32
random_unit()
{
	// NOTE(Justin): Generates rand real in [0, 1).
	f32 Result = ((f32)rand() / ((f32)RAND_MAX + 1));
	return(Result);
}
#endif


inline f32
random_interval(f32 min, f32 max)
{
	// NOTE(Justin): Generates rand real in [min, max).
	f32 Result = 0.0f;

	Result = min + (max - min) * random_unit();

	return(Result);
}

inline v3f
v3f_rand()
{
	v3f Result = v3f(random_unit(), random_unit(), random_unit());

	return(Result);
}

inline v3f
v3f_rand_in_unit_sphere()
{
	v3f Result = {};
	do
	{
		Result = 2.0f * v3f(random_unit(), random_unit(), random_unit()) - v3f(1.0f, 1.0f, 1.0f);
	} while(Result.length_squared() >= 1.0f);

	return(Result);
}

inline v3f
v3f_rand_unit_vector()
{
	v3f Result = unit_vector(v3f_rand_in_unit_sphere());

	return(Result);
}

inline v3f
v3f_rand_in_hemisphere(const v3f &Normal)
{
	v3f Result = v3f_rand_unit_vector();

	if((dot(Result, Normal) <= 0.0f))
	{
		Result = -1.0f * Result;
	}

	return(Result);
}

inline f32
clamp(f32 a, f32 x, f32 b)
{
	f32 Result = x;
	if(x < a)
	{
		Result = a;
	}
	else if(x > b)
	{
		Result = b;
	}

	return(Result);
}

inline v3f
color_linear_to_gamma2(v3f Color)
{
	v3f Result = Color;

	Result[0] = sqrt(Result[0]);
	Result[1] = sqrt(Result[1]);
	Result[2] = sqrt(Result[2]);

	return(Result);
}

inline b32
v3f_near_zero(v3f V)
{
	b32 Result = false;

	f32 epsilon = 1e-8;
	if((ABS(V[0]) < epsilon) &&
	   (ABS(V[1]) < epsilon) &&
	   (ABS(V[2]) < epsilon))
	{
		Result = true;
	}
	return(Result);
}


inline v3f
v3f_hadamard(v3f U, v3f V)
{
	v3f Result;

	Result[0] = U[0] * V[0];
	Result[1] = U[1] * V[1];
	Result[2] = U[2] * V[2];

	return(Result);
}

inline v3f
v3f_rand_in_unit_disk()
{
	v3f Result;
	do
	{
		Result = v3f(random_interval(-1.0f, 1.0f), random_interval(-1.0f, 1.0f), 0.0f);
	} while(Result.length_squared() >= 1.0f);

	return(Result);
}

#define RAYTRACER_MATH_H 
#endif
