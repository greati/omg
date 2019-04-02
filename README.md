# omg - Open Machinery for Graphics

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

omg comes with a raytracer library and executable, which takes a scene
described in YAML and produces an image. In order to run it,
execute:

```bash
./raytracer <path to .yml file> <image destination>
```
