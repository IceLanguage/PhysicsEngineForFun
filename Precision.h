#ifndef _PhysicsEngineForFun_Precision_
#define _PhysicsEngineForFun_Precision_

#include <float.h>
#include <math.h>

namespace PhysicsEngineForFun
{
	#if 0
		#define SINGLE_PRECISION
		typedef float real;
		#define REAL_MAX FLT_MAX
		#define real_sqrt sqrtf
		#define real_abs fabsf
		#define real_sin sinf
		#define real_cos cosf
		#define real_exp expf
		#define real_pow powf
		#define real_fmod fmodf
		#define real_epsilon FLT_EPSILON
		#define R_PI 3.14159f
	#else
		#define DOUBLE_PRECISION
		typedef double real;
		#define REAL_MAX DBL_MAX
		#define real_sqrt sqrt
		#define real_abs fabs
		#define real_sin sin
		#define real_cos cos
		#define real_exp exp
		#define real_pow pow
		#define real_fmod fmod
		#define real_epsilon DBL_EPSILON
		#define R_PI 3.14159265358979
	#endif
}

#endif
