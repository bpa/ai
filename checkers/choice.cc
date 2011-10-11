#include <stdlib.h>
#include "choice.h"

Move *choice_random(State *state, Player p) {
	int i;
	MoveList *moves = state_moves(state, p);
	i = rand() % moves->length;
	return &moves->moves[i];
}
