#include "omg/scene/Scene.h"

using namespace omg;

bool Scene::intersect(const Ray& ray, SurfaceInteraction* si) const {
    return _aggregate->intersect(ray, si);
}

bool Scene::intersect(const Ray& ray) const {
    return _aggregate->intersect(ray);
}
