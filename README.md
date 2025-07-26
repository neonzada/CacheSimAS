# CacheSimAS

CacheSimAS is a configurable multi-level cache simulator developed in C++, supporting virtually infinite levels of cache with full customization via command-line parameters.

Each cache level argument follows this format: `<nsets>:<block_size>:<associativity>`

The simulator takes a binary input file containing a sequence of 32-bit memory addresses. These addresses represent memory accesses and are interpreted in byte addressing.

## 🛠️ Build Instructions
This project uses CMake. Example build steps (with SDL2 for optional UI):
```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake -G "MinGW Makefiles" ..
cmake --build .

# Run the simulator
./CacheSimAS 1024:4:1 input.bin
```
Make sure SDL2 is correctly configured if using the UI.

## 🧪 Example Usage

```bash
# L1 only
CacheSimAS 1024:4:1:r input.bin

# L1 and L2
CacheSimAS 512:8:2 256:16:4 input.bin

# L1, L2 and L3
CacheSimAS 512:8:2 256:16:4 128:32:8 input.bin
```

## 👨‍💻 Authors & Notes
This simulator was developed as a university project by Allan Duarte Ehlert and Stephan Lubke Heidmann.