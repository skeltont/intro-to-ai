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
	float value;
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

	/**
	 * @param b The board object for the current state of the board
	 * @param col Holds the return value for the column of the move
	 * @param row Holds the return value for the row of the move
	 */
    void get_move(OthelloBoard* b, int& col, int& row);

		/*
		 * @param node is the current node in the successor tree, since the call is recursive
		 * @param max_player tells the tree level if it's a maximizing or minimizing step
		 * Description: Build the tree by expanding potential moves (children) at every node.
		 * This function does the part of building the successor tree and bubbling back the
		 * minimax values recursively.
		 */
		void build_minimax_tree(successor *node, bool max_player);

		/*
		 * @param b	The board object for the current state of the board
		 * @param col the column for the move that would create that successor state
		 * @param row the row for the move that would create that successor state
		 * Description: Builds the successor object that describes the nodes of the successor tree
		 */
		successor *initialize_successor(OthelloBoard *b, int col, int row);

		/*
		 * @param col column position
		 * @param row row position
		 * Description: return if position is a corner cell
		 */
		bool is_corner_cell(int col, int row);

		/*
		 * @param node is a pointer to a specific node in the tree
		 * Description: evaluate a node's board state for how many coins, number of corners, and available moves.
		 */
		float eval(successor *node);

    /**
     * @return A copy of the MinimaxPlayer object
     * This is a virtual copy constructor
     */
    MinimaxPlayer* clone();


private:
	successor *head;
};


#endif
