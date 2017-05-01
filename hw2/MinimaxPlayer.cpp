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

 	generate_successors(head, true);

	goal_leaf = find_goal_leaf(head, goal_leaf);
	best_move = get_best_move_from_goal(head, goal_leaf);

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

void MinimaxPlayer::generate_successors(successor *node, bool max_player) {
	int num_cols = node->board.get_num_cols(), num_rows = node->board.get_num_rows();
	char symbol = this->get_symbol();
	successor *temp = NULL;

	if(!max_player){
		symbol = 'X';
	}

	for (int c = 0; c < num_cols; c++) {
		for (int r = 0; r < num_rows; r++) {
			if (node->board.is_legal_move(c, r, symbol)) {
				temp = initialize_successor(&node->board, c, r);
				temp->board.play_move(c, r, symbol);
				temp->value = eval(&temp->board);
				temp->parent = node;
				node->children.push_back(temp);
				generate_successors(temp, !max_player);
			}
		}
	}
}

successor *MinimaxPlayer::find_goal_leaf(successor *node, successor *goal_leaf) {
	if ((node->children.empty()) && (goal_leaf == NULL || node->value > goal_leaf->value)) {
		goal_leaf = node;
	} else {
		for (vector<successor*>::iterator it = node->children.begin(); it < node->children.end(); it++) {
			goal_leaf = find_goal_leaf(*it, goal_leaf);
		}
	}

	return goal_leaf;
}

successor *MinimaxPlayer::get_best_move_from_goal(successor *head, successor *node) {
	if (node->parent == head) {
		return node;
	}
	return get_best_move_from_goal(head, node->parent);
}
