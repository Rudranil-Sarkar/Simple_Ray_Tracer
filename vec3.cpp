#include "vec3.h"
#include <math.h>

vec3::vec3() 
    :x(0), y(0), z(0)
    {}

vec3::vec3(double value)
    :x(value), y(value), z(value)
    {}

vec3::vec3(double x_, double y_, double z_)
    :x(x_), y(y_), z(z_)
    {}

vec3::vec3(const vec3& other)
{
   x = other.x; y = other.y; z = other.z;
}

vec3 vec3::operator+(const vec3& other)
{
    return vec3(x + other.x, y + other.y, z + other.z);
}

vec3 vec3::operator-(const vec3& other)
{
    return vec3(x - other.x, y - other.y, z - other.z);
}

vec3 vec3::operator*(double t)
{
    return vec3(x * t, y * t, z * t);
}

vec3 vec3::operator/(double t)
{
    return vec3(x / t, y / t, z / t);
}

double vec3::dot(const vec3& v1, const vec3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double vec3::squared_length() {
    return x * x + y * y + z * z;
}

double vec3::length() 
{
    return sqrt(x * x + y * y + z * z);
}

vec3 vec3::normalize()
{
    double temp = 1.0 / sqrt(x * x + y * y + z * z);

    return vec3(x * temp, y * temp, z * temp);

}

vec3 vec3::cross(const vec3& v1, const vec3& v2)
{
    return vec3((v1.y * v2.z - v1.z * v2.y), 
                (v1.z * v2.x - v1.x * v2.z),
                (v1.x * v2.y - v1.y * v2.x));
}

vec3 operator+(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x + v2.x,
                v1.y + v2.y,
                v1.z + v2.z);
}

vec3 operator*(const vec3& v1, const vec3& v2)
{
    return vec3(v1.x * v2.x, 
                v1.y * v2.y, 
                v1.z * v2.z);
}

std::ostream& operator<<(std::ostream& stream, const vec3& v)
{
    return stream << "x: " << v.x << " y: " << v.y << " z: " << v.z;
}