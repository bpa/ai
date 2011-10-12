#ifndef BOARD_H
#define BOARD_H
#include <sys/types.h>

class Board {
public:
	char tiles[8][8];
	u_int64_t hash;

	Board();
	Board(Board *board);
	void say();
	void generate_hash();
};

#endif
