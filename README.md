# Timethief GBA

A Game Boy Advance game built with [Butano](https://github.com/GValiente/butano).

## Requirements

- [devkitPro](https://devkitpro.org/wiki/Getting_Started) with devkitARM
- Python 3
- [mGBA](https://mgba.io/) (optional, for emulation)

## Building

```bash
# Clone with submodules
git clone --recurse-submodules <repo-url>

# Build the ROM
make -j$(nproc)

# Clean build files
make clean
```

The output ROM will be `timethief-gba.gba`.

## Running

```bash
mgba timethief-gba.gba
```

## Project Structure

```
timethief-gba/
├── src/           # C++ source files
├── include/       # Header files
├── graphics/      # Sprites and backgrounds (PNG files processed by grit)
├── audio/         # Sound effects and music (WAV/IT/S3M/MOD/XM files)
├── dmg_audio/     # DMG/chiptune audio (VGM files)
├── butano/        # Butano engine (git submodule)
├── build/         # Compiled objects (generated)
└── Makefile       # Build configuration
```

## Adding Assets

### Graphics
Place PNG files in `graphics/` with a corresponding `.json` file for metadata. See [Butano graphics documentation](https://gvaliente.github.io/butano/import.html).

### Audio
Place audio files in `audio/`. Supported formats: WAV, IT, S3M, MOD, XM. See [Butano audio documentation](https://gvaliente.github.io/butano/import.html).

## Resources

- [Butano Documentation](https://gvaliente.github.io/butano/)
- [Butano Examples](https://github.com/GValiente/butano/tree/master/examples)
- [GBA Technical Reference](https://www.coranac.com/tonc/text/toc.htm)
