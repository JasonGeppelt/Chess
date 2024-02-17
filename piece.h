#pragma once

#include "pieceType.h"
#include "board.h"
#include "position.h"
#include "move.h"
#include "uiDraw.h"
#include <vector>
#include <set>

using namespace std;

class Piece {
public:
    // create and destroy the piece
    Piece();
    Piece(PieceType type, bool isWhite, int row, int col);
    ~Piece();

    // getters
    PieceType getPieceType() const;
    bool getIsWhite() const;
    char getLetter() const;
    const Position getPosition() const;
    virtual void getMoves(set<Move>& possible, const Board& board) const = 0;
    virtual void display(ogstream* pgout) const = 0;

    // setters
    void setLastMove(int currentMove);
    Piece& operator=(const Piece& other);
    Piece& operator=(const Position& newPosition);
    bool operator==(const Piece& other) const;
    bool operator!=(const Piece& other) const;

protected:
    PieceType type;
    bool isWhite;
    Position position;
    int lastMove;
};

class Space : public Piece {
public:
    Space(int row, int col);
    virtual void getMoves(set<Move>& possible, const Board& board) const override;
    virtual void display(ogstream* pgout) const override;
};

class King : public Piece {
public:
    King(int row, int col, bool isWhite);
    virtual void getMoves(set<Move>& possible, const Board& board) const override;
    virtual void display(ogstream* pgout) const override;
};

class Queen : public Piece {
public:
    Queen(int row, int col, bool isWhite);
    virtual void getMoves(set<Move>& possible, const Board& board) const override;
    virtual void display(ogstream* pgout) const override;
};

class Rook : public Piece {
public:
    Rook(int row, int col, bool isWhite);
    virtual void getMoves(set<Move>& possible, const Board& board) const override;
    virtual void display(ogstream* pgout) const override;
};

class Bishop : public Piece {
public:
    Bishop(int row, int col, bool isWhite);
    virtual void getMoves(set<Move>& possible, const Board& board) const override;
    virtual void display(ogstream* pgout) const override;
};

class Knight : public Piece {
public:
    Knight(int row, int col, bool isWhite);
    virtual void getMoves(set<Move>& possible, const Board& board) const override;
    virtual void display(ogstream* pgout) const override;
};

class Pawn : public Piece {
public:
    Pawn(int row, int col, bool isWhite);
    virtual void getMoves(set<Move>& possible, const Board& board) const override;
    virtual void display(ogstream* pgout) const override;
};

