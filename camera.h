#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "ray.h"
#include "iostream"

class Camera{
    private:
        double half_h, half_w;
        vec3 horizontal, vertical, lower_left;
        vec3 origin;

    public:
        Camera(vec3 lookfrom, vec3 lookat,
               vec3 vup, double fov_, 
               double aspect_);
        Ray get_ray(double u, double v); // Returns the ray for that pixel

};

#endif
