//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
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

bool Scene::trace(
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
Vector3f Scene::castRay(const Ray& ray, int depth) const
{
    //create variables to stores dir and indir light
    Vector3f dir = { 0.0,0.0,0.0 };
    Vector3f indir = { 0.0,0.0,0.0 };

    //1.Determine if there is an intersection: Does the light intersect with the object in the scene?
    Intersection inter = Scene::intersect(ray);
    //if no intersection
    if (!inter.happened) {
        //will return 0,0,0
        return dir;
    }
    //2.ray hit the light source: The rendering equation only counts the previous self-luminous terms, so it directly returns the self-luminous terms of the material
    if (inter.m->hasEmission()) {
        if (depth == 0) {//first time hitting light source
            return inter.m->getEmission();
        }
        else return dir;//bounced light is hitting light source, return 0,0,0

    }
    //3.ray hits the object: This is the time to start the next step after the pseudo-code(see documment)

    //The light source in the scene is sampled, and the sampling points light_pos and pdf_light are obtained
    Intersection light_pos;
    float pdf_light = 0.0f;
    sampleLight(light_pos, pdf_light);

    //3.1 calculate directional light

    //some prama of the object
    Vector3f p = inter.coords;
    Vector3f N = inter.normal.normalized();
    Vector3f wo = ray.direction;//from object to scene
    //some prama of the object
    Vector3f xx = light_pos.coords;
    Vector3f NN = light_pos.normal.normalized();
    Vector3f ws = (p - xx).normalized();//from light source to object
    float dis = (p - xx).norm();//distance between the object and the light
    float dis2 = dotProduct((p - xx), (p - xx));

    //Determine whether there is a block between the light source and the object:
    //Emits a ray in the direction of ws light source xx -> object p
    Ray light_to_obj(xx, ws);//Ray(orig,dir)
    Intersection light_to_scene = Scene::intersect(light_to_obj);
    //If dis>light_to_scene.distance indicates that there is a block, then the reverse condition can be given:
    if (light_to_scene.happened && (light_to_scene.distance - dis > -EPSILON)) {//no block
        //set some parameters
        Vector3f L_i = light_pos.emit;//light intensity
        Vector3f f_r = inter.m->eval(wo, -ws, N);//BRDF == material
        float cos_theta = dotProduct(-ws, N);//Object Angle
        float cos_theta_l = dotProduct(ws, NN);//Light Angle
        dir = L_i * f_r * cos_theta * cos_theta_l / dis2 / pdf_light;
    }

    //3.2 indirectional light

    //RussianRoulette
    //P_RR:RussianRoulette=0.8
    float ksi = get_random_float();//Randomly pick[0,1]
    if (ksi < RussianRoulette) {
        //Randomly set a direction wi
        Vector3f wi = inter.m->sample(wo, N).normalized();//In this case, the wi is not involved in the calculation and returns a random direction
        Ray r(p, wi);
        Intersection obj_to_scene = Scene::intersect(r);
        //Hit the object &&The object was not a light source
        if (obj_to_scene.happened && !obj_to_scene.m->hasEmission()) {
            Vector3f f_r = inter.m->eval(wo, wi, N);//wo is not involved in the calculation
            float cos_theta = dotProduct(wi, N);
            float pdf_hemi = inter.m->pdf(wo, wi, N);
            indir = castRay(r, depth + 1) * f_r * cos_theta / pdf_hemi / RussianRoulette;
        }
    }
    return dir + indir;
}