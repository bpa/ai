#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "move.h"

Move *move_new() {
	Move *move = (Move*)malloc(sizeof(Move));
	bzero(move->tiles, 13);
	move->moves = 0;
	return move;
}

Move *move_from_string(const char *buf) {
	Move *move = (Move*)malloc(sizeof(Move));
    int i=0;
	bzero(move->tiles, 13);
	const char *ind = buf;
    while(*ind != '\0') {
        move->tiles[i] = (char) atoi(ind);
		i++;
		while (isdigit(*ind)) ind++;
		if (*ind != '\0') ind++;
    }
	move->moves = i;
	return move;
}

void move_add_tile(Move *move, char tile) {
	move->tiles[move->moves] = tile;
	move->moves++;
}

void move_to_string(const Move *move, char *buf) {
	char tmp[4];
	int i = 0;
	buf[0] = '\0';
	if (i<move->moves) {
		sprintf(tmp, "%i", move->tiles[i]);
		strcat(buf, tmp);
		i++;
	}
	for(; i<move->moves; i++) {
		sprintf(tmp, "-%i", move->tiles[i]);
		strcat(buf, tmp);
	}
}

int move_cmp (const Move *a, const Move *b) {
	if (a->moves != b->moves) return a->moves - b->moves;
	for (int i=0; i<a->moves; i++) {
		if (a->tiles[i] != b->tiles[i]) return (int)a->tiles[i] - (int)b->tiles[i];
	}
	return 0;
}
