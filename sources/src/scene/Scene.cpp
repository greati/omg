#include "omg/scene/Scene.h"

using namespace omg;

bool Scene::intersect(const Ray& ray, SurfaceInteraction* si) const {
    //TODO this will change to _objects->intersect(ray, si) when _objetcs become a tree
    for (auto & o : _primitives) 
        o->intersect(ray, si);
    return (si->_primitive != nullptr);
}

bool Scene::intersect(const Ray& ray) const {
    for (auto & o : _primitives) 
        if (o->intersect(ray))
            return true;
    return false;
}
