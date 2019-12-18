#include "ray.h"

Ray::Ray()
    :A(vec3()), B(vec3())
    {}

Ray::Ray(const Ray& other)
{
    A = other.A; B = other.B;
}

Ray::Ray(const vec3& origin, vec3 Direction)
    :A(origin), B(Direction.normalize())
    {}

vec3 Ray::getOrigin() const{ return A; }
vec3 Ray::getDirection() const{ return B; }

vec3 Ray::point_at_parameter(double t) const
{
    vec3 temp = A + B * t;
    return temp;
}