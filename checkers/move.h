#ifndef MOVE_H
#define MOVE_H

typedef struct st_move {
	char tiles[13];
	int moves;
} Move;

Move *move_new();
Move *move_from_string(const char *);
void move_add_tile(Move *, char);
void move_to_string(const Move *, char *buf);
int move_cmp(const Move*, const Move*);

#endif
