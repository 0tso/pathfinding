# Usage instructions

### Basic executing
Always start the program on the command line. Command line input (`stdin`) is required for the program to function.

The program can take one optional command line starting argument: a path to a map image. All images supported by [SFML](https://www.sfml-dev.org/documentation/2.6.1/classsf_1_1Image.php#a9e4f2aa8e36d0cabde5ed5a4ef80290b) are loadable. All white pixels RGB(255, 255, 255) are interpreted to be empty, all blue pixels RGB(0, 0, 255) are interpreted to be starting points, all red pixels RGB(255, 0, 0) are interpret to be end nodes, and every single other pixel is interpreted to be a wall.

There is an example map file to load in the [data](../data/) folder: [test_map.png](../data/test_map.png).

### Map editor
In the map editor, you can move the view with the WASD keys and zoom in and out with the mouse scrollwheel.

You can click with the left mouse button to place walls, and with the right mouse button to clear map squares.

You can press Q to place the starting point under your cursor, and E to place the end point under your cursor.

### Command line input

There are multiple command line options to input **after** starting the program:

<hr>

`exit` exits the program.

<hr>

`start` starts a pathfinding algorithm. It requires one additional argument after it, delimted by a space: the name of the pathfinding algorithm. This can be one of the following: 
* `A*`
* `OptimizedA*`
* `JPS`
* `BBFS`

One optional command line argument can be given: the amount of milliseconds (integer) to wait after each pathfinding logic update. This is useful for visualization.
* Example: `start A* 5` starts pathfinding with A* and waits 5 milliseconds between each pathfinding update.

<hr>