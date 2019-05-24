#ifndef _BLINN_INT_
#define _BLINN_INT_

#include "omg/integrators/SamplerIntegrator.h"
#include "omg/materials/BlinnMaterial.h"
#include "omg/common.h"
#include "omg/lights/VisibilityTester.h"

namespace omg {
/**
 * Integrator based on Blinn Phong.
 *
 * @author Vitor Greati
 * */
class BlinnPhongIntegrator : public SamplerIntegrator {


    private:

        int depth_max = 1;

    public:

        BlinnPhongIntegrator(int spp) : SamplerIntegrator(spp) {
            this->_suffix = "_blinn";
        }

        /**
         * Incidence radiance computation for a given ray
         * acting on a scene.
         *
         * @param ray the ray
         * @param scene the scene
         * @param sampler pointer to a sampler
         * */
        RGBColor li(const Ray& ray,
                const Scene& scene,
                float px = 0.0,
                float py = 0.0,
                const std::shared_ptr<Sampler> sampler = nullptr,
                int depth=0) override {

            //> Resulting irradiance
            RGBColor L {0.0, 0.0, 0.0};

            //> Ambient intensity
            Vec3 ambient_intensity {0.0, 0.0, 0.0};

            //> Blinn parameters
            Vec3 ka;
            Vec3 kd;
            Vec3 ks;
            Vec3 km;
            RealValue gloss;
            Vec3 normal;

            SurfaceInteraction si;
            bool hit = scene.intersect(ray, &si);

            const BlinnMaterial* material = nullptr;

            if (hit) {
                if (material = dynamic_cast<const BlinnMaterial*>(si._primitive->get_material())) {
                   ka = material->ka();
                   kd = material->kd();
                   ks = material->ks();
                   km = material->km();
                   gloss = material->glossiness();
                   normal = tao::unitize(si._n);
                }
            } else {
                if (depth == 0)
                    return scene.get_background()->find(px, py);
                else
                    return RGBColor {0.0, 0.0, 0.0};
            }

            for (auto& light : scene.get_lights()) {
                if (light->is_ambient()) {
                    ambient_intensity = light->get_intensity();
                    if (material != nullptr) continue; else break;
                }

                if (material != nullptr) {
                    Vec3 wi;
                    VisibilityTester vt;
                    auto I = light->sample_li(si, &wi, &vt);


                    if (vt.unoccluded(scene)) {
                        L += std::max(0.0f, tao::dot(normal, wi)) * kd.element_wise(I, 
                                [](auto x, auto y) {return x*y;});

                        auto wo = tao::unitize(si._wo);
                        auto h = (wo + wi) / tao::norm(wo + wi);

                        L += std::pow(std::max(0.0f, tao::dot(normal, h)), gloss) 
                         * ks.element_wise(I, 
                                [](auto x, auto y) {return x*y;});

                    }
                }
            }

            L += ka.element_wise(ambient_intensity, [](auto x, auto y) { return x * y;});

            if (!(material->km() == Vec3{0.0, 0.0, 0.0}) && depth < depth_max) {
                auto ray_direction = tao::unitize(ray.get_direction());
                auto reflected = ray_direction - 2.0f * (tao::dot(ray_direction, normal)) * normal;
                L += km.element_wise(this->li(Ray {si._p, tao::unitize(reflected)}, scene, px, py, nullptr, depth + 1),
                            [](auto x, auto y) {return x*y;});
            }

            return {std::min(255.0f, L(0)), std::min(255.0f, L(1)), std::min(255.0f, L(2))};
        }

};
};
#endif
