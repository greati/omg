#ifndef __VEC__
#define __VEC__

#include <array>

/**
 * Represents an N-dimensional vector.
 *
 * @author Vitor Greati
 * */
namespace rayt {
template<int VecSize, typename T = float>

class Vec {

    private:

        std::array<T, VecSize> comps;   /** Vector components */

    public:

        Vec(const std::array<T, VecSize> & _comps) : comps {_comps} {/*empty*/}

        Vec(const std::initializer_list<T> & _comps) {
            if (_comps.size() != VecSize)
                throw std::invalid_argument("vec size not expected");
            for (auto b = _comps.begin(), i = 0; b != _comps.end(); ++b, ++i)
                comps[i] = *b;
        }

        /**
         * Copy-assignment operator.
         *
         * @param other source
         * @return copied
         * */
        inline Vec& operator=(Vec other) {
            std::swap(comps, other.comps);
            return *this;
        }

        /**
         * Initializer list assignment operator.
         *
         * @param other source
         * @return copied
         * */
        inline Vec& operator=(const std::initializer_list<T> & _comps) {
            for (auto b = _comps.begin(), i = 0; b != _comps.end(); ++b, ++i)
                comps[i] = *b;
            return *this;
        }

        inline decltype(comps) operator()(){
            return this->comps;
        }

};


template<typename T=float>
class Vec3 : public Vec<3, T> {

    public:
        Vec3(const std::array<T, 3> & _comps) 
            : Vec<3, T>{_comps} {/*empty*/}

        Vec3(const std::initializer_list<T> & _comps)
            : Vec<3, T>{_comps} { /*empty*/}

};

}

#endif
