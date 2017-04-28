/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include "MinimaxPlayer.h"

using std::vector;

MinimaxPlayer::MinimaxPlayer(char symb) : Player(symb) {
		head = new successor;
}

MinimaxPlayer::~MinimaxPlayer() {
}

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {
	successor *curr;

	head = initialize_successor(*b);
	curr = head;

 	generate_successors(curr, true);
	printf("got here lololol\n");
	col = 2;
	row = 3;
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}

successor *MinimaxPlayer::initialize_successor(OthelloBoard b) {
	successor *s = new successor;

	s->board = b;

	return s;
}

int MinimaxPlayer::eval(OthelloBoard *b) {
	int value = 0, num_cols = b->get_num_cols(), num_rows = b->get_num_rows();
	char cell, symbol = this->get_symbol();

	for (int c = 0; c < num_cols; c++) {
		for (int r = 0; r < num_rows; r++) {
			cell = b->get_cell(c, r);
			if (symbol == cell)
				value += 1;
			else if (cell == '.')
				continue;
			else
				value -= 1;
		}
	}

	return value;
}

void MinimaxPlayer::generate_successors(successor *node, bool swap) {
	int num_cols = node->board.get_num_cols(), num_rows = node->board.get_num_rows();
	char symbol = this->get_symbol();
	successor *temp;

	if(!swap){
		symbol = 'X';
	}

	printf("generating successors\n");

	for (int c = 0; c < num_cols; c++) {
		for (int r = 0; r < num_rows; r++) {
			if (node->board.is_legal_move(c, r, symbol)) {
				temp = initialize_successor(node->board);
				temp->board.play_move(c, r, symbol);
				temp->parent = node;
				node->children.push_back(temp);
				printf("valid move found \t\t: %d %d\n", c, r);
				generate_successors(temp, !swap);
			} else {
				printf("not valid move found\t\t: %d %d\n", c, r);
			}
		}
	}
}

// void MinimaxPlayer::parse_tree(successor *head) {
// 	// recursively parse this tree
// }
