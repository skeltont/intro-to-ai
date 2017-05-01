/*
 * MinimaxPlayer.h
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */

#ifndef MINIMAXPLAYER_H
#define MINIMAXPLAYER_H

#include "OthelloBoard.h"
#include "Player.h"
#include <vector>

struct successor {
	successor *parent;
	std::vector<successor*> children;
	OthelloBoard board;
	int col;
	int row;
	int value;
};

/**
 * This class represents an AI player that uses the Minimax algorithm to play the game
 * intelligently.
 */
class MinimaxPlayer : public Player {
public:

	/**
	 * @param symb This is the symbol for the minimax player's pieces
	 */
	MinimaxPlayer(char symb);

	/**
	 * Destructor
	 */
	virtual ~MinimaxPlayer();

	successor *find_goal_leaf(successor *head, successor *goal_leaf);
	successor *get_best_move_from_goal(successor *head, successor *goal);

	/**
	 * @param b The board object for the current state of the board
	 * @param col Holds the return value for the column of the move
	 * @param row Holds the return value for the row of the move
	 */
    void get_move(OthelloBoard* b, int& col, int& row);

		void generate_successors(successor *node, bool max_player);

		successor *initialize_successor(OthelloBoard *b, int col, int row);

    /**
     * @return A copy of the MinimaxPlayer object
     * This is a virtual copy constructor
     */
    MinimaxPlayer* clone();


		int eval(OthelloBoard *b);

private:
	successor *head;
};


#endif
