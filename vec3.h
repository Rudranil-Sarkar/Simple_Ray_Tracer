#ifndef __VEC3_H__
#define __VEC3_H__
#include <iostream>

class vec3 {
    public:
        double x, y, z;
        
        vec3();
        vec3(double value);
        vec3(double x_, double y_, double z_);
        vec3(const vec3& other);

        vec3 operator+(const vec3& other);
        vec3 operator-(const vec3& other);
        vec3 operator*(double t);
        vec3 operator/(double t);

        static double dot(const vec3& v1, const vec3& v2);
        double length();
        double squared_length();
        vec3 normalize();
        static vec3 cross(const vec3& v1, const vec3& v2);

};

std::ostream& operator <<(std::ostream& stream, const vec3& v);

vec3 operator*(const vec3& v1, const vec3& v2);
vec3 operator+(const vec3& v1, const vec3& v2);

#endif