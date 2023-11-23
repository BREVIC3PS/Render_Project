
#include <fstream>
#include <iostream>
#include "Scene.hpp"
#include "Renderer.hpp"
#include <omp.h>

inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.000001;

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene, int spp)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(278, 273, -800);
    int m = 0;
    int tid;
    // change the spp value to change sample ammount
    std::cout << "SPP: " << spp << "\n";
    // each thread is responsible for one pixel? -> No! it is impossible to create so many threads
    // each thread is responsible for pixel_number/thread_number pixels
    int pixel_number = framebuffer.size();
    int threadsNumber = 1024;
    int blockSize = pixel_number / threadsNumber;
    omp_set_num_threads(threadsNumber);
    std::cout << "Number of threads: " << threadsNumber << std::endl;
#pragma omp parallel private(tid)
    {
        tid = omp_get_thread_num();
        for (int pixel = tid * blockSize; pixel < (tid + 1) * blockSize; pixel++ )
        {
            int j = pixel / scene.width;
            int i = pixel % scene.width;
            // generate primary ray direction
            float x = (2 * (i + 0.5) / (float)scene.width - 1) * imageAspectRatio * scale;
            float y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

            Vector3f dir = normalize(Vector3f(-x, y, 1));
            for (int k = 0; k < spp; k++) {
                framebuffer[pixel] += scene.castRay(Ray(eye_pos, dir), 0) / spp;
            }
        }
#pragma omp critical
        {   
            m++;
            UpdateProgress((float)m / (float)(threadsNumber));
        }
    }

    FILE* fp = fopen("binary.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", scene.width, scene.height);
    for (auto i = 0; i < scene.height * scene.width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].x), 0.6f));
        color[1] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].y), 0.6f));
        color[2] = (unsigned char)(255 * std::pow(clamp(0, 1, framebuffer[i].z), 0.6f));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp);    
}
