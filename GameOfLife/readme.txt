This program will simulate Conway's Game of Life. To run it, you will need a properly formatted text file. This file should contain the following:
Number of rows in the grid.
Number of columns in the grid.
Number of iterations of the game to simulate.
As many coordinates for live cells at the start of the game as you want. These are separated by line, with the first number being the x coordinate in the grid, and the second being the y coordinate. For example, if you want a live cell in the top left corner, and another in the bottom left of a 40x40 grid where the game runs for 100 iterations, your file should look like this:
40
40
100
0 0
0 39

The command line arguments for this program are as follows:
The first argument is your input filename.
The second decides whether or not the board wraps. If it does wrap, a cell on the right-most side of the board will have neighbors on the left-most side. Type "wrap" for wrapping, and "nowrap" for no wrapping.
The third argument determines whether or not each iteration of the game will show or not. "show" will display every iteration, and "hide" will only display the final iteration of the game.
The fourth argument should only be used if "show" was chosen. This will determine the speed at which the game runs. "slow" will show 3 iterations per second, "med" will show 10 iterations per second, and "fast" will show 30 iterations per second. If you want a wrapping grid that displays every iteration at a rate of 10 iterations per second, your command line will look like this:
./GoL input.txt wrap show med