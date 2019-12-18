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
	vec3 p;

	do{
		p = vec3(randf(), randf(), randf()) * 2.0 - vec3(1, 1, 1);
	} while(p.squared_length() >= 1.0);

	return p;
}


#endif