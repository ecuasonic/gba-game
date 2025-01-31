# GBA Game

A simple Game Boy Advance game in the works. Written in C and ARM Assembly using DevkitPro. The game features a moving sprite against a scrolling background, which stationary berries relative to the background. Future updates include thorns, boundary collisions, and a losing screen.

## Features

- **Sprite and Background Handling**: The background scrolls with the sprite and wraps around when the sprite moves too far.
- **Object VRAM (OVRAM) Management**: Optimized sprite positioning and behavior through OAM attributes.
- **Dynamic Palette Management**: Utilized PAL RAM for efficient color handling, reducing memory usage.
- **Buffered Memory Operations**: Improved performance by processing offscreen data before rendering.
- **Tribool-based Input Handling**: Implemented a system for efficient user input processing.

## Requirements

- [DevkitPro](https://devkitpro.org/)
- [mGBA Emulator](https://mgba.io/)

## Installation

1. Clone the repository:
```bash
git clone https://github.com/ecuasonic/gba-game.git
cd gba-game
```
2. Build the project:
```bash
make
```

## Future Plans

- Add thorn obstacles and boundary collisions.
- Implement a losing screen upon collision with thorn.
- Include more sprite animations and background effects.

## Video Demo

Watch a demo of the game on YouTube: [Game Demo](https://youtu.be/jC4p3z7YD6o)
