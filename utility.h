#ifndef __UTILITY_H__
#define __UTILITY_H__
#include <random>

static float map(float value, 
          float istart, 
          float istop, 
          float ostart, 
          float ostop) {
    return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

static float randf() {
	float random = ((float) rand()) / (float) RAND_MAX;
	return random;
}

static vec3 random_in_unit_sphere() {
	double z = randf() * 2.0 - 1.0;
	double a = randf() * 2.0 * 3.1415926;
	double r = sqrtf(1.0 - z * z);
	double x = r * cosf(a);
	double y = r * sinf(a);
	return vec3(x, y, z);
}


#endif