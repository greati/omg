# omg - Open Machinery for Graphics

**omg** is a library for computer graphics applications that provides
some executables, like a ready-to-go raytracer that uses YAML
scene descriptions to produce 3D images.

**omg** uses [tao](http://github.com/greati/tao), a mathematical
library written by the author. Caution: it is not optimized yet,
but works well.

Learn how to use **omg** by [reading the docs](https://greati.github.io/omg/docs/html/index.html)!

## Installing
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
### Available features

- [ ] Backgrounds
    - [x] Solid
    - [x] Bilinear gradient
    - [ ] Texture
- [ ] Camera
    - [x] Size
    - [ ] Orthographic
    - [ ] Perspective
