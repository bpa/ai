#ifndef STATE_H
#define STATE_H
#include "board.h"
#include "move.h"

typedef struct st_state {
    Board board;
	long int hash;
    MoveList *moves[2];
	int value;
} State;

State *state_init(const Board *board);
State *state_transition(State *, Move *);
MoveList *state_moves(State *, Player);

#endif
