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
	moves = 0;
	const char *ind = buf;
    while(*ind != '\0') {
        tiles[i] = (char) atoi(ind);
		i++;
		while (isdigit(*ind)) ind++;
		ind++;
    }
}

void Move::addTile(char tile) {
	tiles[moves] = tile;
	moves++;
}

void Move::to_string(char *buf) {
    int i;
	char temp[8];

    buf[0] = '\0';
    for(i=0; tiles[i] != '\0'; i++) {
		sprintf(temp, "%d", tiles[i]);
		strcat(buf, temp);
		strcat(buf, "-");
    }
    buf[strlen(buf)-1] = '\0';
}

string Move::str() {
	stringstream ss;
	char *t = tiles;
	if (*t != '\0') ss << *t;
	for(; *t!='\0'; t++) {
		ss << '-' << *t;
	}
	return ss.str();
}

bool Move::operator< (const Move &that) const {
	if (moves < that.moves) return true;
	for (int i=0; i<moves; i++) {
		if (tiles[i] < that.tiles[i]) return true;
	}
	return false;
}
