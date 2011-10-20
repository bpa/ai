#ifndef AI_H
#define AI_H

#include <map>
#include <queue>
#include <glib.h>
#include "board.h"

typedef struct st_key {
	__u32 red;
	__u32 black;
	__u32 kings;
	__u32 player;
} Key;

#define KEY(id, board) \
  Key id; \
  id.red = board->red; \
  id.black = board->black; \
  id.kings = board->kings; \
  id.player = board->player;

bool key_cmp(Key &, Key &);

class AI {
public:
	AI(Player, float);
	Move *choose_next_move();
	void move(const char *);
	void execute_move(Move *);
	void search();
	void gc();

	Board *current;
	int states_in_memory;
	GTree *states;
	GSequence *backlog;
	GQueue *garbage;
};

#endif
