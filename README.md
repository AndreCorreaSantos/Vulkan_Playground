# Vulkan Rendering Project

This project includes Vulkan rendering of:

- Gerstner Waves
- Procedurally Generated Perlin Noise Terrain
- Perlin Noise Generated Blobs
- And much more...

The boilerplate code and initial file logic are based on tutorials from [Brendan Galea&#39;s YouTube channel](https://www.youtube.com/c/BrendanGalea).

## Installation Instructions

### 1. Install the Code Formatter

To install `clang-format`, run the following command:

```bash
sudo apt install clang-format
```

To execute the formatter run:
```
make format
```

### 2. Install the Necessary Dependencies

#### Install the Latest NVIDIA Drivers

Download and install the latest NVIDIA drivers from [NVIDIA&#39;s website](https://www.nvidia.com/en-us/drivers/).

#### Install Vulkan

Follow the Vulkan SDK installation guide from the [Vulkan homepage](https://vulkan.lunarg.com/doc/sdk/latest/windows/getting_started.html).

#### Install Shader Compilers

Download `glslc` from the [Shaderc GitHub repository](https://github.com/google/shaderc/tree/main/glslc) and add it to `/usr/local/bin/glslc`.

To compile the shaders, simply run:

```bash
./compile.sh
```

## Disclaimer

This project is most likely not compatible with non-Linux systems.
