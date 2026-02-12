# PixelQuest

PixelQuest is a 2D game built with [raylib](https://www.raylib.com/), featuring single-player and local multiplayer modes. The game includes multiple levels, player movement, split-screen camera support, and a simple UI displaying player stats and FPS.

## Features

- Main menu and game over screens
- Multiple levels
- Single-player and two-player support with split-screen view
- Real-time player movement and collision
- UI showing player positions, velocities, and frame timing
- Configurable resolution, full screen, and target FPS
- Linux and Windows support

## Controls

- Use the keyboard to control players (specific keys depend on your implementation)
- Press `Escape` to return to the main menu or exit from the game over screen

## Building and Running

### Requirements

- [raylib](https://www.raylib.com/) (C library)
- C compiler (e.g., gcc or clang)
- Make (optional, if you have a Makefile)

### Build

Supports building for linux and windows, makefile is setup to detect OS and build accordingly.
Windows require MinGW to build

```sh
make build
```

### Run

```sh
./bin/release/pixelquest
```

## Code Structure

- `src/game.c`: Main game loop, player and level management, rendering, and UI
- `src/level.h`, `src/menu.h`, etc.: Supporting headers for levels and menus

## License

MIT License
