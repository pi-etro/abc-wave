# *A B C &nbsp;&nbsp; W A V E*

<div align="center">
    Interact online at <a href="https://pi-etro.github.io/abc-wave">https://pi-etro.github.io/abc-wave</a>
    <br>
    Allow Autoplay to listen to the classic vaporwave music
    <br>
    リサフランク420 / 現代のコンピュ by Macintosh Plus (Ramona Xavier).
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

The `openglwindow` class is responsible for creating, updating and displaying all scene and models with their corresponding textures. The floor animation is managed by this class and is implemented in a way that when a tile is behind the `camera`, it is translated to the back of the scene, creating a treadmill effect. This effect was implemented only on local build, due the inefficiency of the WebAssembly build with textures, which decreases the framerate of the application.

<br>
<p align="center">
  <img width="450" src="https://raw.githubusercontent.com/pi-etro/abc-wave/main/img/local_demo.gif">
</p>
<p align="center">Figure 3. Treadmill effect on local build.</p>
<br>

Three different shaders were used in the scene: the `texture` shader with mesh mapping was used on the palm trees and with spherical mapping on the pillars, the "pure" `blinnphong` was used on the tiles (using the material's properties to colour them) and the `normal` shader, which colors the model based on its normals, was used on the head of David to give it an iridescent look.

<br>
<p align="center">
  <img width="225" src="https://raw.githubusercontent.com/pi-etro/abc-wave/main/img/metal.jpg">
  <img width="225" src="https://raw.githubusercontent.com/pi-etro/abc-wave/main/img/palm.png">
</p>
<p align="center">Figure 4. Textures used on the pillars and the palm trees, respectively.</p>
<br>

Finally, the `camera` class is a implementation of a LookAt Camera that can be moved with `WASD` or `↑←↓→` keys, making it possible to "walk" through the scene.

The main libraries used by ABCg are: [SDL](https://www.libsdl.org/) to handle inputs and outputs, [GLEW](http://glew.sourceforge.net/) to access the OpenGL graphical API and [emscripten](https://emscripten.org/) to generate the application's WebAssembly.

## Releases

### Version 2.0

  - [x] User interaction
  - [x] Model texture
  - [x] Scene lighting
  - [x] Improved treadmill effect

### Version 1.0

  - [x] Basic scene composition and design
  - [x] Scene animation

## Usage

Build the application with `build.sh` (Linux/macOS) or `build.bat` (Windows) and run the executable file created at `abcg/build/bin/abc-wave`.

## License and credits

ABCg is a framework developed by Harlen Batagelo for the Computer Graphics course at UFABC, and is licensed under the MIT License. See [LICENSE](https://github.com/hbatagelo/abcg/blob/main/LICENSE) for more information.
