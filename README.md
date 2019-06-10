# Space-Invaders

Space Invaders is a classic, fixed shooter, arcade game. The user controls a spaceship that can only move horizontally across the bottom of the screen. The spaceship is mounted with a laser cannon that the user is able to control with the aim to destroy the aliens (enemies). The player-controlled spaceship starts off protected behind bunkers that are placed horizontally across the screen. The enemy aliens also have the ability to shoot lasers downwards as they also move horiztonally and downwards. The bunkers will absorb the aliens lasers so long as it remains active. 

## Getting Started

Installation requires Git and triggers `git clone`. Building the project requires Make and a suitable C\C++ compiler.

### Prerequisites

- [Git](https://git-scm.com/)
- GNU based machines already come pre-installed with Make. For Windows users, [Make can be found here.](http://gnuwin32.sourceforge.net/packages/make.htm) 
- [Raspberry Pi 3 Model B+](https://www.raspberrypi.org/products/raspberry-pi-3-model-b-plus/)
- SNES Controller (can be purchased on Amazon, Ebay, etc)

### Installing

To begin installation, you must first clone the git repository.

```
git clone https://github.com/DanielContreras/Space-Invaders.git
cd Space-Invaders
```

### Building and Running the project

First, we must build the project. From the root directory of the Project, run the following commands:

```
Make clean
Make
```

Once the project has properly compiled, the executible will be found in the root directory. To run the executible, run the following command:

```
./myProg
```

### Controls

The game is played with an SNES controller. 

![](https://github.com/DanielContreras/Space-Invaders/blob/master/assets/controls.png)

### Instructions

![](https://github.com/DanielContreras/Space-Invaders/blob/master/assets/instructions.gif)

## Authors

* [**Daniel Contreras**](https://github.com/DanielContreras)
* [**Tariq Rahmani**](https://github.com/tariqrahmani08)
* [**Abdelraheem Ghebari**](https://github.com/abudeh123)

## License

This project is licensed under the MIT License - see [LICENSE.md](https://github.com/DanielContreras/Space-Invaders/blob/master/LICENSE) for details
