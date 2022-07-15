/* Author: Randal Obringer
* Date Last Modified: ~2011 
* Description: header for checkers game. 
*/

#ifndef CHECKERS_H
#define CHECKERS_H
#include "colors.h"
#include "game.h"
#include <iostream>
#include <string>
using namespace std;

//Inherit from game class
class checkers:public game{
	public:
		static const int ROWS=8; //# of rows on a checker board
		static const int COLUMNS=8; //# of columns on a checker board
		checkers();	//constructor
		string get_user_move( )const; //return the user's move
	protected:
		// *******************************************************************
		// VIRTUAL FUNCTIONS (these are overridden from the game base class)
		// *******************************************************************
		// Return a pointer to a copy of myself:
		game* clone( ) const;
		// Compute all the moves that the next player can make:
		void compute_moves(std::queue<std::string>& moves) const;
		// Display the status of the current game:
		void display_status( ) const;
		// Evaluate the current board position.
		// NOTE: Positive values are good for the computer.
		int evaluate( ) const;
		// Return true if the current game is finished:
		bool is_game_over( ) const;
		// Return true if the given move is legal for the next player:
		bool is_legal(const std::string& move) const;
		// Have the next player make a specified move:
		void make_move(const std::string& move);
		// Restart the game from the beginning:
		void restart( );

	private:
		string board[ROWS][COLUMNS];
};
#endif