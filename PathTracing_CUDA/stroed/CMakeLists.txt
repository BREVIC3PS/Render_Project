cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)
set(OpenMP_CXX_FLAGS -openmp)
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    find_package(OpenMP REQUIRED)

    
    include(CMakePrintHelpers)
    cmake_print_properties(
    TARGETS
    OpenMP::OpenMP_CXX
    PROPERTIES
    INTERFACE_COMPILE_OPTIONS
    INTERFACE_INCLUDE_DIRECTORIES
    INTERFACE_LINK_LIBRARIES
    )

endif()

add_executable(RayTracing main.cpp Object.hpp Vector.cpp Vector.hpp Sphere.hpp global.hpp Triangle.hpp Scene.cpp
        Scene.hpp Light.hpp AreaLight.hpp BVH.cpp BVH.hpp Bounds3.hpp Ray.hpp Material.hpp Intersection.hpp
        Renderer.cpp Renderer.hpp)

target_link_libraries(RayTracing
      PUBLIC
        OpenMP::OpenMP_CXX
      )