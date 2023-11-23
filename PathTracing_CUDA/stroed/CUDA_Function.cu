#pragma once
#include "Bounds3.hpp"


#include <vector>
#include "Vector.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "AreaLight.hpp"
#include "BVH.hpp"
#include "Ray.hpp"
#include "Scene.hpp"

__device__ bool Bounds3::IntersectP(const Ray& ray, const Vector3f& invDir,
                                const std::array<int, 3>& dirIsNeg) const
{
    // invDir: ray direction(x,y,z), invDir=(1.0/x,1.0/y,1.0/z), use this because Multiply is faster that Division
    // dirIsNeg: ray direction(x,y,z), dirIsNeg=[int(x>0),int(y>0),int(z>0)], use this to simplify your logic
    // TODO test if ray bound intersects

    Vector3f tmin = (pMin - ray.origin) * invDir;
    Vector3f tmax = (pMax - ray.origin) * invDir;
    if (dirIsNeg[0])
        std::swap(tmin.x, tmax.x);
    if (dirIsNeg[1])
        std::swap(tmin.y, tmax.y);
    if (dirIsNeg[2])
        std::swap(tmin.z, tmax.z);
    float texit = std::min(tmax.x, std::min(tmax.y, tmax.z));
    float tenter = std::max(tmin.x, std::max(tmin.y, tmin.z));
    return tenter <= texit && texit >= 0;
    

}







// Compute refraction direction using Snell's law
//
// We need to handle with care the two possible situations:
//
//    - When the ray is inside the object
//
//    - When the ray is outside.
//
// If the ray is outside, you need to make cosi positive cosi = -N.I
//
// If the ray is inside, you need to invert the refractive indices and negate the normal N
__device__  Vector3f Scene::refract(const Vector3f &I, const Vector3f &N, const float &ior) const
    {
        float cosi = clamp(-1, 1, dotProduct(I, N));
        float etai = 1, etat = ior;
        Vector3f n = N;
        if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
    }



    // Compute Fresnel equation
//
// \param I is the incident view direction
//
// \param N is the normal at the intersection point
//
// \param ior is the material refractive index
//
// \param[out] kr is the amount of light reflected
__device__  void Scene::fresnel(const Vector3f &I, const Vector3f &N, const float &ior, float &kr) const
    {
        float cosi = clamp(-1, 1, dotProduct(I, N));
        float etai = 1, etat = ior;
        if (cosi > 0) {  std::swap(etai, etat); }
        // Compute sini using Snell's law
        float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
        // Total internal reflection
        if (sint >= 1) {
            kr = 1;
        }
        else {
            float cost = sqrtf(std::max(0.f, 1 - sint * sint));
            cosi = fabsf(cosi);
            float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (Rs * Rs + Rp * Rp) / 2;
        }
        // As a consequence of the conservation of energy, transmittance is given by:
        // kt = 1 - kr;
    }


Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

__device__ void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

__device__ bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
__device__ Vector3f Scene::castRay(const Ray& ray, int depth) const
{
    // TO DO Implement Path Tracing Algorithm here
    Vector3f hitColor = this->backgroundColor;
    Intersection shade_point_inter = Scene::intersect(ray);
    if (shade_point_inter.happened)
    {
        Vector3f p = shade_point_inter.coords;
        Vector3f wo = ray.direction;
        Vector3f N = shade_point_inter.normal;
        Vector3f L_dir(0), L_indir(0);
        Vector3f p_deviation = (dotProduct(ray.direction, N) < 0) ?
            p + N * EPSILON :
            p - N * EPSILON;

        switch (shade_point_inter.m->getType())
        {
        case MIRROR:
        {
            //Test Russian Roulette with probability RussianRouolette
            float ksi = get_random_float();
            if (ksi < RussianRoulette)
            {
                //wi=sample(wo,N)
                Vector3f wi = normalize(shade_point_inter.m->sample(wo, N));
                //Trace a ray r(p,wi)
                Ray ray_pTowi(p_deviation, wi);
                //If ray r hit a object at q
                Intersection bounce_point_inter = Scene::intersect(ray_pTowi);
                if (bounce_point_inter.happened)
                {
                    float pdf = shade_point_inter.m->pdf(wo, wi, N);
                    if (pdf > EPSILON)
                        L_indir = castRay(ray_pTowi, depth + 1) * shade_point_inter.m->eval(wo, wi, N) * dotProduct(wi, N) / (pdf * RussianRoulette);
                }
            }
            break;
        }
        default:
        {
            //sampleLight(inter,pdf_light)
            Intersection light_point_inter;
            float pdf_light;
            sampleLight(light_point_inter, pdf_light);
            //Get x,ws,NN,emit from inter
            Vector3f x = light_point_inter.coords;
            Vector3f ws = normalize(x - p);
            Vector3f NN = light_point_inter.normal;
            Vector3f emit = light_point_inter.emit;
            float distance_pTox = (x - p).norm();
            //Shoot a ray from p to x
            Ray ray_pTox(p_deviation, ws);
            //If the ray is not blocked in the middleff
            Intersection blocked_point_inter = Scene::intersect(ray_pTox);
            if (abs(distance_pTox - blocked_point_inter.distance < 0.01))
            {
                L_dir = emit * shade_point_inter.m->eval(wo, ws, N) * dotProduct(ws, N) * dotProduct(-ws, NN) / (distance_pTox * distance_pTox * pdf_light);
            }
            //Test Russian Roulette with probability RussianRouolette
            float ksi = get_random_float();
            if (ksi < RussianRoulette)
            {
                //wi=sample(wo,N)
                Vector3f wi = normalize(shade_point_inter.m->sample(wo, N));
                //Trace a ray r(p,wi)
                Ray ray_pTowi(p_deviation, wi);
                //If ray r hit a non-emitting object at q
                Intersection bounce_point_inter = Scene::intersect(ray_pTowi);
                if (bounce_point_inter.happened && !bounce_point_inter.m->hasEmission())
                {
                    float pdf = shade_point_inter.m->pdf(wo, wi, N);
                    if (pdf > EPSILON)
                        L_indir = castRay(ray_pTowi, depth + 1) * shade_point_inter.m->eval(wo, wi, N) * dotProduct(wi, N) / (pdf * RussianRoulette);
                }
            }
            break;
        }
        }
        hitColor = shade_point_inter.m->getEmission() + L_dir + L_indir;
        hitColor.x = (clamp(0, 1, hitColor.x));
        hitColor.y = (clamp(0, 1, hitColor.y));
        hitColor.z = (clamp(0, 1, hitColor.z));

    }
    return hitColor;
}
