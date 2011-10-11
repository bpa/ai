#ifndef MOVE_H
#define MOVE_H
#include "board.h"
#include "player.h"

typedef char Move[13];

typedef struct st_move_list {
	int length;
	Move moves[48];
} MoveList;

MoveList *move_list_create(Board *, Player);

Move *move_from_string(const char *);
void move_to_string(Move *, char *);

#endif
