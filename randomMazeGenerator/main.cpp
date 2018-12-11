#include "SDL2-2.0.9\include\SDL.h"

#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.9\\lib\\x86\\SDL2main.lib")

#include <time.h>
#include <cmath>
#include <Windows.h>
#include <iostream>
using namespace std;

struct Block
{
	SDL_Rect rect;
	int value;
};

void initMaze(Block *Maze, int mazeN, int gridWidth, int gridHeight, int boxSize)
{
	for (int i = 0; i < mazeN; i++)
	{
		Maze[i].rect.h = boxSize;
		Maze[i].rect.w = boxSize;
		Maze[i].rect.x = (i % gridWidth) * boxSize;
		Maze[i].rect.y = (i / gridWidth) * boxSize;
		if (i / gridWidth == 0) Maze[i].value = 0;
		else if (i / gridWidth == gridHeight) Maze[i].value = 0;
		else if ((i % gridWidth) % 2 == 0) Maze[i].value = 0;
		else if ((i / gridWidth) % 2 == 0) Maze[i].value = 0;
		else Maze[i].value = 1;
	}
}

void push(int *stack, int &stackN, int input)
{
	stack[stackN++] = input;
}

int pop(int *stack, int &stackN)
{
	return stack[--stackN];
}

void wmain()
{
	//initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	//create window
	int screenWidth = 800;
	int screenHeight = 600;
	SDL_Window *window = SDL_CreateWindow("random walker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

	//create renderer attached to the window created above
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	srand(time(0));
	
	//run timer
	int timeStart = 0;
	int timeEnd = 0;

	//create maze grid
	int boxSize = 10;

	int gridWidth;
	if ((screenWidth / boxSize) % 2 == 1) gridWidth = screenWidth / boxSize;
	else gridWidth = (screenWidth / boxSize) - 1;
	int gridHeight;
		if ((screenHeight / boxSize) % 2 == 1) gridHeight = screenHeight / boxSize;
		else gridHeight = (screenHeight / boxSize) - 1;
	int mazeN = gridHeight * gridWidth;

	Block *Maze = new Block[mazeN];

	initMaze(Maze, mazeN, gridWidth, gridHeight, boxSize);

	//create maze walker
	int mazeWalkerX = gridWidth / 2;
	int mazeWalkerY = gridHeight / 2;
	int direction;
	int lastPosition = 1;
	int position = 0;
	int maxPathN = (gridWidth - 1) * (gridHeight - 1);
	int pathN = 0;
	int *Path = new int[maxPathN];

	for (;;)
	{
		//check for quit
		SDL_Event event;
		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT) exit(0);
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) exit(0);
		}
		timeStart = SDL_GetTicks();
		if (timeStart - timeEnd >= 1000 / 60)
		{
			//update walker position

			if (lastPosition == position)
			{
				direction = pop(Path, pathN);
				if (direction == 0)
				{
					for (int i = 0; i < 3; i++) Maze[(mazeWalkerY * gridWidth) + mazeWalkerX--].value = 3;
					mazeWalkerX++;
				}
				else if (direction == 1)
				{
					for (int i = 0; i < 3; i++) Maze[(mazeWalkerY * gridWidth) + mazeWalkerX++].value = 3;
					mazeWalkerX--;
				}
				else if (direction == 2)
				{
					for (int i = 0; i < 3; i++) Maze[(mazeWalkerY-- * gridWidth) + mazeWalkerX].value = 3;
					mazeWalkerY++;
				}
				else if (direction == 3)
				{
					for (int i = 0; i < 3; i++) Maze[(mazeWalkerY++ * gridWidth) + mazeWalkerX].value = 3;
					mazeWalkerY--;
				}
			}

			else
			{
				direction = rand() % 4;

				for (int j = 0; j < 4; j++)
				{
					if (direction == 0)
					{
						if (Maze[(mazeWalkerY * gridWidth) + mazeWalkerX + 2].value == 1 && mazeWalkerX + 2 < gridWidth)
						{
							for (int i = 0; i < 3; i++) Maze[(mazeWalkerY * gridWidth) + mazeWalkerX++].value = 2;
							mazeWalkerX--;
							push(Path, pathN, direction);
							break;
						}
						else direction = (direction + 1) % 4;
					}
					else if (direction == 1)
					{
						if (Maze[(mazeWalkerY * gridWidth) + mazeWalkerX - 2].value == 1 && mazeWalkerX - 2 > 0)
						{
							for (int i = 0; i < 3; i++) Maze[(mazeWalkerY * gridWidth) + mazeWalkerX--].value = 2;
							mazeWalkerX++;
							push(Path, pathN, direction);
							break;
						}
						else direction = (direction + 1) % 4;
					}
					else if (direction == 2)
					{
						if (Maze[((mazeWalkerY + 2) * gridWidth) + mazeWalkerX].value == 1 && mazeWalkerY + 2 < gridHeight)
						{
							for (int i = 0; i < 3; i++) Maze[(mazeWalkerY++ * gridWidth) + mazeWalkerX].value = 2;
							mazeWalkerY--;
							push(Path, pathN, direction);
							break;
						}
						else direction = (direction + 1) % 4;
					}
					else if (direction == 3)
					{
						if (Maze[((mazeWalkerY - 2) * gridWidth) + mazeWalkerX].value == 1 && mazeWalkerY - 2 > 0)
						{
							for (int i = 0; i < 3; i++) Maze[(mazeWalkerY-- * gridWidth) + mazeWalkerX].value = 2;
							mazeWalkerY++;
							push(Path, pathN, direction);
							break;
						}
						else direction = (direction + 1) % 4;
					}
				}
			}
			lastPosition = position;
			position = (mazeWalkerY * gridWidth) + mazeWalkerX;

			if (pathN == 0)
			{
				Sleep(1000);
				initMaze(Maze, mazeN, gridWidth, gridHeight, boxSize);
			}

			//set color to black
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			//clear screen
			SDL_RenderClear(renderer);

			//draw grid
			for (int i = 0; i < mazeN; i++)
			{
				if (Maze[i].value == 0 || Maze[i].value == 1)
				{
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(renderer, &Maze[i].rect);
				}
				else if (Maze[i].value == 2)
				{
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
					SDL_RenderFillRect(renderer, &Maze[i].rect);
				}
				else
				{
					SDL_SetRenderDrawColor(renderer, 124, 124, 124, 255);
					SDL_RenderFillRect(renderer, &Maze[i].rect);
				}
			}

			SDL_RenderPresent(renderer);

			timeEnd = SDL_GetTicks();
		}
	}
}