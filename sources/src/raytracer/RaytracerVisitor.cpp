#include "omg/raytracer/RaytracerVisitor.h"
#include "omg/scene/Scene.h"
#include <iostream>

using namespace omg;

void RaytracerVisitor::visit(const std::shared_ptr<Scene>& scene) {

    auto camera = scene->get_camera();
    auto width = camera->get_width();
    auto height = camera->get_height();

    auto background = scene->get_background();
    this->_buffer = std::move(std::make_unique<Buffer<3>>(width, height));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            //auto [r, g, b] = background->find(x / static_cast<float>(width), y / static_cast<float>(height));
            auto color = background->find(x / static_cast<float>(width), y / static_cast<float>(height));
            auto [r, g, b] = std::tuple {color(0), color(1), color(2)};
            this->_buffer->set({x, y}, {
                    static_cast<unsigned char>(r), 
                    static_cast<unsigned char>(g), 
                    static_cast<unsigned char>(b)});
        }
    }
}
