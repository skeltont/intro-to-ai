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
	successor *goal_leaf = NULL, *best_move = NULL;

	head = initialize_successor(b, -1, -1);

 	build_minimax_tree(head, true);
	for (vector<successor*>::iterator it = head->children.begin(); it < head->children.end(); it++) {
		if (best_move == NULL || ((*it)->value > best_move->value)) {
			best_move = *it;
		}
	}

	col = best_move->col;
	row = best_move->row;
}

MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}

successor *MinimaxPlayer::initialize_successor(OthelloBoard *b, int col, int row) {
	successor *s = new successor;

	s->col = col;
	s->row = row;
	s->board = *b;
	s->value = -9999999;

	return s;
}

// int MinimaxPlayer::eval(OthelloBoard *b) {
float MinimaxPlayer::eval(successor *node) {
	int num_cols = node->board.get_num_cols(), num_rows = node->board.get_num_rows();
	float coins = 0, empty_count = 16, result = 0, max_corner = 0, min_corner = 0, max_moves = 0, min_moves = 0;
	char cell, min_symbol, max_symbol = this->get_symbol();

	if (max_symbol == 'X') {
		min_symbol = 'O';
	} else {
		min_symbol = 'X';
	}

	for (int c = 0; c < num_cols; c++) {
		for (int r = 0; r < num_rows; r++) {
			cell = node->board.get_cell(c, r);

			if (max_symbol == cell) {
				coins++;
				if (is_corner_cell(c,r))
					max_corner++;
			} else if (cell == '.') {
				empty_count--;
				if (node->board.is_legal_move(c,r,max_symbol))
					max_moves++;
				if (node->board.is_legal_move(c,r,min_symbol))
					min_moves++;
			} else {
				coins--;
				if (is_corner_cell(c,r))
					min_corner++;
			}
		}
	}

	/* OTHELLO EVALUATION HEURISTICS IN ORDER FROM LEAST TO MOST IMPORTANCE */

	// maximize our number of coins. coins aren't that important
	if (empty_count > 0)
		result += (coins / empty_count) * 1;

	// check for corner control node. corners are the only for sure safe spot on the table
	if ((max_corner + min_corner) > 0)
		result += ((max_corner - min_corner) / (max_corner + min_corner)) * 100;

	// count how many potential moves they have. This is very important because more moves
	// means that the opponent is losing
	if ((max_moves + min_moves) > 0)
		result += ((max_moves - min_moves) / (max_moves + min_moves)) * 1000;

	return result;
}

bool MinimaxPlayer::is_corner_cell(int col, int row) {
	if (col == 0 && row == 0) return true;
	if (col == 3 && row == 0) return true;
	if (col == 0 && row == 3) return true;
	if (col == 3 && row == 3) return true;

	return false;
}

void MinimaxPlayer::build_minimax_tree(successor *node, bool max_player) {
	int num_cols = node->board.get_num_cols(), num_rows = node->board.get_num_rows();
	char symbol = this->get_symbol();
	bool first = false;
	successor *temp = NULL;

	if(!max_player) {
		if (symbol == 'O') {
			symbol = 'X';
		} else {
			symbol = 'O';
		}
	}

	for (int c = 0; c < num_cols; c++) {
		for (int r = 0; r < num_rows; r++) {
			if (node->board.is_legal_move(c, r, symbol)) {
				temp = initialize_successor(&node->board, c, r);
				temp->board.play_move(c, r, symbol);
				temp->value = eval(temp);
				temp->parent = node;
				node->children.push_back(temp);
				build_minimax_tree(temp, !max_player);
			}
		}
	}

	if (!node->children.empty()) {
		for (vector<successor*>::iterator it = node->children.begin(); it < node->children.end(); it++) {
			if (!first || (max_player && (*it)->value > node->value) || (!max_player && (*it)->value < node->value)) {
				if (!first) first = true;
				node->value = (*it)->value;
			}
		}
	}
}
