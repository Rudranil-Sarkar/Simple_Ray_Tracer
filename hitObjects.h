#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "ray.h"
#include <vector>
#include "utility.h"

class Material;

struct hit_record {
    vec3        normal;
    double      t;
    vec3        p;
    const Material*   mat;
};

class Material {
    public:
        virtual bool scatter(const Ray& r, const hit_record& rec,
                             vec3& attn, Ray& scattered) const = 0;
};

class lambertian : public Material {
    private:
        vec3 albedo;
    public:
        lambertian(const vec3& a);
        bool scatter(const Ray& r, const hit_record& rec,
                      vec3& attn, Ray& scattered) const override;
};

class Metal : public Material {
    private:
        vec3 albedo;
        double fuzz;
    public:
        Metal(const vec3& a, double f);
        bool scatter(const Ray& r, const hit_record& rec,
                      vec3& attn, Ray& scattered) const override;
};

class refraction: public Material {
    private:
        double refraction_index;
        bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted) const;
        double schlick(double cosine, double ri) const;
    public:
        refraction(double ri);
        bool scatter(const Ray& r, const hit_record& rec,
                      vec3& attn, Ray& scattered) const override;
};

class hitable {
    public:
        virtual bool hit_test(const Ray& r, double tmin, double tmax, hit_record & h) = 0;
};

class Sphere : public hitable{
    public:
        vec3 center;
        double Radius;
        Material* mat;

        Sphere(vec3 c, double R, Material* mat_);
        bool hit_test(const Ray& r, double tmin, double tmax, hit_record& h) override;
};

class HitObjectList : public hitable {
    private:
        std::vector<Sphere> hit_list;
    public:
        template<class ...Args>
        HitObjectList(Args... args) : hit_list { std::forward<Args>(args)... } {}
        bool hit_test(const Ray& r, double tmin, double tmax, hit_record& rec) override
        {
            bool hitAnyThing = false;
            double closest = tmax;
            for(Sphere& s: hit_list)
            {
                bool has_hit = s.hit_test(r, tmin, closest, rec);
                if(has_hit) closest = rec.t;
                hitAnyThing |= has_hit;
            }

            return hitAnyThing;
        }
};

#endif