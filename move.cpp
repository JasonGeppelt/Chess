/***********************************************************************
 * Source File:
 *    MOVE : Keep track of a single chess move
 * Author:
 *    Br. Helfrich
 * Summary:
 *    Everything we need to know about a single chess move
 ************************************************************************/

#include "move.h"
#include "piece.h"
#include "board.h"
#define NDEBUG
#include <cassert>
#include <iostream>
#include <sstream>

using namespace std;

/***************************************************
 * MOVE : DEFAULT CONSTRUCTOR
 ***************************************************/
Move::Move() :
      promote(SPACE),       capture(SPACE),       enpassant(false),
      castleK(false),     castleQ(false),       isWhiteTurn(true)
{
}
/***************************************************
 * MOVE : PARAMETER CONSTRUCTOR
 * Constructor with source and destination positions
 ***************************************************/
Move::Move(const Position& src, const Position& dest, PieceType promote) :
    source(src), dest(dest), promote(promote), capture(SPACE), enpassant(false),
    castleK(false), castleQ(false), isWhiteTurn(true) {}

/***********************************************
 * MOVE : COMPLETE
 * We have a move that has nothing but a source
 * and a destination. Figure out what it should be
 **********************************************/
void Move::complete(const Board& board)
{
    // set the capture
    capture = board[dest].getPieceType();

    // set the color
    isWhiteTurn = board[source].getIsWhite();

    // handle if this is a ppromotion
    if (board[source].getPieceType() == PAWN)
    {
        // For white pawns reaching the 8th rank or black pawns reaching the 1st rank
        if ((isWhiteTurn && dest.getRow() == 0) || (!isWhiteTurn && dest.getRow() == 7))
        {
            // Set promotion to QUEEN by default
            promote = QUEEN;
        }
    }

    // handle if this is an en-passant
    if (board[source].getPieceType() == PAWN) {
        // Check if the move meets the conditions for en passant
        if (!board.getMoveHistory().empty() && capture == SPACE &&
            abs(board.getLastMove().getDes().getCol() - source.getCol()) == 1 &&
            abs(board.getLastMove().getDes().getRow() == source.getRow()) &&
            abs(board.getLastMove().getSrc().getRow() - source.getRow()) == 2 &&
            dest.getCol() == board.getLastMove().getDes().getCol())
        {
            // Set move as an enpassant
            enpassant = true;
        }
    }

    // handle if this is a castling
    if (board[source].getPieceType() == KING) {
        // Check for kingside castling
        if (dest.getCol() == source.getCol() + 2) {
            Position rookPos(source.getRow(), 7); // Rook's position for kingside castling
            if (board[rookPos].getPieceType() == ROOK) {
                // Check if the squares between the king and the rook are empty
                bool squaresEmpty = true;
                for (int col = source.getCol() + 1; col < 7; ++col) {
                    if (board[Position(source.getRow(), col)].getPieceType() != SPACE) {
                        squaresEmpty = false;
                        break;
                    }
                }
                if (squaresEmpty) {
                    castleK = true; // Set move as a King side Castle
                }
            }
        }
        // Check for queenside castling
        else if (dest.getCol() == source.getCol() - 2) {
            Position rookPos(source.getRow(), 0); // Rook's position for queenside castling
            if (board[rookPos].getPieceType() == ROOK) {
                // Check if the squares between the king and the rook are empty
                bool squaresEmpty = true;
                for (int col = source.getCol() - 1; col > 0; --col) {
                    if (board[Position(source.getRow(), col)].getPieceType() != SPACE) {
                        squaresEmpty = false;
                        break;
                    }
                }
                if (squaresEmpty) {
                    castleQ = true; // Set move as a Queen side Castle
                }
            }
        }
    }
}


/***********************************************
 * MOVE : EQUALITY
 **********************************************/
bool Move::operator == (const Move & rhs) const
{
   if (source    == rhs.getSrc()       &&
       dest      == rhs.getDes()       &&
       castleK   == rhs.getCastleK()   &&
       castleQ   == rhs.getCastleQ()   &&
       enpassant == rhs.getEnPassant() &&
       capture   == rhs.getCapture()   &&
       promote   == rhs.getPromotion())
   {
      assert(enpassant == rhs.getEnPassant());
      assert(castleK   == rhs.getCastleK());
      assert(castleQ   == rhs.getCastleQ());
      assert(promote   == rhs.getPromotion());
      return true;
   }
   else
      return false;
}

/***********************************************
 * MOVE : LETTER FROM PIECE TYPE
 *        Get the Smith notation letter for a piece from
 *        the Piece Type
 *********************************************/
char Move::letterFromPieceType(PieceType pt) const
{
   switch (pt)
   {
      case SPACE:
         return ' ';
      case KING:
         return 'k';
      case QUEEN:
         return 'q';
      case ROOK:
         return 'r';
      case KNIGHT:
         return 'n';
      case BISHOP:
         return 'b';
      case PAWN:
         return 'p';
   }
   assert(false);
   return ' ';
}

/***********************************************
 * MOVE : PIECE TYPE FROM LETTER
 *        Get the Piece Type for a piece from
 *        the Smith notation letter
 *********************************************/
PieceType Move::pieceTypeFromLetter(char letter) const
{
   switch (letter)
   {
      case 'k':
         return KING;
      case 'q':
         return QUEEN;
      case 'r':
         return ROOK;
      case 'b':
         return BISHOP;
      case 'n':
         return KNIGHT;
      case 'p':
         return PAWN;
   }
   assert(false);
   return SPACE;
}


/***********************************************
 * MOVE : GET TEXT
 *        Generate the human-readable text
 *********************************************/
string Move::getText() const
{
   // if there is an error, use it instead
   if (error.length())
      return error;
   
   // use the string stream so we can use the insertion operators of
   // the position class
   std::ostringstream sout;
   sout << source << dest;

   // handle all the special stuff
   if (enpassant)
      sout << "E";
   if (castleK)
      sout << "c";
   if (castleQ)
      sout << "C";
   if (promote != SPACE)
      sout << toupper(letterFromPieceType(promote));
   if (capture != SPACE && !enpassant)
      sout << letterFromPieceType(capture);

   // return the result as a string
   return sout.str();
}

/**********************************************
 * Move : ASSIGNMENT
 **********************************************/
const Move & Move::operator = (const Move & rhs)
{
   source    = rhs.getSrc();
   dest      = rhs.getDes();
   promote   = rhs.getPromotion();
   capture   = rhs.getCapture();
   enpassant = rhs.getEnPassant();
   castleK   = rhs.getCastleK();
   castleQ   = rhs.getCastleQ();
   isWhiteTurn   = rhs.getWhiteMove();
   error     = rhs.error;
   return *this;
}


/**********************************************
 * MOVE : READ
 * Parse the input text into its components.
 * We will not validate until later...
 **********************************************/
void Move::read(const string & s)
{
   // clear the move
   *this = Move();

   //
   // from square
   //
   source = s.c_str() + 0;  // start at the start

   //
   // to square
   //
   dest = s.c_str() + 2;   // start two characters in, the destination

   //
   // capture and promotion
   //

   for (int i = 4; i < (int)s.size(); i++)
   {
      switch(s[i])
      {
         case 'p':   // capture a pawn
            capture = PAWN;
            break;
         case 'n':   // capture a knight
            capture = KNIGHT;
            break;
         case 'b':   // capture a bishop
            capture = BISHOP;
            break;
         case 'r':   // capture a rook
            capture = ROOK;
            break;
         case 'q':   // capture a queen
            capture = QUEEN;
            break;
         case 'k':   // !! you can't capture a king you silly!
            capture = KING;
            break;

         case 'c':  // short castling or kings castle
            castleK = true;
            break;
         case 'C':  // long castling or queen castle
            castleQ = true;
            break;
         case 'E':  // En-passant
            enpassant = true;
            break;

         case 'N':  // Promote to knight
            promote = KNIGHT;
            break;
         case 'B':  // Promote to Bishop
            promote = BISHOP;
            break;
         case 'R':  // Promote to Rook
            promote = ROOK;
            break;
         case 'Q':  // Promote to Queen
            promote = QUEEN;
            break;

         default:
            // unknown piece
            error = s;
            throw string("Unknown promotion piece specification");
      }
   }

   // if we made it this far, we are successful
   return;
}


/******************************************
 * MOVE INSERTION OPERATOR
 *       Aint nothing but a glorified getText call...
 ******************************************/
ostream & operator << (ostream & out, Move & rhs)
{
   out << rhs.getText();

   return out;
}

/*************************************
 * MOVE EXTRACTION OPERATOR
 *      Just the assignment operator with a string
 *      on the right-hand-side
 **************************************/
istream & operator >> (istream & in,  Move & rhs)
{   
   string s;
   if (in >> s)
      rhs = s; // assigment operator calling Move::read()

   return in;   
}

