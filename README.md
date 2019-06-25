# omg - Open Machinery for Graphics

![star](examples/imgs/tea_dragon.png)

**omg** is a library for computer graphics applications that provides
some executables, like a ready-to-go raytracer that uses YAML
scene descriptions to produce 3D images.

**omg** uses [tao](http://github.com/greati/tao), a mathematical
library written by the author. Caution: it is not optimized yet,
but works well.

Learn how to use **omg** by [reading the docs](https://greati.github.io/omg/html/index.html)!

## Installing

### Cloning

This project makes use of *git submodules* feature, so you need
to clone it using:

```bash
git clone --recursive https://github.com/greati/omg.git
```

### Build

```
mkdir build
cd build
cmake ../sources .
cmake --build .
```

### Test

```
make test
```

## Raytracer

**omg** comes with a raytracer library and executable, which takes a scene
described in YAML (check the [YAML Raytracer wiki](https://github.com/greati/omg/wiki/YAML-Raytracer)) and produces an image. In order to run it,
execute:

```bash
./raytracer <path to .yml file> <image destination>
```
### Features

- [ ] Backgrounds
    - [x] Solid
    - [x] Bilinear gradient
    - [ ] Texture
- [x] Camera
    - [x] Size
    - [x] Orthographic
    - [x] Perspective
    - [x] Oblique versions of orthographic and perspective
- [x] Integrators
    - [x] Flat
    - [x] Depth map
    - [x] Normal map
    - [x] Blinn Phong (with reflection and hard shadow)
- [x] Objects
    - [x] Spheres
    - [x] Triangle meshes
- [ ] Accelerators
    - [x] BVH
    - [ ] KDTree
- [x] Geometric transformations
