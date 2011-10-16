#ifndef AI_H
#define AI_H

#include <map>
#include <queue>
#include <vector>
#include "board.h"

class AI {
public:
	AI(Player, float);
	Move *choose_next_move();
	void move(const char *);
	void execute_move(Move *);
	void search();
	void gc();

	Board *current;
	map<Board, Board*> states;
	queue<Board> backlog;
	queue<Board*> garbage;
};

#endif
