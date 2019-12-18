#include <iostream>
#include <time.h>
#include "FileHandler.h"
#include "camera.h"
#include "hitObjects.h"

vec3 color(const Ray& r, HitObjectList& world, int bounce)
{
	hit_record hit;
	if(world.hit_test(r, 0.00001, MAXFLOAT, hit))
	{
		Ray scattered;
		vec3 attn;
		if(bounce < 100 && hit.mat -> scatter(r, hit, attn, scattered))
			return attn * color(scattered, world, bounce + 1);
		else
		{
			return vec3(0);
		}
		
	}

	double t = 0.5 * (r.getDirection().y + 1.0); 
	return ((1.0 - t) * vec3(1.0, 1.0, 1.0) + 
				   t * vec3(0.5, 0.7, 1.0));
}


int main() {

	Framebuffer f(800, 400);
	Camera c(vec3(-2, 2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 90, 2);
	srand(time(0));
	lambertian diffusePink(vec3(0.8, 0.3, 0.3));
	lambertian diffuseGreen(vec3(0.8, 0.8, 0.0));
	Metal Pink(vec3(0.8, 0.8, 0.2), 0.3);
	refraction M(1.5);
	HitObjectList world { Sphere(vec3(0, 0.0, -1), 0.5, &diffusePink),
						  Sphere(vec3(0, -100.5, -1), 100, &diffuseGreen),
						  Sphere(vec3(1, 0, -1), 0.5, &Pink),
						  Sphere(vec3(-1, 0, -1), 0.5, &M) };

	for(int i = 0; i < f.getHeight(); i++)
	{
		for(int j = 0; j < f.getWidth(); j++)
		{
			vec3 col;
			constexpr int ns = 100;
			for(int k = 0; k < ns; k++)
			{
				double u = double(j + randf()) / double(f.getWidth());
				double v = double(i + randf()) / double(f.getHeight());

				col = col + color(c.get_ray(u, v), world, 0);
			}

			col = col / ns;

			col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

			f.SetPixel(j, i, col * 255);

			std::cout << "\r::Progress: " << map(i , 0,  f.getHeight()  - 1, 0, 100) << std::flush;

		}
	}

	SaveFile::Save("out.ppm", f);

	return 0;
}
