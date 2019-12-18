#ifndef __RAY_H__
#define __RAY_H__

#include "vec3.h"

class Ray {
    private:
        vec3 A, B;

    public:
        Ray();
        Ray(const Ray& other);
        Ray(const vec3& origin, vec3 direction);

        vec3 getOrigin() const;
        vec3 getDirection() const;
        vec3 point_at_parameter(double t) const;
};

#endif