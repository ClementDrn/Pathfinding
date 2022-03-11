# Pathfinding

## Description

This project includes a C++ **Pathfinding** library and an application named **App** to test it.


## Installation

### Windows

Clone the repository.

```git
git clone --recursive https://github.com/ClementDrn/Pathfinding
```

Generate the Visual Studio projects with premake by running the `Scripts/WinVS-GenerateProjects.bat` script. This requires Visual Studio 2022 to be installed.

Open the Visual Studio solution and build it.

You can now run the **App** project to test the library.

***/!\ It seems that running App with a Win32 architecture build does not work.***


## Features

### Pathfinding

The **Pathfinding** library can be used to find a path from a starting point to a goal through a grid containing obstacles. 

For now, the only supported algorithm is `A*`.

### App

The test application can illustrate the use of the **Pathfinding** library. It is a SFML desktop application showing a grid with one starting point, one goal, obstacles and a path between the two points.

You can interact with the grid with keyboard and mouse inputs.

| Inputs                                 | Action                    |
|----------------------------------------|---------------------------|
| `Left Mouse Press`                     | Place obstacle            |
| `Right Mouse Press`                    | Remove obstacle           |
| `Left Mouse Press` + `Left Control`    | Move start                |
| `Right Mouse Click` + `Left Control`   | Move goal                 |


## License

The **Pathfinding** library and the **App** are licensed under the MIT license.


## Credits

* [SFML](https://github.com/SFML/SMFL)
* Wikipédia [[FR](https://fr.wikipedia.org/wiki/Algorithme_A*)] [[EN](https://en.wikipedia.org/wiki/A*_search_algorithm)]
* [Sebastian Lague](https://youtu.be/-L-WgKMFuhE)
* Clément Darne
