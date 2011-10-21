#ifndef AI_H
#define AI_H

#include <map>
#include <queue>
#include <glib.h>
#include "board.h"

class AI {
public:
	AI(Player, float);
	Move *choose_next_move();
	void process_node(Board *);
	void move(const char *);
	void execute_move(Move *);
	void search();
	void gc();
	void gc_step();
	void gc_garbage();

	Board *current;
	int pieces;
	GTree *states;
	GTree *steps[25];
	GSequence *backlog;
	GQueue *garbage;
};

#endif
