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
        return ' ';
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

 /**************************************
  * SPACE
  **************************************/
Space::Space(int row, int col) : Piece(PieceType::SPACE, false, row, col) {}

void Space::getMoves(set<Move>& possible, const Board& board) const {} // Space has no moves
void Space::display(ogstream* pgout) const {} // Space has no graphic

// KING
King::King(int row, int col, bool isWhite) : Piece(KING, isWhite, row, col) {}

void King::getMoves(set<Move>& possible, const Board& board) const {
    static const vector<pair<int, int>> kingMoves = {
        {0, 1}, {1, 0}, {1, 1},
        {0, -1}, {-1, 0}, {-1, -1},
        {1, -1}, {-1, 1}
    };
    int row = position.getRow();
    int col = position.getCol();

    for (const auto& move : kingMoves) {
        int newRow = row + move.first;
        int newCol = col + move.second;
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
            const Piece* targetPiece = &board[Position(newRow, newCol)];
            if (targetPiece->getLetter() == ' ' || targetPiece->getIsWhite() != isWhite) {
                possible.insert(Move(position, Position(newRow, newCol)));
            }
        }
    }

    // Check if the squares between the king and the rooks are empty
    bool canCastleKingSide = board[Position(row, col + 1)].getPieceType() == SPACE &&
        board[Position(row, col + 2)].getPieceType() == SPACE;

    bool canCastleQueenSide = board[Position(row, col - 1)].getPieceType() == SPACE &&
        board[Position(row, col - 2)].getPieceType() == SPACE &&
        board[Position(row, col - 3)].getPieceType() == SPACE;

    // Iterate over adjacent squares and add valid moves
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr != 0 || dc != 0) { // Exclude the king's current position
                int newRow = row + dr;
                int newCol = col + dc;
                if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8) {
                    const Piece* targetPiece = &board[Position(newRow, newCol)];
                    if (targetPiece->getPieceType() == SPACE || targetPiece->getIsWhite() != isWhite) {
                        possible.insert(Move(position, Position(newRow, newCol)));
                    }
                }
            }
        }
    }

    // Add castling moves if conditions are met
    if (canCastleKingSide) {
        possible.insert(Move(position, Position(row, col + 2), KING));
    }
    if (canCastleQueenSide) {
        possible.insert(Move(position, Position(row, col - 2), KING));
    }
}

void King::display(ogstream* pgout) const {
    pgout->drawKing(position.getLocation(), isWhite);
}

// QUEEN
Queen::Queen(int row, int col, bool isWhite) : Piece(QUEEN, isWhite, row, col) {}

void Queen::getMoves(set<Move>& possible, const Board& board) const {
    // Combine Rook and Bishop directions
    vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    int row = position.getRow(), col = position.getCol();

    for (const auto& dir : directions) {
        int nextRow = row, nextCol = col;
        while (true) {
            nextRow += dir.first;
            nextCol += dir.second;
            if (nextRow < 0 || nextRow >= 8 || nextCol < 0 || nextCol >= 8) break;
            const Piece& target = board[Position(nextRow, nextCol)];
            if (target.getPieceType() == SPACE) {
                possible.insert(Move(position, Position(nextRow, nextCol)));
            }
            else {
                if (target.getIsWhite() != isWhite) {
                    possible.insert(Move(position, Position(nextRow, nextCol)));
                }
                break;
            }
        }
    }
}

void Queen::display(ogstream* pgout) const {
    pgout->drawQueen(position.getLocation(), isWhite);
}

// ROOK
Rook::Rook(int row, int col, bool isWhite) : Piece(ROOK, isWhite, row, col) {}

void Rook::getMoves(set<Move>& possible, const Board& board) const {
    // Directions: up, down, left, right
    vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    int row = position.getRow(), col = position.getCol();

    for (const auto& dir : directions) {
        int nextRow = row, nextCol = col;
        while (true) {
            nextRow += dir.first;
            nextCol += dir.second;
            if (nextRow < 0 || nextRow >= 8 || nextCol < 0 || nextCol >= 8) break; // Out of bounds
            const Piece& target = board[Position(nextRow, nextCol)];
            if (target.getPieceType() == SPACE) { // Empty space
                possible.insert(Move(position, Position(nextRow, nextCol)));
            }
            else {
                if (target.getIsWhite() != isWhite) { // Capture
                    possible.insert(Move(position, Position(nextRow, nextCol)));
                }
                break; // Blocked by a piece
            }
        }
    }
}

void Rook::display(ogstream* pgout) const {
    pgout->drawRook(position.getLocation(), isWhite);
}

// BISHOP
Bishop::Bishop(int row, int col, bool isWhite) : Piece(BISHOP, isWhite, row, col) {}

void Bishop::getMoves(set<Move>& possible, const Board& board) const {
    // Directions: diagonals
    vector<pair<int, int>> directions = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    int row = position.getRow(), col = position.getCol();

    for (const auto& dir : directions) {
        int nextRow = row, nextCol = col;
        while (true) {
            nextRow += dir.first;
            nextCol += dir.second;
            if (nextRow < 0 || nextRow >= 8 || nextCol < 0 || nextCol >= 8) break;
            const Piece& target = board[Position(nextRow, nextCol)];
            if (target.getPieceType() == SPACE) {
                possible.insert(Move(position, Position(nextRow, nextCol)));
            }
            else {
                if (target.getIsWhite() != isWhite) {
                    possible.insert(Move(position, Position(nextRow, nextCol)));
                }
                break;
            }
        }
    }
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
    int direction = isWhite ? -1 : 1; // Adjust direction based on pawn color
    int startRow = isWhite ? 6 : 1; // Starting rows differ based on color
    int row = position.getRow();
    int col = position.getCol();

    // Forward one space
    if (board[Position(row + direction, col)].getPieceType() == SPACE) {
        possible.insert(Move(position, Position(row + direction, col)));
        // Double move from start position
        if (row == startRow && board[Position(row + 2 * direction, col)].getPieceType() == SPACE) {
            possible.insert(Move(position, Position(row + 2 * direction, col)));
        }
    }

    // En-passant
    if (!board.getMoveHistory().empty()) {
        // Check if the move meets the conditions for en passant
        const Move& lastMove = board.getLastMove();
        if (board[lastMove.getDes()].getPieceType() == PAWN &&
            abs(lastMove.getDes().getCol() - col) == 1 &&
            abs(lastMove.getDes().getRow() == row &&
            abs(lastMove.getSrc().getRow() - row) == 2))
        {
            // Add en passant move to the possible set
            possible.insert(Move(position, Position(row + direction, lastMove.getDes().getCol())));
        }
    }
}

void Pawn::display(ogstream* pgout) const {
    pgout->drawPawn(position.getLocation(), isWhite);
}
