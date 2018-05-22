#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "k.h"
#include <time.h>


void add_random_tile(struct game *game){
	int row, col;
	srand(time(NULL));
	// find random, but empty tile
	do{
		row = rand() % 4;
		col = rand() % 4;
	}while(game->board[row][col] != ' ');

	// place to the random position 'A' or 'B' tile
	int tile = 'A' + (rand() % 2);
	game->board[row][col] = tile;
}


//iterates through the 2d array and checks for a 'K'
bool is_game_won(const struct game game){
	for(int row = 0; row < SIZE; row++){
		for(int col = 0; col < SIZE; col++){
			if(game.board[row][col] == 'K'){
				printf("Congratulations! You are a winner!");
				return true;
			}
		}
	}
	return false;
}

//checks if move is possible and returns true if possible
bool is_move_possible(const struct game game){
	//if there is an empty space in 2d array
	for(int row = 0; row < SIZE; row++){
		for(int col = 0; col < SIZE; col++){
			if(game.board[row][col] == ' '){
				return true;
			}
		}
	}
	//if there are 2 adjacent values in 2d array : vertically , horizontally
	for(int row = 0; row < SIZE; row++){
		for(int col = 0; col < SIZE-1; col++){
			//first half of if statement iterates through checking if there are vertically adjacent
			//values in the game statement. The second half checks horizontally.
			if(game.board[col][row] == game.board[col+1][row] ||
				game.board[row][col] == game.board[row][col+1]){
				return true;
			}
		}
	}
	return false;
}


//constructs the game field and prints the game state of the 2d array
void render(const struct game game) {
	//draws the game field
	printf("Score : %d\n", game.score);
	for (int i = 0; i < SIZE * 2 + 1; i++) {
		if (i % 2 == 0) {
			printf("+");
		} else {
			printf("-");
		}
	}
	printf("\n");
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			printf("|");
			printf("%c", game.board[row][col]);
		}
		printf("|\n");
		for (int i = 0; i < SIZE * 2 + 1; i++) {
			if (i % 2 == 0) {
				printf("+");
			} else {
				printf("-");
			}
		}
		printf("\n");
	}
}


//rotates the game board in order to help squish the board
void rotateClockWise(struct game *game) {
	//creates temporary 2d array and copies values from actual game board
	char temp[SIZE][SIZE];
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			temp[row][col] = game->board[row][col];
		}
	}
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			game->board[row][col] = temp[SIZE - 1 - col][row];
		}
	}
}


//squishes only to the right but can squish in all directions
//with the help of rotateClockWise, combines the elements
//and pushes to the selected direction
void squish(struct game *game) {
	//shift all elements to the right for each row
	for (int row = 0; row < SIZE; row++) {
		//keeps shifting elements in the column until they are at their rightmost position
		for (int i = 0; i < SIZE - 1; i++) {
			for (int col = 0; col < SIZE - 1; col++) {
				if (game->board[row][col + 1] == ' ') {
					char temp[SIZE][SIZE];
					temp[row][col] = game->board[row][col + 1];
					game->board[row][col + 1] = game->board[row][col];
					game->board[row][col] = temp[row][col];
				}
			}
		}
	}
	//combines the adjacent elements by incrementing the value
	for (int row = 0; row < SIZE; row++) {
		for (int col = SIZE - 1; col > 0; col--) {
			if (game->board[row][col - 1] == game->board[row][col]
					&& game->board[row][col - 1] != ' '
					&& game->board[row][col] != ' ') {
				game->board[row][col]++;
				int letter = game->board[row][col];
				int value = pow(2, (letter - 64));
				game->score += value;
				game->board[row][col-1] = ' ';
			}
		}
	}
	//shifts the elements again to the right for each row
	for (int row = 0; row < SIZE; row++) {
		//keeps shifting elements in the column until they are at their rightmost position
		for (int i = 0; i < SIZE - 1; i++) {
			for (int col = 0; col < SIZE - 1; col++) {
				if (game->board[row][col + 1] == ' ') {
					char temp[SIZE][SIZE];
					temp[row][col] = game->board[row][col + 1];
					game->board[row][col + 1] = game->board[row][col];
					game->board[row][col] = temp[row][col];
				}
			}
		}
	}
}

//checks if move is possible, if possible then update game board state
//adds random tile only if updated
bool update(struct game *game, int dy, int dx) {
	//switch case for the vertical movement
	switch (dy) {
	//going up
	case -1:
		rotateClockWise(game);
		squish(game);
		rotateClockWise(game);
		rotateClockWise(game);
		rotateClockWise(game);
		add_random_tile(game);
		return true;
	//going down
	case 1:
		rotateClockWise(game);
		rotateClockWise(game);
		rotateClockWise(game);
		squish(game);
		rotateClockWise(game);
		add_random_tile(game);
		return true;
	}

	//switch case for the horizontal movement
	switch (dx) {
	//going left
	case -1:
		rotateClockWise(game);
		rotateClockWise(game);
		squish(game);
		rotateClockWise(game);
		rotateClockWise(game);
		add_random_tile(game);
		return true;
	//going right
	case 1:
		squish(game);
		add_random_tile(game);
		return true;
	}

	return false;
}
