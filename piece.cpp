/***********************************************************************
 * Source File:
 *     PIECE
 ************************************************************************/

#include "piece.h"
#define NDEBUG
#include <cassert>

/**************************************************************
 * PIECE : DEFAULT CONSTRUCTOR
 * Initialize a Piece object with default values.
 *************************************************************/
Piece::Piece() : type(PieceType::SPACE), isWhite(false), lastMove(-1) {}

Piece::Piece(PieceType type, bool isWhite, int row, int col)
    : type(type), isWhite(isWhite), position(row, col), lastMove(-1) {}

Piece::~Piece() {}

/**************************************************************
 * PIECE : SIMPLE GETTERS
 * Retrieves type, isWhite, position, or LastMove
 *************************************************************/
PieceType Piece::getPieceType() const {
    return type;
}

bool Piece::getIsWhite() const {
    return isWhite;
}

const Position Piece::getPosition() const {
    return position;
}

void Piece::setLastMove(int currentMove) {
    lastMove = currentMove;
}

/**************************************************************
 * PIECE : GETTER FOR LETTER REPRESENTATION
 * Retrievesthe letter representation of the piece.
 * OUTPUT: string representing the piece (e.g., "K" for white king)
 *************************************************************/
char Piece::getLetter() const {
    switch (type) {
    case SPACE:
        return ' ';
    case KING:
        return isWhite ? 'K' : 'k'; 
    case QUEEN:
        return isWhite ? 'Q' : 'q'; 
    case ROOK:
        return isWhite ? 'R' : 'r'; 
    case BISHOP:
        return isWhite ? 'B' : 'b'; 
    case KNIGHT:
        return isWhite ? 'N' : 'n'; 
    case PAWN:
        return isWhite ? 'P' : 'p';
    default:
        return '?';
    }
}

/**************************************************************
 * PIECE : ASSIGNMENT OPERATOR OVERLOAD (PIECE)
 * Assign one Piece object to another.
 * INPUT: other - Piece object to assign from
 * OUTPUT: reference to the assigned Piece object
 *************************************************************/
Piece& Piece::operator=(const Piece& other) {
    if (this != &other) {
        this->type = other.type;
        this->isWhite = other.isWhite;
        this->position = other.position;
        this->lastMove = other.lastMove;
    }
    return *this;
}

/**************************************************************
 * PIECE : ASSIGNMENT OPERATOR OVERLOAD (POSITION)
 * Assign a new position to the piece.
 * INPUT: newPosition - Position object to assign
 * OUTPUT: reference to the modified Piece object
 *************************************************************/
Piece& Piece::operator=(const Position& newPosition) {
    this->position = newPosition;
    return *this;
}

/**************************************************************
 * PIECE : EQUALITY OPERATOR OVERLOAD
 * Compare two Piece objects for equality.
 * INPUT: other - Piece object to compare with
 * OUTPUT: true if the pieces are equal, false otherwise
 *************************************************************/
bool Piece::operator==(const Piece& other) const {
    return (this->type == other.type && this->isWhite == other.isWhite && this->lastMove == other.lastMove);
}

/**************************************************************
 * PIECE : INEQUALITY OPERATOR OVERLOAD
 * Compare two Piece objects for inequality.
 * INPUT: other - Piece object to compare with
 * OUTPUT: true if the pieces are not equal, false otherwise
 *************************************************************/
bool Piece::operator!=(const Piece& other) const {
    return !(*this == other);
}

/**************************************************************
 * PIECE SUBCLASSES : CONSTRUCTORS AND METHODS
 * Definitions for constructors and methods of Piece subclasses.
 * Includes definitions for Space, King, Queen, Rook, Bishop,
 * Knight, and Pawn classes.
 **************************************************************/
// SPACE
Space::Space(int row, int col) : Piece(PieceType::SPACE, false, row, col) {}

void Space::getMoves(set<Move>& possible, const Board& board) const {} // Space has no moves

void Space::display(ogstream* pgout) const {} // Space has no graphic

// KING
King::King(int row, int col, bool isWhite) : Piece(KING, isWhite, row, col) {}

void King::getMoves(set<Move>& possible, const Board& board) const {
    // Logic to calculate possible moves for the king
}

void King::display(ogstream* pgout) const {
    pgout->drawKing(position.getLocation(), isWhite);
}

// QUEEN
Queen::Queen(int row, int col, bool isWhite) : Piece(QUEEN, isWhite, row, col) {}

void Queen::getMoves(set<Move>& possible, const Board& board) const {
    // Logic to calculate possible moves for the queen
}

void Queen::display(ogstream* pgout) const {
    pgout->drawQueen(position.getLocation(), isWhite);
}

// ROOK
Rook::Rook(int row, int col, bool isWhite) : Piece(ROOK, isWhite, row, col) {}

void Rook::getMoves(set<Move>& possible, const Board& board) const {
    // Logic to calculate possible moves for the rook
}

void Rook::display(ogstream* pgout) const {
    pgout->drawRook(position.getLocation(), isWhite);
}

// BISHOP
Bishop::Bishop(int row, int col, bool isWhite) : Piece(BISHOP, isWhite, row, col) {}

void Bishop::getMoves(set<Move>& possible, const Board& board) const {
    // Logic to calculate possible moves for the bishop
}

void Bishop::display(ogstream* pgout) const {
    pgout->drawBishop(position.getLocation(), isWhite);
}

// KNIGHT
Knight::Knight(int row, int col, bool isWhite) : Piece(KNIGHT, isWhite, row, col) {}

void Knight::getMoves(set<Move>& possible, const Board& board) const {
    // Current position of the knight
    int row = position.getRow();
    int col = position.getCol();

    // Define the knight's possible moves relative to its current position
    static const vector<pair<int, int>> knightMoves = {
        {-1, -2}, {-2, -1}, 
        {-2,  1},        {-1,  2},
        {1,  -2},        {2,  -1}, 
        {2,   1}, {1,   2}
    };

    // Iterate over possible moves and check if they are valid
    for (const auto& move : knightMoves) {
        int newRow = row + move.first;
        int newCol = col + move.second;

        // Check if the new position is within the board bounds
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            const Piece* targetPiece = &board[Position(newRow, newCol)];
            if (targetPiece->getLetter() == ' ' || targetPiece->getIsWhite() != isWhite) {
                // If the target square is empty or contains an opponent's piece, add the move
                possible.insert(Move(position, Position(newRow, newCol)));
            }
        }
    }
}

void Knight::display(ogstream* pgout) const {
    pgout->drawKnight(position.getLocation(), isWhite);
}

// PAWN
Pawn::Pawn(int row, int col, bool isWhite) : Piece(PAWN, isWhite, row, col) {}

void Pawn::getMoves(set<Move>& possible, const Board& board) const {
    // Logic to calculate possible moves for the pawn
}

void Pawn::display(ogstream* pgout) const {
    pgout->drawPawn(position.getLocation(), isWhite);
}
