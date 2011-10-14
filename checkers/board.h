#ifndef BOARD_H
#define BOARD_H
#include <vector>
#include <asm/types.h>
#include <sys/types.h>
#include "player.h"
#include "move.h"

#define NONE     0x00
#define RED      0x00
#define OCCUPIED 0x01
#define KING     0x02
#define BLACK    0x04

/*---------------
   11  05  31  25 
 10  04  30  24 
   03  29  23  17 
 02  28  22  16 
   27  21  15  09 
 26  20  14  08 
   19  13  07  01 
 18  12  06  00 
---------------*/
class Board {
public:
	__u32 red;
	__u32 black;
	__u32 kings;

	Board();
	Board(const char *);
	Board(Board *, Move *);
	void say();
	vector<Move> *generate_moves(Player);
};

#endif
