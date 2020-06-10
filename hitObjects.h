#ifndef __SPHERE_H__
#define __SPHERE_H__
#include "ray.h"
#include <vector>
#include "utility.h"

class Material;

/*A struct of store all the rays hit properties*/
struct hit_record {
    vec3        normal;
    double      t;
    double      u;
    double      v;
    vec3        p;
    const Material*   mat;
};

class texture {
    public:
        virtual vec3 value(double u, double v, const vec3& p) = 0;
};

class constant_texture : public texture {
    private:
        vec3 color;
    public:
        constant_texture(const vec3& c_);
        vec3 value(double u, double v, const vec3& p) override;
};

class checker_board : public texture {
    private:
        texture* o; texture* e;
        double size;                            // Lower number means bigger in size
    public:
        checker_board(texture* o_, texture* e_, double s_);
        vec3 value(double u, double v, const vec3& p) override;
};

/*The Abstract class responsible of all of the diff materials*/
class Material {
    public:
        virtual bool scatter(const Ray& r, const hit_record& rec,
                             vec3& attn, Ray& scattered) const = 0;
        virtual vec3 emmited(double u, double v, const vec3& p) const { return vec3(0); }
};

class diffuse_light : public Material {
    private:
        texture* color;
    public:
        diffuse_light(texture* a);
        bool scatter(const Ray& r, const hit_record& rec,
                             vec3& attn, Ray& scattered) const override { return false; }
        
        vec3 emmited(double u, double v, const vec3& p) const override;
};

/*The material that don't reflect and scatter the light randomly*/
class lambertian : public Material {
    private:
        texture* albedo;
    public:
        lambertian(texture * a);
        bool scatter(const Ray& r, const hit_record& rec,
                      vec3& attn, Ray& scattered) const override;
};

/*Scatter the light uniformly basically reflection*/
class Metal : public Material {
    private:
        texture* albedo;
        double fuzz;
    public:
        Metal(texture* a, double f);
        bool scatter(const Ray& r, const hit_record& rec,
                      vec3& attn, Ray& scattered) const override;
};

/*Refact and Reflect light like water*/
class refraction: public Material {
    private:
        double refraction_index;
        bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted) const;
        double schlick(double cosine, double ri) const; //Returns the material's reflective angle
    public:
        refraction(double ri);
        bool scatter(const Ray& r, const hit_record& rec,
                      vec3& attn, Ray& scattered) const override;
};

/*EveryThing that Light can hit on*/
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

/*To store all the hit objects in the scene*/
class HitObjectList : public hitable {
    private:
        std::vector<hitable*> hit_list;
    public:
        template<class ...Args>
        HitObjectList(Args... args) : hit_list { std::forward<Args>(args)... } {}

        HitObjectList(const HitObjectList& other) { hit_list = other.hit_list; }

        HitObjectList() {}

        void addObject(hitable* h) { hit_list.push_back(h); }

        bool hit_test(const Ray& r, double tmin, double tmax, hit_record& rec) override
        {
            bool hitAnyThing = false;
            double closest = tmax;
            for(hitable* s: hit_list)
            {
                bool has_hit = s -> hit_test(r, tmin, closest, rec);
                if(has_hit) closest = rec.t;
                hitAnyThing |= has_hit;
            }

            return hitAnyThing;
        }
};

#endif