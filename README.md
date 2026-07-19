# byteknife

A cross-platform static and dynamic analysis toolkit for ELF and PE binaries - parses headers, sections, symbol tables, and imports/exports, with integrated x86/x86-64 disassembly and an interactive inspector GUI.

## Status

Early work in progress. ELF/PE header parsing is the current focus; a GUI (via imgui) will come later.

## Setup

```bash
git clone --recurse-submodules https://github.com/xavhq/byteknife.git
cd byteknife
# if you already cloned without --recurse-submodules:
git submodule update --init --recursive
```

---

## Linux

### Dependencies

- CMake 3.20+
- clang / clang++
- GLFW3: `sudo apt install libglfw3-dev`
- OpenGL (usually preinstalled with GPU drivers; if missing: `sudo apt install libgl1-mesa-dev`)

### Build

```bash
cmake -B build -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
cmake --build build
```

### Run

```bash
./build/dist/byteknife <path-to-binary>
```

---

## Windows

### Dependencies

- [CMake](https://cmake.org/download/) 3.20+ (add to PATH during install)
- [LLVM/Clang for Windows](https://github.com/llvm/llvm-project/releases) (add to PATH during install)
- [vcpkg](https://github.com/microsoft/vcpkg) — used to install GLFW and OpenGL headers

Set up vcpkg once, if you haven't already:

```powershell
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install glfw3 opengl
```

### Build

From a regular terminal (PowerShell or cmd) in the project root:

```powershell
cmake -B build -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

Replace `C:/path/to/vcpkg` with wherever you cloned vcpkg. `Ninja` is recommended as the generator (install via `winget install Ninja-build.Ninja` if you don't have it) — it plays better with clang than the default Visual Studio generator. If you'd rather use Visual Studio's generator instead, drop `-G "Ninja"` and CMake will default to it, but you'll need the Visual Studio Build Tools installed regardless for the underlying linker.

### Run

```powershell
.\build\dist\byteknife.exe <path-to-binary>
```

---

## Editor setup (both platforms)

`compile_commands.json` is generated automatically in `build/` (used by clangd/clang-tidy). Symlink or copy it to the project root so editors pick it up without extra config:

```bash
# Linux/macOS
ln -s build/compile_commands.json .

# Windows (PowerShell, run as Administrator for a symlink, or just copy it)
Copy-Item build\compile_commands.json .
```

## License

MIT
