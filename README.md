# HotCrossBuns

A distributed pub-sub model built using the HotStuff consensus algorithm.
Refer [PROTOCOL.md](./PROTOCOL.md)

## Build Instructions

1. Clone the GitHub repository
2. `cd` into the folder
3. Run the following commands:
```bash
cmake -B build
cmake --build build
```
4. The executable `hotcrossbuns` should be in the `build/` folder
5. To build again (without changing the build configuration, run just the following
```bash
cmake --build build
```
6. To build again with configuration changes, repeat step 3.

## Project Structure

- `src/` **Source folder**: 
    Includes all the files with the main functionality. These files are built into
    `libhcb.a`
- `app/` **Executable folder**:
    The code for the actual `hotcrossbuns` executable, build using `libhcb.a`
