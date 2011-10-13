#ifndef STATE_H
#define STATE_H
#include <vector>
class State;
#include "board.h"
#include "move.h"
#include "player.h"

using namespace std;

class State {
public:
    Board board;
	long int hash;
	int value;

	State();
	State(const Board *board);
	State(State *, Move *);
	vector<Move> *moves_for_player(Player);

private:
    vector<Move> moves[2];
};

#endif
