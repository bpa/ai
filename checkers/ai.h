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
	void init_timer(float limit);

	Board *current;
	int pieces;
	GHashTable *states;
	GHashTable *steps[25];
	GQueue *garbage;
	timer_t timerid;
	struct itimerspec turn_time_limit;
};

#endif
