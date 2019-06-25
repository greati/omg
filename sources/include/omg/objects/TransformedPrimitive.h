#ifndef _OMG_TRANS_PRIM_
#define _OMG_TRANS_PRIM_
namespace omg {
/**
 * Represents transformed primitive to be rendered
 * in the screen.
 *
 * @author Vitor Greati
 * */
class TransformedPrimitive : public Primitive {

    private:

        std::shared_ptr<Primitive> primitive;
        const Transform primitive_to_world;

    public:

        TransformedPrimitive(std::shared_ptr<Primitive>& primitive,
                const Transform& primitive_to_world)
        : primitive {primitive}, primitive_to_world {primitive_to_world} {/* empty */}

        bool intersect(const Ray& r, SurfaceInteraction* interaction) const {
            // TODO AnimatedPrimitive: use interpolate to produce the transform
            Ray ray = inverse(primitive_to_world).t_ray(r);
            if (!primitive->intersect(ray, interaction))
                return false;
            r.tMax = ray.tMax;
            if (!primitive_to_world.is_identity())
                *interaction = primitive_to_world.t_si(*interaction);
            return true; 
        }


        virtual bool intersect(const Ray& r) const {
            Ray ray = inverse(primitive_to_world).t_ray(r);
            return primitive->intersect(ray);
        }

        virtual Bounds3 world_bound() const {
            return primitive->world_bound();
        }
};
};
#endif
