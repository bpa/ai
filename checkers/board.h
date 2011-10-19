#ifndef BOARD_H
#define BOARD_H
#include <glib.h>
#include <asm/types.h>
#include <sys/types.h>
#include "player.h"
#include "move.h"

class Board;

typedef struct st_child {
	Move move;
	Board *board;
} Child;

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
	Player player;
	GList *parents;
	GList *children;
	int value, min, max, refcount;

	Board(Player);
	Board(Player, const char *);
	Board(Board *, Move *);
	~Board();
	void generate_moves();
	void init();
	void add_jump_moves();
	void add_normal_moves();
	bool operator< (const Board &) const;
	void operator= (const Board &);
};

ostream &operator<<(ostream &, const Board &);

#endif
