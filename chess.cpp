/**********************************************************************
 * CHESS Main game file
 * The game of chess
 **********************************************************************/

#include "uiInteract.h"
#include "uiDraw.h"
#include "pieceType.h"
#include "board.h"
#include "piece.h"
#include "position.h"
#include "move.h"

using namespace std;

void callBack(Interface* pUI, void* p)
{
    Board* board = (Board*)p;
    set<Move> possible;

    Position source = pUI->getPreviousPosition();
    Position destination = pUI->getSelectPosition();
    Move moveAttempt = Move(source, destination);
    moveAttempt.complete(*board);

    // DEBUG
    //cout << source.getLocation() << " ," << destination.getLocation() << endl;

    if (source.getLocation() != -1 && board->move(moveAttempt)) {
        pUI->clearSelectPosition(); 
    }
    else if (source.getLocation() == -1) {

        // DEBUG
        //cout << (*board)[source].getLetter() << endl;
        //if ((*board)[source].getPieceType() == ROOK) {
        //    cout << "ITS A ROOK WHAT THE HECK" << endl;
        //}

        (*board)[source].getMoves(possible, *board);
    }

    // if we clicked on a blank spot, unselect it
    if (destination != -1 && (*board)[destination].getPieceType() == SPACE) {
        pUI->clearSelectPosition();
    }

    // draw the game
    board->display(source, *pUI, possible);
}

int main(int argc, char** argv)
{
    Interface ui("Chess");
    ogstream pgout;

    Board board(&pgout);
    board.reset(true);

    ui.run(callBack, &board);

    return 0;
}
