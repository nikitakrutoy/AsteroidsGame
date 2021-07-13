# AsteroidsGame
Yet Another Asteroids Game

This game was made as part of admission tests for MIPT GameDev Master Program.

## Installation
`cmake .` from project root to build. The binary would be in project's bin folder. You can move it to /usr/local/bin. All resources are contained in binary.

You can also try to run binaries from bin folder. MacOS binary was built on Big Sur 11.4. Linux binary was build in docker container. Commands to reproduce:

```
docker build . -t asteroidsbuild 
docker run --volume /path/to/project/root:/app asteroidsbuild /bin/bash -c "cmake ../ && cmake --build ."
```

## Gameplay
`Left/Right` to rotate. `Up`to Boost. `Space` to shoot. Game can paused at any moment with `Esc` key.

After crash you will be invicible for a few second. Your ship will blink. Crashing into asteroids also destoies them.
Smaller asteroids will give you more score.

You can play in Infinite Mode where you have 3 lives and infinitly spawning asteroids or complete levels with fixed amount of asteroids.

In levels, asteroids movement is seamless with respect to bordres of the screen. In Infite mode it also seamless but the border is a bit wider then sceen.

Starship movement is always seamless.

Levels progress is stored on disk between game launches in /tmp folder.


## Framework
Engine.h and Engine.cpp were provided and not modified.
Game uses stb_images.h and stb_truetype for image and font processing.

Most of gameplay logic code can be found in [GameFieldScene](/src/GameFieldScene.cpp) class. It draws game field and manages starship, asteroids and projectiels behaviour.

[Geometry.cpp](/src/Geometry.cpp) containes set of classes for processing points, lines and paths.

[GameObject.cpp](/src/GameObject.cpp) defines base GameObject class and bunch of game object that used in game: Starship, Asteroid, Projectile, TextObject, Star, Planet etc.

[Scene.h](/src/Scene.h) defines base class for scenes, which are also game object. The difference is that scenes can containe and manipulate other game objetc. Main purpose of scenes is to describe logic inside one game screen (scene). Usually one Scene child defines one actual scene.

[MenuScene.cpp](/src/MenuScene.cpp) and [MenuScene.h](/src/MenuScene.h) containes UI scenes classes. Mostly different kind of menus.

[MenuItem.cpp](/src/MenuItem.cpp) and[MenuItem.h](/src/MenuItem.h) container UI game object classes.

[SceneManeger.cpp](/src/SceneManager.cpp) implements manager that controls switching scenes for scene layers. There are 3 scene layers: Background, Game Field and UI. The only difference is rasterization order.

[Rasterizer.cpp](/src/Rasterizer.cpp) implements different kinds of rasterization algorithms used for drawing game objects and scenes.

[GameState.h](/src/GameState.h) defines global game variables like lives and score that are accessble to all scene and game objects.

## Setting

To modify some parameters before build you can be tune macros in inside [MagicNumbers.h](/src/MagicNumbers.h).



## Screenshots
Gameplay Video: https://youtu.be/QXgRr5RMYZQ

![Title Screen](/resources/images/screenshot1.png?raw=true "Title Screen")
![Game Field](/resources/images/screenshot2.png?raw=true "Game Field")
