#include "omg/raytracer/RaytracerVisitor.h"
#include "omg/scene/Scene.h"
#include "omg/objects/Sphere.h"
#include "omg/raytracer/SurfaceInteraction.h"
#include <iostream>

using namespace omg;

void RaytracerVisitor::visit(const std::shared_ptr<Scene>& scene) {

    auto camera = scene->get_camera();
    auto width = camera->get_width();
    auto height = camera->get_height();

    auto background = scene->get_background();
    this->_buffer = std::move(std::make_unique<Buffer<3>>(width, height));

    auto objects = scene->get_objects();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            auto [px, py] = std::pair{x / static_cast<float>(width), y / static_cast<float>(height)};
            Ray ray = camera->generate_ray(px, py);
            //----- temporary built in shader
            RGBColor color;
            for (auto & obj : objects) {
                SurfaceInteraction hit_record;
                bool hit = obj->intersect(ray, hit_record);
                color = hit
                    ? RGBColor {255, 0, 0}
                //-------------------------------
                : background->find(px, py);
                if (hit) break;
            }
            auto [r, g, b] = std::tuple {color(0), color(1), color(2)};
            this->_buffer->set({x, y}, {
                    static_cast<unsigned char>(r), 
                    static_cast<unsigned char>(g), 
                    static_cast<unsigned char>(b)});
        }
    }
}
