#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/numeric/odeint.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "lib/variables.h"
#include "lib/utils.h"
#include "lib/vec3.h"
#include "lib/ray.h"
#include "lib/camera.h"
#include "lib/evolution.h"
#include "lib/color.h"

typedef std::vector< double > state_type;
using namespace boost::numeric::odeint;


color GetPixel(stbi_uc* image, size_t imageWidth, size_t imageHeight, int x, int y) {
	color pixel_color(0, 0, 0);
	pixel_color[0] = image[3 * (y * imageWidth + x) + 0] / 256.;
	pixel_color[1] = image[3 * (y * imageWidth + x) + 1] / 256.;
	pixel_color[2] = image[3 * (y * imageWidth + x) + 2] / 256.;
	return pixel_color;
}

color GetValue(state_type& x) {
	color pixel_color;
	double phi = x[2];
	double theta = x[1];

	while (phi < 0) {
		phi = phi + 2 * pi;
	}

	while (theta < 0) {
		theta = theta + pi;
	}
	
	while (phi > 2 * pi) {
		phi = phi - 2 * pi;
	}

	while (theta > pi) {
		theta = theta - pi;
	}
	double u = phi / (2 * pi);
	double v = theta / pi;

	u = clamp(u, 0.0, 1.0);
	v = clamp(v, 0.0, 1.0);

	if (x[0] > 0) {
		// Gargantua
		auto i = static_cast<int>(u * width_gargantua);
		auto j = static_cast<int>(v * height_gargantua);

		if (i >= width_gargantua)  i = width_gargantua - 1;
		if (j >= height_gargantua) j = height_gargantua - 1;

		pixel_color = GetPixel(gargantuaImageData, width_gargantua, height_gargantua, i,j);
	}
	else {
		// Saturn
		auto i = static_cast<int>(u * width_saturn);
		auto j = static_cast<int>(v * height_saturn);

		if (i >= width_saturn)  i = width_saturn - 1;
		if (j >= height_saturn) j = height_saturn - 1;

		pixel_color = GetPixel(saturnImageData, width_saturn, height_saturn, i,j);
	}

	return pixel_color;
}

int main()
{
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	
	double focal_distance = par_rho;

	// Coordinates
	double l, theta, phi, p_l, p_theta, p_phi;
	double nl, ntheta, nphi;
	double r0;

	// Camera
	point3 camera_lookfrom(par_lc, 0, 0); // (l_c, y, z)
	point3 screen(par_lc - focal_distance, 0, 0);
	camera cam(camera_lookfrom, screen, 45, aspect_ratio);
	int pixel_r = 0, pixel_b = 0, pixel_g = 0;

	state_type x(5);
	
	saturnImageData = stbi_load("D:\\Developing\\Wormhole\\Resources\\berna.jpg", &width_saturn, &height_saturn, &components, STBI_rgb);
	gargantuaImageData = stbi_load("D:\\Developing\\Wormhole\\Resources\\pilar.jpg", &width_gargantua, &height_gargantua, &components, STBI_rgb);
	if (saturnImageData == NULL || gargantuaImageData == NULL) {
		std::cout << stbi_failure_reason();
	}

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			color pixel_color(0, 0, 0);
			for (int s = 0; s < n_samples; s++) {
				auto u = double(i + random_double()) / double(image_width - 1);
				auto v = double(j + random_double()) / double(image_height - 1);
				vec3 coord_0 = cam.get_initial_point(u, v);
				coord_0.to_polar_coords(&l, &theta, &phi);

				nl = sin(theta) * cos(phi);
				ntheta = sin(theta) * sin(phi);
				nphi = -cos(theta);

				r0 = evolution::r(l);

				p_l = nl;
				p_theta = r0 * ntheta;
				p_phi = r0 * sin(theta) * nphi;

				auto b = p_phi;
				auto B2 = p_theta * p_theta + p_phi * p_phi / (sin(theta) * sin(theta));

				evolution evo(b, B2);

				//State initialization
				x[0] = l;
				x[1] = theta;
				x[2] = phi;
				x[3] = p_l;
				x[4] = p_theta;

				//size_t steps = integrate(evo, x, 0.0, -200.0, -0.1);
				pixel_color += GetValue(x);
			}
			
			write_color(std::cout, pixel_color, n_samples);
		}
	}
	return 0;
}
