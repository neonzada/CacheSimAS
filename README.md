# CacheSimAS

CacheSimAS is a configurable multi-level cache simulator developed in C++, supporting virtually infinite levels of cache with full customization via command-line parameters.

Each cache level argument follows this format: `<nsets>:<block_size>:<associativity>:<replacement_policy>`

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
If you don't have a hold of a .bin file, don't worry. Simply edit `input.txt` with your hex values of choice, run through `hex2bin` with `./hex2bin input.txt input.bin` and use your newly generated binary input on the simulator!

## 🧪 Example Usage

```bash
# L1 only
CacheSimAS 1024:4:1:r input.bin

# L1 and L2
CacheSimAS 1024:4:1:r 512:8:1:f input.bin

# L1, L2 and L3 (Random, FIFO, LRU)
CacheSimAS 1024:4:1:r 512:8:1:f 256:16:1:l input.bin
```

## 👨‍💻 Authors & Notes
This simulator was developed as a university project by Allan Duarte Ehlert and Stephan Lubke Heidmann.