#include <iostream>
#include <sstream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "move.h"

Move::Move() {
	bzero(tiles, 13);
	moves = 0;
}

Move::Move(const char *buf) {
    int i=0;
	bzero(tiles, 13);
	const char *ind = buf;
    while(*ind != '\0') {
        tiles[i] = (char) atoi(ind);
		i++;
		while (isdigit(*ind)) ind++;
		if (*ind != '\0') ind++;
    }
	moves = i;
}

void Move::addTile(char tile) {
	tiles[moves] = tile;
	moves++;
}

const char *Move::str() const {
	stringstream ss;
	int i = 0;
	if (i<moves) {
		ss << (int)tiles[i];
		i++;
	}
	for(; i<moves; i++) {
		ss << '-' << (int)tiles[i];
	}
	ss << '\0';
	return ss.str().c_str();
}

int Move::cmp (const Move *that) const {
	if (moves != that->moves) return moves - that->moves;
	for (int i=0; i<moves; i++) {
		if (tiles[i] != that->tiles[i]) return tiles[i] - that->tiles[i];
	}
	return 0;
}
