///////////////////////////////////////////////////////////////
//
//             Ethan-John Rasmussen
//             CSCE 211, HW2 Pt 2.
//             March 9th, 2018.
//
///////////////////////////////////////////////////////////////



#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;
bool foundExit = false;

// Global arrays that can be accessed in any of my functions, used to store the path that the algorithm took in forward order.
int pathX[1000] = { 0 };
int pathY[1000] = { 0 };

// Function prototypes
void printMaze(char maze[][WIDTH], int curx, int cury);
bool validMove(char maze[][WIDTH], bool visited[][WIDTH],
	int newX, int newY);
bool move(char maze[][WIDTH], bool visited[][WIDTH],
	int &curX, int &curY, int newX, int newY);
void initializeMaze(char maze[][WIDTH]);
bool search(char maze[][WIDTH], bool visited[][WIDTH],
	int x, int y, int counter);

// Creates a maze 20 by 20, with an exit.
void initializeMaze(char maze[][WIDTH])
{
	int randY, randX;
	
	// This for loop creates the border of the maze.
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{

			if ((j == 0) || (j == 19))
			{
				maze[i][j] = 'X';
			}
			else if ((i == 0) || (i == 19))
			{
				maze[i][j] = 'X';
			}
			else
			{
				maze[i][j] = ' ';
			}
		}
	}
	
	// This for loop fills 25% of the maze area with walls. Decrease the number four to two to see my program create unsolveable mazes.
	for (int i = 0; i < ((HEIGHT-2) * (WIDTH - 2) / 4); i++)
	{
		do
		{
			randY = (rand() % 18) + 1;
			randX = (rand() % 18) + 1;
		} while (maze[randY][randX] == 'X');
		maze[randY][randX] = 'X';
	}

	// This loop creates a random exit for the maze, it loops if it is adjacent to any walls, this does not 100% guarantee that the maze is solve-able but helps.
	do
	{
		randY = (rand() % 18) + 1;
		randX = (rand() % 18) + 1;

	} while (((maze[randY + 1][randX] == 'X') || (maze[randY - 1][randX] == 'X') || (maze[randY][randX + 1] == 'X') || (maze[randY][randX - 1] == 'X')) || (maze[randY][randX] == 'X'));
	maze[randY][randX] = 'E';
}


// Return true or false if moving to the specified coordinate is valid
// Return false if we have been to this cell already
bool validMove(char maze[][WIDTH], bool visited[][WIDTH],
	int newX, int newY)
{
	// Check for going off the maze edges
	if (newX < 0 || newX >= WIDTH)
		return false;
	if (newY < 0 || newY >= HEIGHT)
		return false;
	// Check if target is a wall
	if (maze[newY][newX] == 'X')
		return false;
	// Check if visited
	if (visited[newY][newX])
		return false;
	return true;
}

// Make the move on the maze to move to a new coordinate
// I passed curX and curY by reference so they are changed to
// the new coordinates.  Here we assume the move coordinates are valid.
// This returns true if we move onto the exit, false otherwise.
// Also update the visited array.
bool move(char maze[][WIDTH], bool visited[][WIDTH],
	int &curX, int &curY, int newX, int newY)
{
	bool foundExit = false;
	if (maze[newY][newX] == 'E') 	// Check for exit
		foundExit = true;
	curX = newX;			// Update location
	curY = newY;
	visited[curY][curX] = true;

	return foundExit;
}

// Display the maze in ASCII, use double width for each point in the array so the maze looks more quare like.
void printMaze(char maze[][WIDTH], int curx, int cury)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if ((x == curx) && (y == cury))
				cout << "@@";
			else if (maze[y][x] == 'X')
				cout << "[]";
			else if (maze[y][x] == 'E')
				cout << "EX";
			else
				cout << "  ";
		} 
		cout << endl;
	}
}

// Recursively search from x,y until we find the exit
bool search(char maze[][WIDTH], bool visited[][WIDTH],
	int x, int y, int counter)
{
	
	if (maze[y][x] == 'E')
		return true;
	visited[y][x] = true;
	if (validMove(maze, visited, x, y - 1))
		foundExit = search(maze, visited, x, y - 1, ++counter);
	if (!foundExit && (validMove(maze, visited, x, y + 1)))
		foundExit = search(maze, visited, x, y + 1, ++counter);
	if (!foundExit && (validMove(maze, visited, x - 1, y)))
		foundExit = search(maze, visited, x - 1, y, ++counter);
	if (!foundExit && (validMove(maze, visited, x + 1, y)))
		foundExit = search(maze, visited, x + 1, y, ++counter);
		
	// If the exit has been found then that means the maze was solveable so then it stores the current position in global arrays.
	if (foundExit)
	{	
		pathX[counter] = x;
		pathY[counter] = y;
		return true;
	}
	return false;
}

int main()
{
	srand(time(NULL));
	
	char maze[HEIGHT][WIDTH];
	int randX, randY;
	
	// Create the maze.
	initializeMaze(maze);

	// Create the entrance to the maze in main because the values for the entrance must be passed into the search function.
	do
	{
		randY = (rand() % 18) + 1;
		randX = (rand() % 18) + 1;

	} while (((maze[randY + 1][randX] == 'X') && (maze[randY - 1][randX] == 'X') && (maze[randY][randX + 1] == 'X') && (maze[randY - 1][randX] == 'X')) || (maze[randY][randX] == 'X'));
	
	bool visited[HEIGHT][WIDTH];

	// Initialize visited locations to false
	for (int x = 0; x < WIDTH; x++)
		for (int y = 0; y < HEIGHT; y++)
			visited[y][x] = false;
	visited[randY][randX] = true;

	// Call the recursive function and save the result of it in a boolean to tell if the maze was completed or not.
	bool solveable = search(maze, visited, randX, randY, 0);

	// If the maze was somehow not completed. This program WILL actually determine if it is solveable or not, it is unlikely that it will be unsolveable as it is currently.
	// Increase the number of walls created in my initializeMaze function to see it create unsolveable mazes.
	if (!solveable)
	{
		cout << "Maze was not solveable." << endl;
		printMaze(maze, randX, randY);
		system("pause");
	}
	// If the maze is completed. Prints the maze then clears the screen and prints again.
	else
	{
		for (int i = 0; i < 1000; ++i) {
			if (pathY[i] != 0)
			{
				printMaze(maze, pathX[i], pathY[i]);
				system("pause");
				system("cls");
			}
		}
	}
	return 0;
}
