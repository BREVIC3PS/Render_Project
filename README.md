# Render_Project

Working in Progress!

## Real-time rendering:
![image](https://github.com/BREVIC3PS/Render_Project/blob/master/Render_Project_Demos-original-original.gif)

## Path-Tracing
![image](https://github.com/BREVIC3PS/Render_Project/blob/master/PathTracing/Demo/1.png)

![image](https://github.com/BREVIC3PS/Render_Project/blob/master/PathTracing/Demo/2.png)

![image](https://github.com/BREVIC3PS/Render_Project/blob/master/PathTracing/Demo/result.jpg)

## Path-Tracing CUDA
# Overview
![image](https://github.com/BREVIC3PS/Render_Project/blob/master/Overview.png)
For more information, please check the presentation slide (Project_Presentation_KyuhyeonNam_KeWang_v0.1.pptx)

### Usage (For PathTracing):
```
cd PathTracing
mkdir build
cd build
cmake ..
make (or cmake --build . -j)
./RayTracing (Or manually run RayTracing.exe)
see the result -> binary.ppm
```

### Usage (For PathTracing_CUDA):
```
cd PathTracing_CUDA
mkdir build
cd build
cmake ..
make (or cmake --build . -j)
./RayTracing (Or manually run CUDA_Path_Tracing.exe)
see the result -> binary.ppm
```
