#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

class camera {
    public:
        camera(point3 lookfrom, point3 lookat, double vfov, double aspect_ratio) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 10.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;
            auto focal_length = 0.01;

            offset = vec3(0, 0, 0);

            origin = lookfrom;
            horizontal = vec3(0, viewport_width, 0);
            vertical = vec3(0, 0, viewport_height);
            lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(focal_length, 0, 0);
        }

        ray get_ray(double s, double t) const {
            //vec3 rd = lens_radius * random_in_unit_disk();
            //vec3 offset = u*rd.x() + v*rd.y();
            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

        vec3 get_initial_point(double s, double t) const {
            return lower_left_corner + s * horizontal + t * vertical - offset;
        }

        void get_final_point(double horizontal_cs, double vertical_cs, double* s, double* t) {
            // X axis is in l direction 
            *s = (horizontal_cs - lower_left_corner[1] + offset[1]) / horizontal[1];
            *t = (vertical_cs - lower_left_corner[2] + offset[2]) / vertical[2];
        }
    
    public:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 offset;
};

#endif