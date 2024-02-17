/**********************************************************************
 * CHESS Main game file
 * The game of chess
 **********************************************************************/

#include "uiInteract.h"
#include "uiDraw.h"
#include "pieceType.h"
#include "piece.h"
#include "board.h"
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

    if (source.getLocation() != -1 && board->move(moveAttempt)) {
        pUI->clearSelectPosition(); 
    }
    else if (destination.getLocation() != -1) {
        (*board)[destination].getMoves(possible, *board);
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
