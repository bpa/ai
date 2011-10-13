#include <stdlib.h>
#include "choice.h"

Move *choice_random(State *state, Player p) {
	int i;
	vector<Move> *moves = state->moves_for_player(p);
	i = rand() % moves->size();
	return &moves->at(i);
}
