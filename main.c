#include <stdio.h>
#include "k.h"

char c;

int main() {
	struct game new_game;
	//initialize the game board to have white space
	for (int row = 0; row < SIZE; row++) {
		for (int col = 0; col < SIZE; col++) {
			new_game.board[row][col] = ' ';
		}
	}
	new_game.score = 0;
	add_random_tile(&new_game);

	while (is_move_possible(new_game) == true && is_game_won(new_game) == false) {
		//waits for the user input then based on the input the game will update according to the wasd
		//any other input other than wasd will not change the game state
		render(new_game);
		c = getchar();
		switch (c) {
		case 'w':
			update(&new_game, -1, 0);
			break;
		case 'a':
			update(&new_game, 0, -1);
			break;
		case 's':
			update(&new_game, 1, 0);
			break;
		case 'd':
			update(&new_game, 0, 1);
			break;
		default:
		break;
		}
		while ((c = getchar()) != '\n' && c != EOF);
	}
	return 0;
}
