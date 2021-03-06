#include "hitObjects.h"
#include <math.h>
#include <iostream>

bool Sphere::hit_test(const Ray& r, double tmin, double tmax, hit_record& h) {
    vec3 oc = r.getOrigin() - center;
    double a = vec3::dot(r.getDirection(), r.getDirection());
    double b = 2.0 * vec3::dot(oc, r.getDirection());
    double c = vec3::dot(oc, oc) - Radius * Radius;


    float d = b * b - 4.0 * a * c;

    if(d > 0)
    {
        double t1 = ( -b - sqrtf(d)) / (2.0 * a);
        double t2 = ( -b + sqrtf(d)) / (2.0 * a);

        if(t1 > tmin && t1 < tmax) h.t = t1;
        else if(t2 > tmin && t2 < tmax) h.t = t2;
        else return false;

        h.p = r.point_at_parameter(h.t);
        h.normal = (h.p - center) * (1 / Radius);
        h.mat = mat;

        double phi = atan2(h.p.z, h.p.x);
        double theta = asin(h.p.y);

        h.u = 1 - (phi + (22.0 / 7.0)) / (2 * (22.0 / 7.0));
        h.v = (theta + (22.0 / 7.0) / 2) / (22.0 / 7.0);
    }

    return d > 0.0;
}

constant_texture::constant_texture(const vec3& c_)
    :color(c_)
    {}

vec3 constant_texture::value(double u, double v, const vec3& p)
{
    return color;
}

checker_board::checker_board(texture * o_, texture * e_, double s_)
    :o(o_), e(e_), size(s_)
    {}

vec3 checker_board::value(double u, double v, const vec3& p)
{
    double d = sin(size * p.x) * sin(size * p.y) * sin(size * p.z);

    if(d < 0)
        return o->value(u, v, p);
    return e->value(u, v, p);
}

Sphere::Sphere(vec3 c, double R, Material* mat_)
    :center(c), Radius(R), mat(mat_)
    {}

diffuse_light::diffuse_light(texture* a)
    :color(a)
    {}

vec3 diffuse_light::emmited(double u, double v, const vec3& p) const
{
    return color -> value(u, v, p);
}

lambertian::lambertian(texture * a)
    :albedo(a)
    {}

bool lambertian::scatter(const Ray& r, const hit_record& rec,
                          vec3& attn, Ray& scattered) const
{
    vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    attn = albedo -> value(0, 0, rec.p);
    scattered = Ray(rec.p, target - rec.p);
    return true;
}

Metal::Metal(texture* a, double f)
    :albedo(a), fuzz(f)
    {}

bool Metal::scatter(const Ray& r, const hit_record& rec,
                    vec3& attn, Ray& scattered) const
{
    attn = albedo -> value(0, 0, rec.p);
    vec3 reflected = r.getDirection() - 2 * vec3::dot(r.getDirection(), 
                                                      rec.normal) * rec.normal;

    scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
    return (vec3::dot(scattered.getDirection(), rec.normal));
}

refraction::refraction(double ri)
    : refraction_index(ri)
    {}

bool refraction::refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted) const
{
    vec3 uv = v;
    uv = uv.normalize();
    double dt = vec3::dot(uv, n);
    double d = 1.0 - ni_over_nt* ni_over_nt * (1 - dt * dt);

    if(d > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) -
                    n * sqrt(d);
        return true;
    }

    return false;
}

double refraction::schlick(double cosine, double ri) const
{
    double r0 = (1 - ri) / (1 + ri);
    r0 = r0 * r0;

    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refraction::scatter(const Ray& r, const hit_record& rec,
                      vec3& attn, Ray& scattered) const
{
    vec3 outward_normal;
    vec3 reflected = r.getDirection() - 2 * vec3::dot(r.getDirection(), rec.normal) * rec.normal;

    float ni_over_nt;

    attn = vec3(1.0, 1.0, 1.0);
    vec3 refracted;

    double reflect_prob;
    double cosine;

    if(vec3::dot(r.getDirection(), rec.normal) > 0)
    {
        outward_normal = vec3(0) -rec.normal;
        ni_over_nt = refraction_index;

        cosine = refraction_index * vec3::dot(r.getDirection(),
                                               rec.normal) / 
                 r.getDirection().length();
    }

    else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / refraction_index;
        cosine = -vec3::dot(r.getDirection(), rec.normal) /
                  r.getDirection().length();
    }

    if(refract(r.getDirection(), outward_normal, ni_over_nt, refracted))
    {
        reflect_prob = schlick(cosine, refraction_index);
    }

    else 
    {
        reflect_prob = 1.0;
    }

    if(randf() < reflect_prob)
        scattered = Ray(rec.p, reflected);
    else 
        scattered = Ray(rec.p, refracted);

    return true;

}