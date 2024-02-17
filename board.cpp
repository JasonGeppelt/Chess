/***********************************************************************
 * BOARD Source File
 ************************************************************************/

#include "board.h"
#define NDEBUG
#include <cassert>
using namespace std;

Board::Board(ogstream* pgout, bool noReset) : currentMove(-1), pgout(pgout)
{
    if (!noReset)
        reset(false);
}

Board::~Board()
{
    free();
}

/**************************************************************
 * BOARD : DISPLAY
 * Display the board
 *************************************************************/
void Board::display(const Position source, const Interface& ui, const set<Move>& possible) const
{
    // draw the base board
    pgout->drawBoard();

    // draw the hover and select stuff
    pgout->drawHover(ui.getHoverPosition());
    pgout->drawSelected(ui.getSelectPosition());

    // draw the possible moves
    set<Move> ::iterator it;
    for (it = possible.begin(); it != possible.end(); ++it)
        pgout->drawPossible(it->getDes().getLocation());

    // draw the pieces
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            board[r][c]->display(pgout);
}

/**************************************************************
 * BOARD : PAWN FACTORY
 * Pawn creater for less redundant code
 *************************************************************/
Piece* Board::pawnFactory(int row, int col, bool isWhite) {
    return new Pawn(row, col, isWhite);
}

/**************************************************************
 * BOARD : RESET
 * Just fill the board with the known pieces
 *************************************************************/
void Board::reset(bool fFree)
{
    // free up allocated memory if commanded to
    if (fFree)
        free();

    // middle of the board
    for (int r = 2; r < 6; r++)
        for (int c = 0; c < 8; c++)
            board[r][c] = new Space(r, c);

    // pawns
    for (int c = 0; c < 8; c++)
    {
        board[1][c] = pawnFactory(1, c, false);
        board[6][c] = pawnFactory(6, c, true);
    }

    // black
    board[0][0] = new Rook(0, 0, false);
    board[0][1] = new Knight(0, 1, false);
    board[0][2] = new Bishop(0, 2, false);
    board[0][3] = new Queen(0, 3, false);
    board[0][4] = new King(0, 4, false);
    board[0][5] = new Bishop(0, 5, false);
    board[0][6] = new Knight(0, 6, false);
    board[0][7] = new Rook(0, 7, false);
    // white
    board[7][0] = new Rook(7, 0, true);
    board[7][1] = new Knight(7, 1, true);
    board[7][2] = new Bishop(7, 2, true);
    board[7][3] = new Queen(7, 3, true);
    board[7][4] = new King(7, 4, true);
    board[7][5] = new Bishop(7, 5, true);
    board[7][6] = new Knight(7, 6, true);
    board[7][7] = new Rook(7, 7, true);

    // reset the moves
    currentMove = 0;
    assertBoard();
}

/**************************************************************
 * BOARD : FREE
 * Free up all the allocated memory
 *************************************************************/
void Board::free()
{
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
            delete board[r][c];
}

/**************************************************************
 * BOARD : -=
 * Delete a piece at a given position
 * INPUT pos   The position to delete
 *************************************************************/
void Board::operator -= (const Position& pos)
{
    if (pos.isInvalid())
        return;

    delete board[pos.getRow()][pos.getCol()];
    board[pos.getRow()][pos.getCol()] = new Space(pos.getRow(), pos.getCol());
}

/**************************************************************
 * BOARD : DELETE
 * Delete a piece at a given position AND
 * do not allocate a space
 * INPUT pos   The position to delete
 *************************************************************/
void Board::remove(const Position& pos)
{
    if (pos.isInvalid())
        return;

    delete board[pos.getRow()][pos.getCol()];
    board[pos.getRow()][pos.getCol()] = NULL;
}

/**************************************************************
 * BOARD assign
 * Assign a new piece to the board
 * INPUT pPhs   The new piece to assign to the position
 *************************************************************/
const Piece* Board::operator = (Piece* pPhs)
{
    delete board[pPhs->getPosition().getRow()][pPhs->getPosition().getCol()];
    board[pPhs->getPosition().getRow()][pPhs->getPosition().getCol()] = pPhs;
    return pPhs;
}

/**************************************************************
 * BOARD UNDO
 * Back up one move. This is done by fetching the
 * last move off the move list and executing it
 * backwards
 *************************************************************/
void Board::operator -= (const Move& move)
{
    Position src = move.getSrc();
    Position des = move.getDes();

    // Castle king side
    if (move.getCastleK())
    {
        assert(false);
    }

    // Castle queen side
    else if (move.getCastleQ())
    {
        assert(false);
    }

    // en-passant
    else if (move.getEnPassant())
    {
        assert(false);
    }

    // promotion
    else if (move.getPromotion() != SPACE)
    {
        assert(false);
    }

    // capture?
    else if (move.getCapture() != SPACE)
    {
        assert(false);
    }

    // standard move
    else
    {
        swap(src, des);
    }
}

/**************************************************************
 * BOARD : assertBoard
 * verify the poard is well-formed
 *************************************************************/
void Board::assertBoard()
{
#if defined NDEBUG
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++)
        {
            Position pos = board[r][c]->getPosition();
            assert(pos.getRow() == r);
            assert(pos.getCol() == c);
        }

    assert(currentMove >= 0);
#endif // NDEBUG
}

/**************************************************************
 * BOARD : SWAP
 * Swap two pieces on the board
 * INPUT pos1-pos2   The two positions to swap on the board
 *************************************************************/
void Board::swap(const Position& pos1, const Position& pos2)
{
    assertBoard();

    assert(pos1.isValid());
    assert(pos2.isValid());

    // perform the swap from the board's perspective
    Piece* p = board[pos1.getRow()][pos1.getCol()];
    board[pos1.getRow()][pos1.getCol()] = board[pos2.getRow()][pos2.getCol()];
    board[pos2.getRow()][pos2.getCol()] = p;

    // update the position of the piece so the piece is where
    // it thinks it is. Otherwise, it is lost!
    (*this)[pos1] = pos1;
    (*this)[pos2] = pos2;

    // mark that this piece has moved
    (*this)[pos1].setLastMove(currentMove);
    (*this)[pos2].setLastMove(currentMove);

    // make sure everything worked correctly
    assertBoard();
}

/**************************************************************
 * BOARD : MOVE
 * Execute a move according to the contained instructions
 * INPUT move The instructions of the move
 *************************************************************/
bool Board::move(const Move& move)
{
    bool moveOccurred = false;

    Position src = move.getSrc();
    Position des = move.getDes();

    assert(board[src.getRow()][src.getCol()]->getLetter() != ' ');
    assert(src.isValid());
    assert(des.isValid());

    // Not your turn
    if (whiteTurn() != board[src.getRow()][src.getCol()]->getIsWhite()) {
        return false;
    }

    // Destination occupied 
    if (move.getCapture() != SPACE 
        && board[des.getRow()][des.getCol()]->getIsWhite() == board[src.getRow()][src.getCol()]->getIsWhite() 
        && !move.getCastleK() && !move.getCastleQ()) 
    {
        return false;
    }

    // Castle King side
    if (move.getCastleK())
    {
        int row = (move.getWhiteMove() ? 7 : 0);

        // move the king
        src.set(row, 6);
        des.set(row, 4);
        swap(src, des);

        // move the rook
        src.set(row, 5);
        des.set(row, 7);
        swap(src, des);
        
        // DEBUG 
        cout << "King Side Castle" << endl;

        moveOccurred = true;
    }

    // Castle Queen side
    else if (move.getCastleQ())
    {
        int row = (move.getWhiteMove() ? 7 : 0);

        // move the king
        src.set(row, 2);
        des.set(row, 4);
        swap(src, des);

        // move the rook
        src.set(row, 3);
        des.set(row, 0);
        swap(src, des);

        // DEBUG 
        cout << "Queen Side Castle" << endl;
        
        moveOccurred = true;
    }

    // en-passant
    else if (!moves.empty() && move.getEnPassant())
    {
        // Get the position of the captured pawn
        Position capturedPawnPos = moves.back().getDes();

        // Remove the captured pawn from the board
        *this -= capturedPawnPos;

        // Move your pawn to the destination square
        swap(src, des);

        // DEBUG 
        cout << "En-passant" << endl;
        
        moveOccurred = true;
    }

    // promotion?
    else if (move.getPromotion() != SPACE) {
        // Replace pawn with new piece at destination based on promotion
        Position dest = move.getDes();
        bool isWhite = move.getWhiteMove();
        Piece* promotedPiece = nullptr;

        switch (move.getPromotion()) {
        case QUEEN:
            promotedPiece = new Queen(dest.getRow(), dest.getCol(), isWhite);
            break;
        case ROOK:
            promotedPiece = new Rook(dest.getRow(), dest.getCol(), isWhite);
            break;
        case BISHOP:
            promotedPiece = new Bishop(dest.getRow(), dest.getCol(), isWhite);
            break;
        case KNIGHT:
            promotedPiece = new Knight(dest.getRow(), dest.getCol(), isWhite);
            break;
        default:
            assert(false);
            break;
        }

        if (promotedPiece != nullptr) {
            // Assuming board is a 2D array of Piece pointers
            Position posKill(src.getRow(), des.getCol());
            *this -= posKill;            
            board[dest.getRow()][dest.getCol()] = promotedPiece; // Place the new piece
        }
        
        // DEBUG 
        cout << "Promotion" << endl;

        moveOccurred = true;
    }

    // capture?
    else if (move.getCapture() != SPACE)
    {
        assert(board[des.getRow()][des.getCol()]->getLetter() != ' ');
        assert(board[src.getRow()][src.getCol()]->getLetter() != ' ');
        assert(board[src.getRow()][src.getCol()]->getIsWhite() == move.getWhiteMove());
        assert(board[des.getRow()][des.getCol()]->getIsWhite() != move.getWhiteMove());

        // delete the piece
        *this -= des;

        // swap the piece
        swap(src, des);
        
        // DEBUG 
        cout << "Capture" << endl;

        moveOccurred = true;
    }

    // otherwise, a normal piecemove
    else
    {
        assert(board[des.getRow()][des.getCol()]->getLetter() == ' ');
        assert(board[src.getRow()][src.getCol()]->getLetter() != ' ');
        assert(board[src.getRow()][src.getCol()]->getIsWhite() == move.getWhiteMove());

        // swap the piece
        swap(src, des);
        
        moveOccurred = true;
    }

    assertBoard();

    // advance by one move
    if (moveOccurred) {
        currentMove++;
        addMove(move);
    }

    return moveOccurred;
}
