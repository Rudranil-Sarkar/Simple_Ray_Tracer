#include <iostream>
#include <time.h>
#include "FileHandler.h"
#include "camera.h"
#include "hitObjects.h"

/*Determine the color of the pixel by scattered ray*/
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

/*Self explantory*/
void Random_world_gen(HitObjectList& w) {
	w.addObject(new Sphere(vec3(0, -1000, -1), 1000, new lambertian(vec3(0.5, 0.5, 0.5))));

	for(int a = -11; a < 11; a++)
	{
		for(int b = -11; b < 11; b++)
		{
			float choose_mat = randf();
			vec3 center(a + 0.9 * randf(), 0.2, b + 0.9 * randf());
			if((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if(choose_mat < 0.8)
				{
					w.addObject(new Sphere(center, 0.2, new lambertian(vec3(randf() * randf(),
																			randf() * randf(),
																			randf() * randf()))));
				}

				else if(choose_mat < 0.95) {
					w.addObject(new Sphere(center, 0.2, new Metal(
						vec3(0.5 *(1 + randf()), 0.5 * (1 + randf()), 0.5 * (1 + randf())),
						0.5 * randf()
					)));
				}
				else
					w.addObject(new Sphere(center, 0.2, new refraction(1.5)));
				
			}
		}
	}

	w.addObject(new Sphere(vec3(0, 1, -1), 1.0, new refraction(1.5)));
	w.addObject(new Sphere(vec3(-4, 1, -1), 1.0, new lambertian(vec3(1, 0.0784313725490196, 0.5764705882352941))));
	w.addObject(new Sphere(vec3(4, 1, -1), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0)));
}

int main() {

	std::cout << "::Initializing FrameBuffer ---\n";
	Framebuffer f(1200, 600);

	std::cout << "::Setting up the camera ---\n";
	Camera c(vec3(10, 2, 3), vec3(0, 0, -1), vec3(0, 1, 0), 35, 2);
	srand(time(0));
	lambertian diffusePink(vec3(0.8, 0.3, 0.3));
	lambertian diffuseGreen(vec3(0.8, 0.8, 0.0));
	Metal Pink(vec3(0.8, 0.8, 0.2), 0.3);
	refraction M(1.5);

	std::cout << "::Making the scene\n";
	HitObjectList world; Random_world_gen(world);

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

			std::cout << "\r::Rendering: " << map(i , 0,  f.getHeight()  - 1, 0, 100) <<  "%" << std::flush;
		}
	}

	SaveFile::Save("out.ppm", f);

	return 0;
}
