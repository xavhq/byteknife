# byteknife

A cross-platform static and dynamic analysis toolkit for ELF and PE binaries - parses headers, sections, symbol tables, and imports/exports, with integrated x86/x86-64 disassembly and an interactive inspector GUI.

## Status

Early work in progress. ELF/PE header parsing is the current focus; a GUI (via Qt) will come later.

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

You need CMake, clang, and Qt6 (Widgets + Concurrent). Install commands vary by distro:

**Debian / Ubuntu / Linux Mint (apt):**
```bash
sudo apt install cmake clang qt6-base-dev
```

**Arch Linux (pacman):**
```bash
sudo pacman -S cmake clang qt6-base
```

**Fedora (dnf):**
```bash
sudo dnf install cmake clang qt6-qtbase-devel
```

### Optional: faster linking with mold

byteknife's `CMakeLists.txt` automatically detects and uses the [mold linker](https://github.com/rui314/mold) if it's installed, since it links noticeably faster than the default linker on large builds (Qt + Zydis means a lot of object files to link). This is entirely optional - if mold isn't installed, the build falls back to your system's default linker with no extra configuration needed.

**Debian / Ubuntu:**
```bash
sudo apt install mold
```

**Arch Linux:**
```bash
sudo pacman -S mold
```

**Fedora:**
```bash
sudo dnf install mold
```

Note: mold is Linux-only (with macOS support in early/pre-alpha and no official Windows support yet), so this only applies here, not in the Windows section below.

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

- **CMake 3.20+**: [cmake.org/download](https://cmake.org/download/) - grab the "Windows x64 Installer" (`.msi`), and check "Add CMake to system PATH" during install.
- **LLVM/Clang**: [github.com/llvm/llvm-project/releases](https://github.com/llvm/llvm-project/releases) - download the latest `LLVM-<version>-win64.exe` from the release's Assets list, and check "Add LLVM to system PATH" during install.
- **Qt6**: [qt.io/download-qt-installer](https://www.qt.io/download-qt-installer) - run the Qt Online Installer, sign in (free account required), and select the Qt6 "Desktop" component matching your compiler (MinGW or MSVC kit).

### Build

From a regular terminal (PowerShell or cmd) in the project root:
```powershell
cmake -B build -G "Ninja" -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/mingw_64
cmake --build build
```

Replace `C:/Qt/6.x.x/mingw_64` with your actual Qt install path (shown in the Qt Maintenance Tool, varies by version/kit). `Ninja` is recommended as the generator (install via `winget install Ninja-build.Ninja` if you don't have it). If you'd rather use Visual Studio's generator instead, drop `-G "Ninja"` and CMake will default to it, but you'll need the Visual Studio Build Tools installed regardless for the underlying linker.

### Run

```powershell
.\build\dist\byteknife.exe <path-to-binary>
```

---

## Build script (optional, recommended)

`build.lua` wraps the CMake configure/build steps above into single commands, and forces clang consistently so you don't need to remember the full flags every time.

### Install Lua

**Debian / Ubuntu:**
```bash
sudo apt install lua5.4
```

**Arch Linux:**
```bash
sudo pacman -S lua
```

**Fedora:**
```bash
sudo dnf install lua
```

**Windows:** download an installer from [lua.org](https://www.lua.org/download.html), or install via a package manager like [Scoop](https://scoop.sh) (`scoop install lua`) or [Chocolatey](https://chocolatey.org) (`choco install lua`).

### Make it executable (Linux/macOS only)

```bash
chmod +x build.lua
```

Windows doesn't use Unix file permissions, so this step isn't needed there - run it as `lua build.lua <command>` instead of `./build.lua <command>`.

### Commands

```bash
./build.lua           # configure + build (Release)
./build.lua debug     # configure + build (Debug)
./build.lua run       # build, then run the resulting executable
./build.lua clean     # remove the build/ directory entirely
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

## Documentation

Once built, read the docs: [byteknife](docs/byteknife.md)
