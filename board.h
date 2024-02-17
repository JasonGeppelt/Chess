// Header File:
// BOARD: Represents the game board for Chess
// 
// Summary:
//    A collection of pieces and a small amount of game state
// 

#pragma once

#include "position.h" // for POSITION: how we locate pieces
#include "piece.h"    // for PIECE: what the board consists of
#include "move.h"     // for MOVE: how we move pieces around
#include "uiDraw.h"
#include "uiInteract.h"
#include <iostream>
#include <set>

using namespace std;

namespace Lab06UnitTest
{
	class TestBoard;
	class TestPawn;
	class TestBishop;
	class TestKnight;
	class TestRook;
	class TestKing;
	class TestQueen;
}

class Piece; // Forward declaration

// BOARD
// The game board
class Board
{
public:
	friend Lab06UnitTest::TestBoard;
	friend Lab06UnitTest::TestPawn;
	friend Lab06UnitTest::TestBishop;
	friend Lab06UnitTest::TestKnight;
	friend Lab06UnitTest::TestRook;
	friend Lab06UnitTest::TestKing;
	friend Lab06UnitTest::TestQueen;

	// create and destroy the board
	Board(ogstream* pgout = nullptr, bool noReset = false);
	~Board();

	// getters
	int getCurrentMove() const { return currentMove;		   }
	bool whiteTurn() const { return getCurrentMove() % 2 == 1; }
	void display(const Position source, const Interface& ui, const set<Move>& possible) const;
	const Piece& operator [] (const Position& pos) const
	{
		return *board[pos.getRow()][pos.getCol()];
	}

	// setters
	void free();
	virtual void reset(bool fFree = true);
	bool move(const Move& move);
	void operator -= (const Position& pos);
	void operator -= (const Move& move);
	void remove(const Position& pos);
	const Piece* operator = (Piece* pP);
	Piece& operator [] (const Position& pos)
	{
		return *board[pos.getRow()][pos.getCol()];
	}
	void swap(const Position& pos1, const Position& pos2);
	void setCurrentMove(int currentMove) { this->currentMove = currentMove; }
	static Piece* pawnFactory(int row, int col, bool isWhite);

protected:
	void assertBoard();

	Piece* board[8][8]; // the board of chess pieces
	int currentMove;    // the current move number we are on
	ogstream* pgout;     // the output stream
};


