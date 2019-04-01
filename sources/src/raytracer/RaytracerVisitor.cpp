#include "raytracer/RaytracerVisitor.h"
#include "scene/Scene.h"
#include <iostream>

using namespace rayt;

void RaytracerVisitor::visit(const std::shared_ptr<Scene>& scene) {

    auto camera = scene->get_camera();
    auto width = camera->get_width();
    auto height = camera->get_height();

    auto background = scene->get_background();
    this->_buffer = std::move(std::make_unique<Buffer<3>>(width, height));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            auto [r, g, b] = background->find(x / static_cast<float>(width), y / static_cast<float>(height));
            this->_buffer->set({x, y}, {r, g, b});
        }
    }
}
