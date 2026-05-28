# Coin Collector

A basic 2D game written in **C**, made as a hands-on way to learn the
`SDL2` graphics library and `SDL2_ttf` for text rendering.

## Overview

Move a square around a fullscreen window with **WASD** and collect coins
that spawn at random positions. The score is drawn in the top-right
corner.

## Requirements

- A C compiler (`gcc` via MinGW on Windows)
- `SDL2`
- `SDL2_ttf`

## Building

> **Note:** the included Makefile targets Windows / MinGW
> (uses `-lmingw32`, builds `coin_collector.exe`, cleans with `del`).
> On Linux or macOS you'll need to adjust the Makefile, or compile
> manually with `gcc main.c -o coin_collector $(sdl2-config --cflags --libs) -lSDL2_ttf`.

To build and run on Windows:

​```
make
make run
​```

To clean build artefacts:

​```
make clean
​```

## Purpose

The project is intentionally small — the goal was to get comfortable
with SDL's event loop, rendering, and font handling rather than to
build a polished game.
