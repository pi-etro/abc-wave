# *A B C &nbsp;&nbsp; W A V E*

<div align="center">
    Interact online at <a href="https://pi-etro.github.io/abc-wave">https://pi-etro.github.io/abc-wave</a>
</div>
<br>

|            Name                   |    RA    |
|:---------------------------------:|:--------:|
| Pietro Di Consolo Gregorio        | 11038416 |
| Beatriz Libanio de Araujo Yordaky | 11013714 |

## Table of Contents

* [About](#about)
* [Implementation](#implementation)
* [Usage](#usage)
* [Releases](#releases)
* [License and credits](#license-and-credits)

## About

*A B C &nbsp;&nbsp; W A V E* &nbsp; is a 3D [vaporwave](https://en.wikipedia.org/wiki/Vaporwave) screensaver developed with the [ABCg](https://github.com/hbatagelo/abcg) framework for the Computer Graphics course at UFABC.

The project is inspired by the Vaporwave art style, that emerged in the early 2010s and was inspired by the pop culture, surrealism and 1990s aesthetics.

<br>
<p align="center">
  <img width="450" src="https://raw.githubusercontent.com/pi-etro/abc-wave/main/img/rafael-de-jongh-vaporwave.jpg">
</p>
<p align="center">Figure 1. Vaporwave image example (<a href="https://urbania.ca/article/petit-guide-dintroduction-vaporwave">font</a>).</p>
<br>

## Implementation

The screensaver consists of four instances of the `model` class: head of Michelangelo's David, Greek pillar, palm tree and a tile. Each of this models can be displayed one or more times in the scene, as seen for all models, with exception for the head of David.

<br>
<p align="center">
  <img width="480" src="https://raw.githubusercontent.com/pi-etro/abc-wave/main/img/3dmodels.png">
</p>
<p align="center">Figure 2. 3D models used in the scene.</p>
<br>

The `openglwindow` class is responsible for creating, updating and displaying all scene and models. The floor animation is managed by this class and is implemented in a way that when a tile is behind the `camera`, it is translated to the back of the scene, creating a treadmill effect.

Finally, The `camera` class is a simple implementation of a LookAt Camera with a fixed position and looking to the main scene.

For future implementations, new features will include `camera` user control, textures applied to models and scene lightning.

The main libraries used by ABCg are: [SDL](https://www.libsdl.org/) to handle inputs and outputs, [GLEW](http://glew.sourceforge.net/) to access the OpenGL graphical API and [emscripten](https://emscripten.org/) to generate the application's WebAssembly.

## Releases

### Version 1.0

  - [x] Basic scene composition and design
  - [x] Scene animation

### Features in Future Releases

  - [ ] User interaction
  - [ ] Model texture
  - [ ] Scene lighting

## Usage

Build the application with `build.sh` (Linux/macOS) or `build.bat` (Windows) and run the executable file created at `abcg/build/bin/abcinvaders`.

## License and credits

ABCg is a framework developed by Harlen Batagelo for the Computer Graphics course at UFABC, and is licensed under the MIT License. See [LICENSE](https://github.com/hbatagelo/abcg/blob/main/LICENSE) for more information.
