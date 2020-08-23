#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define NAME_OF_GAME "Snake Game"

// size of field
#define FIELD_LENGHT 60 
#define FIELD_HEIGHT 20

// sprites of
#define FIELD_BLOCK '#'
#define SNAKE_MODEL_HEAD '0'
#define SNAKE_MODEL_BODY 'o'
#define FRUIT_MODEL '@'

#define ESCAPE 27 // exit the game

static enum bool{ false, true } gameOver; // the variable is responsible for continuing the game
static enum move { STOP, UP = 'w', DOWN = 's', RIGHT = 'd', LEFT = 'a' } moveCharacter; // the variable is responsible for move of character

static struct Snake { // structure of snake that has all information about the coordinates
	int unitPositionX[(FIELD_HEIGHT - 1) * (FIELD_LENGHT - 1)];
	int unitPositionY[(FIELD_HEIGHT - 1) * (FIELD_LENGHT - 1)];
	size_t lenght;
} snake; // the static variable snake

static struct Fruit { // structure of fruit that has all information about the coordinates
	int positionX;
	int positionY;
} fruit; // the static variable fruit

static size_t score; // the variable is responsible for the score

void draw_field();
void calculating_coordinates();
inline void control_character();
inline void new_game();
inline void setting_up();
inline void printStr_gameOver();


int main() {
	srand(time(NULL)); // random for the newxt new fruit

	new_game();

	return 0;
}

inline void new_game() { // start a new game
	setting_up();

	while (!gameOver) { // the continuation of the game until the gameOver is false
		draw_field();
		control_character();
		calculating_coordinates();
	}

	printStr_gameOver();
}

inline void setting_up() { // setting up a new game, initializing default variables
	gameOver = false;

	// setting up a snake
	snake.unitPositionX[0] = FIELD_LENGHT / 2;
	snake.unitPositionY[0] = FIELD_HEIGHT / 2;
	snake.lenght = 1;

	// setting up the fruit
	fruit.positionX = 1 + rand() % (FIELD_LENGHT - 1);
	fruit.positionY = 1 + rand() % (FIELD_HEIGHT - 1);

	// setting up default
	moveCharacter = STOP;
	score = 0;

	// setting up the name of the game
	SetConsoleTitle(NAME_OF_GAME);
	HANDLE hStdOut;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ccInfo;
	ccInfo.bVisible = 0;
	ccInfo.dwSize = 10;
	SetConsoleCursorInfo(hStdOut, &ccInfo);
}

void draw_field() { // the function draws a field and all sprites
	for (size_t i = 0; i <= FIELD_HEIGHT; i++) {
		for (size_t j = 0; j <= FIELD_LENGHT; j++) {
			if (i == 0 || i == FIELD_HEIGHT) { // drawing the field
				printf("%c", FIELD_BLOCK);
			}
			else if (j == 0 || j == FIELD_LENGHT) { // drawing the field
				printf("%c", FIELD_BLOCK);
			}
			else if (i == snake.unitPositionY[0] && j == snake.unitPositionX[0]) { // drawing a snake
				printf("%c", SNAKE_MODEL_HEAD);
			}
			else if (i == fruit.positionY && j == fruit.positionX) { // drawing a fruit
				printf("%c", FRUIT_MODEL);
			}
			else {
				enum bool ind = false;
				for (size_t k = 1; k < snake.lenght; k++) { // drawing the snake tail
					if (snake.unitPositionX[k] == j && snake.unitPositionY[k] == i) {
						printf("%c", SNAKE_MODEL_BODY);
						ind = true;
					}
				}
				if (!ind) { // if the tail was not drawn
					printf(" ");
				}
			}
			if (i == FIELD_HEIGHT / 2 && j == FIELD_LENGHT) { // output the score
				printf("\tScore: %d", score);
			}
		}
		printf("\n");
	}
	system("cls");
}

inline void control_character() { // the function is responsible for controlling the character
	if (_kbhit()) { // if the keyboard key was pressed
		switch (_getch()) { // the keyboard key code reading
		case (int)LEFT:
			moveCharacter = LEFT;
			break;
		case (int)UP:
			moveCharacter = UP;
			break;
		case (int)RIGHT:
			moveCharacter = RIGHT;
			break;
		case (int)DOWN:
			moveCharacter = DOWN;
			break;
		case ESCAPE: // exit the game
			exit(1);
			break;
		default:
			break;
		}
	}
}

void calculating_coordinates() {

	for (size_t i = snake.lenght; i > 0; i--) { // machining the snake tail
		snake.unitPositionX[i] = snake.unitPositionX[i - 1];
		snake.unitPositionY[i] = snake.unitPositionY[i - 1];
	}

	switch (moveCharacter) // movement of the snake in the direction
	{
	case LEFT:
		snake.unitPositionX[0]--;
		break;
	case UP:
		snake.unitPositionY[0]--;
		break;
	case RIGHT:
		snake.unitPositionX[0]++;
		break;
	case DOWN:
		snake.unitPositionY[0]++;
		break;
	default:
		break;
	}

	for (size_t i = 1; i < snake.lenght; i++) { // testing the snake to collide with itself
		if (snake.unitPositionX[0] == snake.unitPositionX[i] && snake.unitPositionY[0] == snake.unitPositionY[i]) {
			gameOver = true;
		}
	}

	// if the snake faces the wall, it comes out the other side
	if (snake.unitPositionX[0] < 1) { //  faces the wall  |<-
		snake.unitPositionX[0] = FIELD_LENGHT - 1;
	}
	else if (snake.unitPositionX[0] > FIELD_LENGHT - 1) { //  faces the wall  ->|
		snake.unitPositionX[0] = 1;
	}
	else if (snake.unitPositionY[0] < 1) { //  faces the wall  /\ 
		snake.unitPositionY[0] = FIELD_HEIGHT - 1;
	}
	else if (snake.unitPositionY[0] > FIELD_HEIGHT - 1) { //  faces the wall  \/ 
		snake.unitPositionY[0] = 1;
	}

	if (snake.unitPositionX[0] == fruit.positionX && snake.unitPositionY[0] == fruit.positionY) { // eating fruit
	  // a new random po³ition for the next fruit
		fruit.positionX = 1 + rand() % (FIELD_LENGHT - 1);
		fruit.positionY = 1 + rand() % (FIELD_HEIGHT - 1);

		// increase in tail and score
		snake.lenght++;
		score++;
	}
}

inline void printStr_gameOver() { // print Game Over
	char mas[6][57] = {
		{' ',' ',' ','_','_','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','_','_','_','_',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n','\0'},
		{' ',' ','/',' ',' ','_','_','_','_','|',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','/',' ','_','_',' ','\\',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','\n','\0'},
		{' ',' ','|',' ','|',' ',' ','_','_',' ',' ','_','_',' ','_',' ','_',' ','_','_',' ','_','_','_',' ',' ',' ','_','_','_',' ',' ','|',' ','|',' ',' ','|',' ','|','_',' ',' ',' ','_','_','_','_','_',' ','_',' ','_','_',' ','\n','\0'},
		{' ',' ','|',' ','|',' ','|','_',' ','|','/',' ','_','`',' ','|',' ','\'','_',' ','`',' ','_',' ','\\',' ','/',' ','_',' ','\\',' ','|',' ','|',' ',' ','|',' ','\\',' ','\\',' ','/',' ','/',' ','_',' ','\\',' ','\'','_','_','|','\n','\0'},
		{' ',' ','|',' ','|','_','_','|',' ','|',' ','(','_','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ','|',' ',' ','_','_','/',' ','|',' ','|','_','_','|',' ','|','\\',' ','v',' ','/',' ',' ','_','_','/',' ','|',' ',' ',' ','\n','\0'},
		{' ',' ','\\','_','_','_','_','_','_','|','\\','_','_',',','_','|','_','|',' ','|','_','|',' ','|','_','|','\\','_','_','_','|',' ',' ','\\','_','_','_','_','/',' ',' ','\\','_','/',' ','\\','_','_','_','|','_','|',' ',' ',' ','\n','\0'}
	}; // the array containing strings

	system("cls");
	printf("\n\n");
	for (int i = 0; i < 6; i++) { // print array
		printf("%s", mas[i]);
	}
	printf("\n\n\n\t      ");
	system("pause");
}
