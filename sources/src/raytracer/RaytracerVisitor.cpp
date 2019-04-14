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

    Sphere sphere {0.8, Point3{0.0, 0.0, -3.0}};

    SurfaceInteraction hit_record;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            auto [px, py] = std::pair{x / static_cast<float>(width), y / static_cast<float>(height)};
            Ray ray = camera->generate_ray(px, py);
            //----- temporary built in shader
            bool hit = sphere.intersect(ray, hit_record);
            float pt = 1.0/hit_record._t;
            auto color = hit
                ? RGBColor {
                    (pt * 255), 
                    (pt * 255), 
                    (pt * 255)
                } 
            //-------------------------------
                : background->find(px, py);
            auto [r, g, b] = std::tuple {color(0), color(1), color(2)};
            this->_buffer->set({x, y}, {
                    static_cast<unsigned char>(r), 
                    static_cast<unsigned char>(g), 
                    static_cast<unsigned char>(b)});
        }
    }
}
