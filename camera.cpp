#include "camera.h"
#include <math.h>

Camera::Camera(vec3 lookfrom, vec3 lookat,
               vec3 vup, double fov_, 
               double aspect_)
{
    vec3 u, v, w;
    double fov_rad = fov_ * 3.14159 / 180;

    half_h = tan(fov_rad / 2);
    half_w = aspect_ * half_h;

    origin = lookfrom;

    w = (lookfrom - lookat).normalize();
    u = vec3::cross(vup,w).normalize();
    v = vec3::cross(w, u);

    lower_left = origin - half_w * u - half_h * v - w;

    horizontal = 2 * half_w * u;
    vertical = 2 * half_h * v;
}

Ray Camera::get_ray(double u, double v)
{
    return Ray(origin, lower_left + u * horizontal + v * vertical - origin);
}