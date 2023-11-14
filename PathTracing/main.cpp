#include "Renderer.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Vector.hpp"
#include "global.hpp"
#include <chrono>

// In the main function of the program, we create the scene (create objects and
// lights) as well as set the options for the render (image width and height,
// maximum recursion depth, field-of-view, etc.). We then call the render
// function().
int main(int argc, char** argv)
{

    // Change the definition here to change resolution
    int width = 256;
    int height = 256;
    Scene scene(width, height);
    std::cout << "Image Size: " << width * height << std::endl;
    Material* red = new Material(DIFFUSE, Vector3f(0.0f));
    red->Kd = Vector3f(0.63f, 0.065f, 0.05f);
    Material* green = new Material(DIFFUSE, Vector3f(0.0f));
    green->Kd = Vector3f(0.14f, 0.45f, 0.091f);
    Material* white = new Material(DIFFUSE, Vector3f(0.0f));
    white->Kd = Vector3f(0.725f, 0.71f, 0.68f);
    Material* light = new Material(DIFFUSE, (8.0f * Vector3f(0.747f+0.058f, 0.747f+0.258f, 0.747f) + 15.6f * Vector3f(0.740f+0.287f,0.740f+0.160f,0.740f) + 18.4f *Vector3f(0.737f+0.642f,0.737f+0.159f,0.737f)));
    light->Kd = Vector3f(0.65f);

    MeshTriangle floor("../models/cornellbox/floor.obj", white);
    //MeshTriangle shortbox("../models/cornellbox/shortbox.obj", white, Vector3f(0, 0,0 ), Vector3f(1, 1, 1));
    //MeshTriangle tallbox("../models/cornellbox/tallbox.obj", white);
    //MeshTriangle bunny("..//models/BMW M8 Competition Widebody/M8C.obj", white, Vector3f(250, 30,300 ),Vector3f(100, 100, 100), 45);
    MeshTriangle bunny("..//models/bunny/bunny.obj", white, Vector3f(250, 0, 220), Vector3f(2000, 2000, 2000),180);
    MeshTriangle left("../models/cornellbox/left.obj", red);
    MeshTriangle right("../models/cornellbox/right.obj", green);
    MeshTriangle light_("../models/cornellbox/light.obj", light);

    scene.Add(&floor);
    /*scene.Add(&shortbox);
    scene.Add(&tallbox);*/
    scene.Add(&bunny);
    scene.Add(&left);
    scene.Add(&right);
    scene.Add(&light_);

    char op = 'n';
    float distance = 0;

    while(op)
    {
        switch (op)
        {
        case 'x':
            bunny.move(Vector3f(distance, 0, 0));
            break;
        case 'y':
            bunny.move(Vector3f(0, distance, 0));
            break;
        case 'z':
            bunny.move(Vector3f(0, 0, distance));
            break;
        case 's':
            bunny.scale(Vector3f(distance, distance, distance));
        case 'r':
            bunny.rotate(distance);
        default:
            break;
        }
        scene.buildBVH();
        Renderer r;


        auto start = std::chrono::system_clock::now();
        r.Render(scene);
        auto stop = std::chrono::system_clock::now();

        std::cout << "Render complete: \n";
        std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
        std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
        std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";
        std::cout << bunny.bounding_box.pMin <<std::endl<< bunny.bounding_box.pMax << '\n';
        std::cin >> op >> distance;
    }

    return 0;
}