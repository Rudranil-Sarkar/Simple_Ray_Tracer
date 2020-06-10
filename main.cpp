#include <iostream>
#include <time.h>
#include <thread>
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
		vec3 emmited = hit.mat -> emmited(hit.u, hit.v, hit.p);
		if(bounce < 100 && hit.mat -> scatter(r, hit, attn, scattered))
			return emmited + attn * color(scattered, world, bounce + 1);
		else
		{
			return emmited;
		}
	}
	else
	{
			return vec3(0.05 / 10);
	}

	// double t = 0.5 * (r.getDirection().y + 1.0); 
	// return ((1.0 - t) * vec3(1.0, 1.0, 1.0) + 
	// 			   t * vec3(0.5, 0.7, 1.0));
}

/*Self explantory*/
void Random_world_gen(HitObjectList& w) {
	w.addObject(new Sphere(vec3(0, -1000, -1), 1000, new Metal(new constant_texture(vec3(0.5)), 0.5)));

	for(int a = -11; a < 11; a++)
	{
		for(int b = -11; b < 11; b++)
		{
			float choose_mat = randf();
			double d = randf() * 0.5;
			vec3 center(a + 0.8 * randf(), d, b + 0.8 * randf());
			if((center - vec3(4, d, 0)).length() > 0.9)
			{
				if(choose_mat < 0.79)
				{
					w.addObject(new Sphere(center, d, new lambertian(new constant_texture(vec3(randf() * randf(),
																			randf() * randf(),
																			randf() * randf())))));
				}

				else if(choose_mat < 0.95) {
					w.addObject(new Sphere(center, d, new Metal(new constant_texture
						(vec3(0.5 *(1 + randf()), 0.5 * (1 + randf()), 0.5 * (1 + randf()))),
						0.5 * randf()
					)));
				}

				else
					w.addObject(new Sphere(center, d, new refraction(1.5)));
				
			}
		}
	}

	w.addObject(new Sphere(vec3(-4, 1, -1), 1.0, new lambertian(new constant_texture(vec3(1, 0.0784313725490196, 0.5764705882352941)))));
	// w.addObject(new Sphere(vec3(0, 1, -1), 1.0, new refraction(1.5)));
	w.addObject(new Sphere(vec3(0, 1, -1), 1.0, new diffuse_light(new constant_texture(vec3(4)))));
	w.addObject(new Sphere(vec3(4, 1, -1), 1.0, new Metal(new constant_texture(vec3(0.7, 0.6, 0.5)), 0.0)));

	// Add some lights

	w.addObject(new Sphere(vec3(2, 0.5, 4), 0.5, new diffuse_light(new constant_texture(vec3(2)))));
	w.addObject(new Sphere(vec3(2.25, 0.6, -2.01), 0.6, new diffuse_light(new constant_texture(vec3(2)))));
	w.addObject(new Sphere(vec3(-3.28, 0.4, 5.3), 0.4, new diffuse_light(new constant_texture(vec3(2)))));

}

int main() {

	std::cout << "::Initializing FrameBuffer ---\n";
	Framebuffer f(400, 400);

	std::cout << "::Setting up the camera ---\n";
	Camera c(vec3(0, 12, 0), vec3(0, 0, -1), vec3(0, 1, 0), 65, f.getWidth() / f.getHeight());
	srand(time(0));

	std::cout << "::Making the scene ---\n";
	HitObjectList world; Random_world_gen(world);

	std::cout << "::Rendering ---\n";
	auto worker = [&f, &c, &world](size_t row_, size_t width_, size_t col_, size_t height)
	{
		for(int i = col_; i < col_ + height; i++)
		{
			for(int j = row_; j < row_ + width_; j++)
			{
				constexpr int ns = 12; // The anti-aliasing factor Set this to smaller value for quick rendering
				vec3 col;
				for(int k = 0; k < ns; k++)
				{
					double u = double(j + randf()) / double(f.getWidth());
					double v = double(i + randf()) / double(f.getHeight());

					col = col + color(c.get_ray(u, v),world, 0);
				}

				col = col / ns;

				col = vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

				f.SetPixel(j, i, col * 255);

			}
		}
	};

	std::thread t1([&worker]() { worker(0, 	 200, 0, 200); });
	std::thread t2([&worker]() { worker(200, 200 - 1, 0, 200); });
	std::thread t3([&worker]() { worker(0, 200, 200, 200 - 1); });
	std::thread t4([&worker]() { worker(200, 200 - 1, 200, 200 - 1); });

	t1.join(); t2.join(); t3.join(); t4.join();

	std::string s("out.ppm");
	SaveFile::Save(s.c_str(), f);
	return 0;
}
