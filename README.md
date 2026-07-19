# byteknife

A cross-platform static analysis tool for ELF and PE files.
Analyze sections, imports & machine code with ease.

## Status

Early work in progress. ELF/PE header parsing is the current focus; a GUI (via imgui) will come later.

## Dependencies

- CMake 3.20+
- clang / clang++
- GLFW3 (`sudo apt install libglfw3-dev`)
- OpenGL

## Setup

```bash
git clone --recurse-submodules https://github.com/xavhq
cd byteknife
# if you already cloned without --recurse-submodules:
git submodule update --init --recursive
```

## Build

```bash
cmake -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```

`compile_commands.json` is generated automatically in `build/` (used by clangd/clang-tidy). Symlink it to the project root so editors pick it up without extra config:

```bash
ln -s build/compile_commands.json .
```

## Run

```bash
./build/dist/byteknife <path-to-binary>
```

## License

MIT
